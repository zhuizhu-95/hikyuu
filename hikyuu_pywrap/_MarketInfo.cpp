/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-26
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include "pickle_support.h"
#include <hikyuu/serialization/MarketInfo_serialization.h>

using namespace hku;
namespace py = pybind11;

void export_MarketInfo(py::module& m) {
    py::class_<MarketInfo>(m, "MarketInfo", "市场信息记录")
      .def(py::init<>())
      .def(py::init<const string&, const string&, const string&, const string&, const Datetime&>())
      .def("__str__", &MarketInfo::toString)
      .def("__repr__", &MarketInfo::toString)
      .def_property_readonly("market", &MarketInfo::market, py::return_value_policy::copy,
                             "市场简称")
      .def_property_readonly("name", &MarketInfo::name, py::return_value_policy::copy, "市场名称")
      .def_property_readonly("description", &MarketInfo::description, py::return_value_policy::copy,
                             "市场说明信息")
      .def_property_readonly("code", &MarketInfo::code, py::return_value_policy::copy,
                             "该市场对应的指数代码")
      .def_property_readonly("last_date", &MarketInfo::lastDate, "该市场数据的最后更新日期")

        DEF_PICKLE(MarketInfo);
}