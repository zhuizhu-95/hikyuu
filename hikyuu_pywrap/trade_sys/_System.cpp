/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/system/build_in.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_System(py::module& m) {}