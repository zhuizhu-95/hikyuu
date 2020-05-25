/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-25
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/datetime/TimeDelta.h>
#include "../pybind_utils.h"
#include "../pickle_support.h"

#if HKU_PYTHON_SUPPORT_PICKLE
#include <hikyuu/serialization/TimeDelta_serialization.h>
#endif

#include "_TimeDelta.h"

using namespace hku;
namespace py = pybind11;

void export_TimeDelta(py::module& m) {
    m.def("print_delta", [](TimeDelta& t) { std::cout << t << std::endl; });
}