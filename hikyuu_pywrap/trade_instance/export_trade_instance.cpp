/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-18
 *      Author: fasiondog
 */

#include <hikyuu/indicator/build_in.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

void export_AmaInstance(py::module& m);

void export_trade_instance(py::module& m) {
    export_AmaInstance(m);
}