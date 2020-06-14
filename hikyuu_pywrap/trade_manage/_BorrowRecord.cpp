/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-10
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_manage/BorrowRecord.h>
#include "../convert_Datetime.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_BorrowRecord(py::module& m) {
    py::class_<BorrowRecord>(m, "BorrowRecord", "记录当前借入的证券信息")
      .def(py::init<>())
      .def(py::init<const Stock&, double, price_t>())
      .def("__str__", to_py_str<BorrowRecord>)
      .def("__repr__", to_py_str<BorrowRecord>)
      .def_readwrite("stock", &BorrowRecord::stock, "借入证券")
      .def_readwrite("number", &BorrowRecord::number, "借入总数量")
      .def_readwrite("value", &BorrowRecord::value, "借入总价值")

        DEF_PICKLE(BorrowRecord);
}
