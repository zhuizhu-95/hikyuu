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
    py::class_<MarketInfo>(m, "MarketInfo")
      .def(py::init<>())
      .def(py::init<const string&, const string&, const string&, const string&, const Datetime&>())
      .def("__str__", &MarketInfo::toString)
      .def("__repr__", &MarketInfo::toString)
      .def_property_readonly("market", &MarketInfo::market, py::return_value_policy::reference,
                             "Market identification.")
      .def_property_readonly("name", &MarketInfo::name, py::return_value_policy::reference,
                             "The full name of market")
      .def_property_readonly("description", &MarketInfo::description,
                             py::return_value_policy::reference)
      .def_property_readonly("code", &MarketInfo::code, py::return_value_policy::reference,
                             "The market corresponds to the stock index code")
      .def_property_readonly("last_date", &MarketInfo::lastDate, py::return_value_policy::reference,
                             "The current final date of the market.")

        DEF_PICKLE(MarketInfo);
}