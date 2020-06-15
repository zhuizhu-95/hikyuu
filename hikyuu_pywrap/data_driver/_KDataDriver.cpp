/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-14
 *      Author: fasiondog
 */

#include "_KDataDriver.h"

void export_KDataDriver(py::module& m) {
    py::class_<KDataDriver, KDataDriverPtr, PyKDataDriver>(m, "KDataDriver")
      .def(py::init<>())
      .def(py::init<const string&>())

      .def_property_readonly("name", &KDataDriver::name)

      .def("__str__", to_py_str<KDataDriver>)
      .def("__repr__", to_py_str<KDataDriver>)

      .def("getParam", &KDataDriver::getParam<boost::any>)

      .def("_init", &KDataDriver::_init)
      .def("loadKData", &KDataDriver::loadKData)
      .def("getCount", &KDataDriver::getCount)
      .def("getKRecord", &KDataDriver::getKRecord)
      //.def("_getIndexRangeByDate", &KDataDriverWrap::_getIndexRangeByDate,
      //    &KDataDriverWrap::default_getIndexRangeByDate)

      ;
}