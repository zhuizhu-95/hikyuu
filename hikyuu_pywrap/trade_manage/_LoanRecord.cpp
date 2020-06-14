/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-10
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <hikyuu/trade_manage/LoanRecord.h>
#include "../convert_Datetime.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_LoanRecord(py::module& m) {
    py::class_<LoanRecord>(m, "LoanRecord", "借款记录（融资记录）")
      .def(py::init<>())
      .def(py::init<const Datetime&, price_t>(), py::arg("date"), py::arg("value"))

      .def("__str__", to_py_str<LoanRecord>)
      .def("__repr__", to_py_str<LoanRecord>)

      .def_readwrite("date", &LoanRecord::datetime, "借入时间")
      .def_readwrite("value", &LoanRecord::value, "借入资产价值")

        DEF_PICKLE(LoanRecord);
}
