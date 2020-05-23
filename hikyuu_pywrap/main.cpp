/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-17
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/hikyuu.h>

using namespace hku;
namespace py = pybind11;

void export_Datetime(py::module& m);

PYBIND11_MODULE(_hikyuu, m) {
    m.def("getVersion", &getVersion);
    m.def("getVersionWithBuild", &getVersionWithBuild);

    export_Datetime(m);
}