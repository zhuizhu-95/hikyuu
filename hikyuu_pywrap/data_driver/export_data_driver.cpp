/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-14
 *      Author: fasiondog
 */

#include <hikyuu/indicator/build_in.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

void export_BaseInfoDriver(py::module& m);
void export_BlockInfoDriver(py::module& m);
void export_KDataDriver(py::module& m);
void export_DataDriverFactory(py::module& m);

void export_data_driver(py::module& m) {
    export_BaseInfoDriver(m);
    export_BlockInfoDriver(m);
    export_KDataDriver(m);
    export_DataDriverFactory(m);
}