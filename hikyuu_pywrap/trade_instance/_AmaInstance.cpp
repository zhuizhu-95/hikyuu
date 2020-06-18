/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-18
 *      Author: fasiondog
 */

#include <hikyuu/indicator/build_in.h>
#include <pybind11/pybind11.h>
#include <hikyuu/trade_instance/ama_sys/AmaInstance.h>

using namespace hku;
namespace py = pybind11;

void export_AmaInstance(py::module& m) {
    m.def("AmaSpecial", AmaSpecial);
}
