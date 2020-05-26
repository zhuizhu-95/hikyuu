/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-26
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/KData.h>
#include <hikyuu/Stock.h>
#include "_Parameter.h"
#include "convert_Datetime.h"
#include "pickle_support.h"
#include <hikyuu/serialization/Stock_serialization.h>

using namespace hku;
namespace py = pybind11;

void export_Stock(py::module& m) {
    py::class_<Stock>(m, "Stock")
      .def(py::init<>())
      .def(py::init<const string&, const string&, const string&>())
      .def(py::init<const Stock&>())
      .def("__str__", &Stock::toString)
      .def("__repr__", &Stock::toString)

      .def_property_readonly("id", &Stock::id)
      .def_property_readonly("market", &Stock::market, py::return_value_policy::reference)
      .def_property_readonly("code", &Stock::code, py::return_value_policy::reference)
      .def_property_readonly("market_code", &Stock::market_code, py::return_value_policy::reference)
      .def_property_readonly("name", &Stock::name, py::return_value_policy::reference)

      .def_property_readonly("type", &Stock::type)
      .def_property_readonly("valid", &Stock::valid)
      .def_property_readonly("start_datetime", &Stock::startDatetime)
      .def_property_readonly("last_datetime", &Stock::lastDatetime)
      .def_property_readonly("tick", &Stock::tick)
      .def_property_readonly("tick_value", &Stock::tickValue)
      .def_property_readonly("unit", &Stock::unit)
      .def_property_readonly("precision", &Stock::precision)
      .def_property_readonly("atom", &Stock::atom)
      .def_property_readonly("min_tradeNumber", &Stock::minTradeNumber)
      .def_property_readonly("max_tradeNumber", &Stock::maxTradeNumber)

      .def("is_null", &Stock::isNull)
      .def("is_buffer", &Stock::isBuffer)
      .def("get_kdata", &Stock::getKData)
      .def("get_timeline_list", &Stock::getTimeLineList)
      .def("get_trans_list", &Stock::getTransList)
      .def("get_count", &Stock::getCount, py::arg("ktype") = KQuery::DAY)
      .def("get_market_value", &Stock::getMarketValue)
      .def("get_krecord", &Stock::getKRecord, py::arg("pos"), py::arg("ktype") = KQuery::DAY)
      .def("get_Krecord_by_dateB", &Stock::getKRecordByDate, py::arg("date"),
           py::arg("ktype") = KQuery::DAY)
      .def("get_krecord_list", &Stock::getKRecordList)
      .def("get_datetime_list",
           py::overload_cast<size_t, size_t, KQuery::KType>(&Stock::getDatetimeList, py::const_))
      .def("get_datetime_list",
           py::overload_cast<const KQuery&>(&Stock::getDatetimeList, py::const_))
      .def("get_finance_info", &Stock::getFinanceInfo)
      .def("get_history_finance_info", &Stock::getHistoryFinanceInfo)
      .def("realtime_update", &Stock::realtimeUpdate)
      //.def("getWeight", getWeight1)
      //.def("getWeight", getWeight2)
      .def("load_kdata_to_buffer", &Stock::loadKDataToBuffer)
      .def("release_kdata_buffer", &Stock::releaseKDataBuffer)

      .def("__eq__", &Stock::operator==)
      .def("__ne__", &Stock::operator!=)

        DEF_PICKLE(Stock);

    m.def("get_stock", &getStock, "Return the stock object of the specified code, like 'sh000001'");
}