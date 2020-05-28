/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-26
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/KData.h>
#include <hikyuu/Stock.h>
#include <hikyuu/serialization/Stock_serialization.h>
#include "_Parameter.h"
#include "convert_Datetime.h"
#include "pickle_support.h"
#include "StockType.h"
#include "pybind_utils.h"

using namespace hku;
namespace py = pybind11;

void export_Stock(py::module& m) {
    py::class_<Stock>(m, "Stock")
      .def(py::init<>())
      .def(py::init<const string&, const string&, const string&>(), py::arg("market"),
           py::arg("code"), py::arg("name"))
      .def(py::init<const Stock&>())
      .def("__str__", &Stock::toString)
      .def("__repr__", &Stock::toString)

      .def_property_readonly("market", &Stock::market, py::return_value_policy::copy,
                             "所属市场简称")
      .def_property_readonly("code", &Stock::code, py::return_value_policy::copy, "证券代码")
      .def_property_readonly("market_code", &Stock::market_code, py::return_value_policy::copy,
                             "市场简称+证券代码，如: sh000001")
      .def_property_readonly("name", &Stock::name, py::return_value_policy::copy, "证券名称")
      .def_property_readonly(
        "type", [](const Stock& stock) { return StockType(stock.type()); }, "证券类型")
      .def_property_readonly("valid", &Stock::valid, "该证券当前是否有效")
      .def_property_readonly("start_datetime", &Stock::startDatetime, "证券起始日期")
      .def_property_readonly("last_datetime", &Stock::lastDatetime, "证券最后日期")
      .def_property_readonly("tick", &Stock::tick, "最小跳动量")
      .def_property_readonly("tick_value", &Stock::tickValue, "最小跳动量价格")
      .def_property_readonly("unit", &Stock::unit, "每单位价值 = tickValue / tick")
      .def_property_readonly("precision", &Stock::precision, "价格精度")
      .def_property_readonly("atom", &Stock::atom, "允许的最小交易数量，同min_trade_number")
      .def_property_readonly("min_tradeNumber", &Stock::minTradeNumber, "允许的最小交易数量")
      .def_property_readonly("max_tradeNumber", &Stock::maxTradeNumber, "允许的最大交易数量")

      .def("is_null", &Stock::isNull, "是否为空对象")
      .def("is_buffer", &Stock::isBuffer, "指定类型的K线数据是否被缓存")

      .def("get_kdata", &Stock::getKData)
      .def("get_timeline_list", &Stock::getTimeLineList)
      .def("get_trans_list", &Stock::getTransList)

      .def("get_count", &Stock::getCount, py::arg("ktype") = KQuery::DAY,
           R"(获取指定类型的K线数据量

:param Query.KType ktype: K线数据类别
:return: K线记录数)")

      .def(
        "get_market_value", &Stock::getMarketValue,
        R"(获取指定时刻的市值，即小于等于指定时刻的最后一条记录的收盘价

:param datetime: 指定时刻
:param Query.KType ktype: K线数据类别
:return: 指定时刻的市值)")

      .def("get_krecord", &Stock::getKRecord, py::arg("pos"), py::arg("ktype") = KQuery::DAY)
      .def("get_Krecord_by_dateB", &Stock::getKRecordByDate, py::arg("date"),
           py::arg("ktype") = KQuery::DAY)
      .def("get_krecord_list", &Stock::getKRecordList)
      .def("get_datetime_list",
           [](const Stock& stk, size_t start, size_t end, KQuery::KType ktype) {
               return vector_to_python_list<Datetime>(stk.getDatetimeList(start, end, ktype));
           })

      .def("get_datetime_list",
           [](const Stock& stk, const KQuery& q) {
               return vector_to_python_list<Datetime>(stk.getDatetimeList(q));
           })

      .def("get_finance_info", &Stock::getFinanceInfo)
      .def("get_history_finance_info", &Stock::getHistoryFinanceInfo)
      .def("realtime_update", &Stock::realtimeUpdate)
      //.def("getWeight", getWeight1)
      //.def("getWeight", getWeight2)
      .def("load_kdata_to_buffer", &Stock::loadKDataToBuffer)
      .def("release_kdata_buffer", &Stock::releaseKDataBuffer)

      .def("__hash__", [](const Stock& stock) { return (Py_hash_t)stock.id(); })

      .def("__eq__", &Stock::operator==)
      .def("__ne__", &Stock::operator!=)

        DEF_PICKLE(Stock);

    m.def("get_stock", &getStock, "Return the stock object of the specified code, like 'sh000001'");
}