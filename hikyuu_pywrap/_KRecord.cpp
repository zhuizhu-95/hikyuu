/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-30
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include "convert_Datetime.h"
#include "pickle_support.h"
#include <hikyuu/serialization/KRecord_serialization.h>

using namespace hku;
namespace py = pybind11;

std::string KRecord_to_string(const KRecord& k) {
    return fmt::format("KRecord({}, {:<.4f}, {:<.4f}, {:<.4f}, {:<.4f}, {:<.4f}, {:<.4f})",
                       k.datetime.str(), k.openPrice, k.highPrice, k.lowPrice, k.closePrice,
                       k.transAmount, k.transCount);
}

bool (*krecord_eq)(const KRecord&, const KRecord&) = operator==;

void export_KRecord(py::module& m) {
    py::class_<KRecord>(m, "KRecord", "K线数据记录")
      .def(py::init<>())
      .def(py::init<const Datetime&>())
      .def(py::init<const Datetime&, price_t, price_t, price_t, price_t, price_t, price_t>(),
           py::arg("date"), py::arg("open"), py::arg("high"), py::arg("low"), py::arg("close"),
           py::arg("amo"), py::arg("vol"))

      .def("__str__", KRecord_to_string)
      .def("__repr__", KRecord_to_string)
      .def("__eq__", krecord_eq)

      .def_readwrite("date", &KRecord::datetime, "时间")
      .def_readwrite("open", &KRecord::openPrice, "开盘价")
      .def_readwrite("high", &KRecord::highPrice, "最高价")
      .def_readwrite("low", &KRecord::lowPrice, "最低价")
      .def_readwrite("close", &KRecord::closePrice, "收盘价")
      .def_readwrite("amo", &KRecord::transAmount, "成交金额")
      .def_readwrite("vol", &KRecord::transCount, "成交数量")

        DEF_PICKLE(KRecord);
}