/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-10
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <hikyuu/trade_manage/FundsRecord.h>
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_FundsRecord(py::module& m) {
    py::class_<FundsRecord>(m, "FundsRecord", "当前资产情况记录")
      .def(py::init<>())

      .def("__str__", to_py_str<FundsRecord>)
      .def("__repr__", to_py_str<FundsRecord>)

      .def_readwrite("cash", &FundsRecord::cash, "当前现金")
      .def_readwrite("market_value", &FundsRecord::market_value, "当前多头市值")
      .def_readwrite("short_market_value", &FundsRecord::short_market_value, "当前空头仓位市值")
      .def_readwrite("base_cash", &FundsRecord::base_cash, "当前投入本金principal")
      .def_readwrite("base_asset", &FundsRecord::base_asset, "当前投入的资产价值")
      .def_readwrite("borrow_cash", &FundsRecord::borrow_cash, "当前借入的资金，即负债")
      .def_readwrite("borrow_asset", &FundsRecord::borrow_asset, "当前借入证券资产价值")

        DEF_PICKLE(FundsRecord);
}
