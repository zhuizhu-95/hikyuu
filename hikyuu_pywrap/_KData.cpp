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

void export_KData(py::module& m) {
    py::class_<KData>(
      m, "KData",
      "通过 Stock.getKData 获取的K线数据，由 KRecord 组成的数组，可象 list 一样进行遍历")

      .def(py::init<>())
      .def(py::init<const KData&>())
      .def(py::init<const Stock&, const KQuery&>())

      .def("__str__", &KData::toString)
      .def("__repr__", &KData::toString)
      .def("__len__", &KData::size)

      .def_property_readonly("start_pos", &KData::startPos,
                             "在原始K线记录中对应的起始位置，如果为空返回0")

      .def_property_readonly(
        "end_pos", &KData::endPos,
        "在原始K线记录中对应范围的下一条记录的位置，如果为空返回0,其他等于last_pos + 1")

      .def_property_readonly(
        "last_pos", &KData::lastPos,
        "获取在原始K线记录中对应的最后一条记录的位置，如果为空返回0,其他等于end_pos - 1 ")

      .def_property_readonly("OPEN", &KData::open, "开盘价指标")
      .def_property_readonly("CLOSE", &KData::close, "收盘价指标")
      .def_property_readonly("HIGH", &KData::high, "最高价指标")
      .def_property_readonly("LOW", &KData::low, "最低价指标")
      .def_property_readonly("AMO", &KData::amo, "成交金额指标")
      .def_property_readonly("VOL", &KData::vol, "成交量指标")

      .def(
        "get_date_List", [](const KData& k) { return vector_to_python_list(k.getDatetimeList()); },
        "返回交易日期列表")

      .def("get", &KData::getKRecord, "获取指定位置的KRecord，未作越界检查")

      .def("get_by_date", &KData::getKRecordByDate, "按日期查询KRecord")

      .def("_get_Pos", &KData::getPos,
           "按日期查询对应的索引位置")  // python中需要将Null的情况改写为None

      .def("size", &KData::size)
      .def("empty", &KData::empty)
      .def("get_query", &KData::getQuery, "获取关联的 Query 对象")
      .def("get_stock", &KData::getStock, "获取关联的Stock，如果没有关联，返回无效的 Stock")
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
}