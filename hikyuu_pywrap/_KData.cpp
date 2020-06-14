/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-30
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include "convert_Datetime.h"
#include "pickle_support.h"
#include <hikyuu/serialization/KData_serialization.h>
#include <hikyuu/indicator/crt/KDATA.h>

using namespace hku;
namespace py = pybind11;

string print_KData(const KData& k) {
    std::stringstream buf;
    // buf << k << std::endl;
    buf << "----------------------------------------------------------------" << std::endl;
    buf << "datetime             open     high    low    close    amo    vol" << std::endl;
    buf << "----------------------------------------------------------------" << std::endl;
    size_t max_print = 20;
    size_t total = k.size();
    string strip("  ");
    if (total <= max_print) {
        for (size_t i = 0; i < total; ++i) {
            KRecord r = k.getKRecord(i);
            buf << r.datetime.str() << strip << r.openPrice << strip << r.highPrice << strip
                << r.lowPrice << strip << r.closePrice << strip << r.transAmount << strip
                << r.transCount << std::endl;
            ;
        }
    } else {
        for (size_t i = 0; i < max_print / 2; ++i) {
            KRecord r = k.getKRecord(i);
            buf << r.datetime.str() << strip << r.openPrice << strip << r.highPrice << strip
                << r.lowPrice << strip << r.closePrice << strip << r.transAmount << strip
                << r.transCount << std::endl;
            ;
        }
        buf << "..." << std::endl;
        for (size_t i = total - max_print / 2; i < total; ++i) {
            KRecord r = k.getKRecord(i);
            buf << r.datetime.str() << strip << r.openPrice << strip << r.highPrice << strip
                << r.lowPrice << strip << r.closePrice << strip << r.transAmount << strip
                << r.transCount << std::endl;
            ;
        }
    }

    return buf.str();
}

void export_KData(py::module& m) {
    py::class_<KData>(
      m, "KData",
      "通过 Stock.getKData 获取的K线数据，由 KRecord 组成的数组，可象 list 一样进行遍历")

      .def(py::init<>())
      .def(py::init<const KData&>())
      .def(py::init<const Stock&, const KQuery&>())

      .def("__str__", print_KData)
      .def("__repr__", &KData::toString)
      .def("__len__", &KData::size)

      .def_property_readonly("startPos", &KData::startPos,
                             "在原始K线记录中对应的起始位置，如果为空返回0")

      .def_property_readonly(
        "endPos", &KData::endPos,
        "在原始K线记录中对应范围的下一条记录的位置，如果为空返回0,其他等于last_pos + 1")

      .def_property_readonly(
        "lastPos", &KData::lastPos,
        "获取在原始K线记录中对应的最后一条记录的位置，如果为空返回0,其他等于end_pos - 1 ")

      .def_property_readonly("open", &KData::open, "开盘价指标")
      .def_property_readonly("close", &KData::close, "收盘价指标")
      .def_property_readonly("high", &KData::high, "最高价指标")
      .def_property_readonly("low", &KData::low, "最低价指标")
      .def_property_readonly("amo", &KData::amo, "成交金额指标")
      .def_property_readonly("vol", &KData::vol, "成交量指标")

      .def(
        "getDatetimeList",
        [](const KData& k) { return vector_to_python_list(k.getDatetimeList()); },
        "返回交易日期列表")

      .def("get", &KData::getKRecord, "获取指定位置的KRecord，未作越界检查")

      .def("getKRecordByDate", &KData::getKRecordByDate, "按日期查询KRecord")

      .def(
        "getPos",
        [](const KData& k, const Datetime& d) {
            size_t pos = k.getPos(d);
            return (pos == Null<size_t>()) ? py::none() : py::int_(pos);
        },
        R"(获取指定时间对应的索引位置

    :param datetime: 指定的时间
    :return: 对应的索引位置，如果不在数据范围内，则返回 None)")

      .def("size", &KData::size)
      .def("empty", &KData::empty)
      .def("getQuery", &KData::getQuery, "获取关联的 Query 对象")
      .def("getStock", &KData::getStock, "获取关联的Stock，如果没有关联，返回无效的 Stock")
      .def("tocsv", &KData::tocsv, "输出数据到指定的 csv 文件中")

      .def("__getitem__",
           [](const KData& k, long long i) {
               long long len = k.size();
               long long index = i;
               if (index < 0) {
                   index = len + index;
               }

               if (index < 0 || index >= len) {
                   HKU_THROW_EXCEPTION(std::out_of_range, "Index out of range: {}", i);
               }

               return k.getKRecord(index);
           })

      .def("__getitem__",
           [](const KData& k, const Datetime& date) { return k.getKRecordByDate(date); })

      .def("__getitem__",
           [](const KData& k, const string& date) { return k.getKRecordByDate(Datetime(date)); })

      .def("__getitem__",
           [](const KData& k, py::slice slice) {
               KRecordList result;
               size_t start, stop, step, slicelength;
               if (!slice.compute(k.size(), &start, &stop, &step, &slicelength)) {
                   throw py::error_already_set();
               }

               result.reserve((size_t)slicelength);
               for (size_t i = 0; i < slicelength; ++i) {
                   result.push_back(k.getKRecord(start));
                   start += step;
               }

               return vector_to_python_list(result);
           })

        DEF_PICKLE(KData);

    int64 null_int64 = Null<int64>();
    Datetime null_date = Null<Datetime>();

    m.def(
      "getKData",
      py::overload_cast<const string&, int64, int64, KQuery::KType, KQuery::RecoverType>(getKData),
      py::arg("market_code"), py::arg("start") = 0, py::arg("end") = null_int64,
      py::arg("ktype") = KQuery::DAY, py::arg("recover_type") = KQuery::NO_RECOVER,
      R"(根据证券代码及起止位置获取 [start, end) 范围的 K 线数据

    :param str market_code: 证券代码，如: 'sh000001'
    :param int start: 起始索引
    :param int end: 结束索引
    :param Query.KType ktype: K 线类型, 'DAY'|'WEEK'|'MONTH'|'QUARTER'|'HALFYEAR'|'YEAR'|'MIN'|'MIN5'|'MIN15'|'MIN30'|'MIN60'
    :param Query.RecoverType recover_type: 复权类型)");

    m.def("getKData",
          py::overload_cast<const string&, const Datetime&, const Datetime&, KQuery::KType,
                            KQuery::RecoverType>(getKData),
          py::arg("market_code"), py::arg("start") = Datetime::min(), py::arg("end") = null_date,
          py::arg("ktype") = KQuery::DAY, py::arg("recover_type") = KQuery::NO_RECOVER,
          R"(根据证券代码及起止日期获取 [start, end) 范围的 K 线数据

    :param str market_code: 证券代码，如: 'sh000001'
    :param int start: 起始日期
    :param int end: 结束日期
    :param Query.KType ktype: K 线类型, 'DAY'|'WEEK'|'MONTH'|'QUARTER'|'HALFYEAR'|'YEAR'|'MIN'|'MIN5'|'MIN15'|'MIN30'|'MIN60'
    :param Query.RecoverType recover_type: 复权类型)");
}