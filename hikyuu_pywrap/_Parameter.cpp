/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-24
 *      Author: fasiondog
 */

#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <hikyuu/utilities/Parameter.h>
#include "_Parameter.h"
#include "pickle_support.h"

using namespace hku;
namespace py = pybind11;

std::string Parameter_to_str(const Parameter& param) {
    std::stringstream ss;
    ss << param;
    return ss.str();
}

void export_Parameter(py::module& m) {
    py::class_<Parameter>(m, "Parameter")
      .def(py::init<>())
      .def("__str__", &Parameter_to_str)
      .def("__repr__", &Parameter_to_str)

      .def("have", &Parameter::have)

      .def("__setitem__", &Parameter::set<boost::any>)
      .def("__getitem__", &Parameter::get<boost::any>)
      .def("set", &Parameter::set<boost::any>)
      .def("get", &Parameter::get<boost::any>)
      .def("getNameList", &Parameter::getNameList)
      .def("getNameValueList", &Parameter::getNameValueList)

        DEF_PICKLE(Parameter);
}