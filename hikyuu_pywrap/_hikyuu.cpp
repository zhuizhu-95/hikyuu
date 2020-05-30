/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-26
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/hikyuu.h>

using namespace hku;
namespace py = pybind11;

void export_hikyuu(py::module& m) {
    m.def("get_version", getVersion);
    m.def("get_version_with_build", getVersionWithBuild);

    m.def("hikyuu_init", hikyuu_init);
}