/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-30
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include "convert_Datetime.h"
#include "pickle_support.h"
#include <hikyuu/serialization/TransRecord_serialization.h>

using namespace hku;
namespace py = pybind11;

bool (*transrecord_eq)(const TransRecord&, const TransRecord&) = operator==;

string TransRecord_to_string(const TransRecord& tr) {
    string direct;
    switch (tr.direct) {
        case TransRecord::BUY:
            direct = "BUY";
            break;

        case TransRecord::SELL:
            direct = "SELL";
            break;

        case TransRecord::AUCTION:
            direct = "AUCTION";
            break;

        default:
            direct = "INVALID";
            break;
    }
    return fmt::format("TransRecord({}, {:<.4f}, {:<.4f}, {})", tr.datetime.str(), tr.price, tr.vol,
                       direct);
}

void export_TransRecord(py::module& m) {
    py::class_<TransRecord> trans(m, "TransRecord", "分笔成交记录");
    trans.def(py::init<>())
      .def_readwrite("date", &TransRecord::datetime, "日期时间")
      .def_readwrite("price", &TransRecord::price, "价格")
      .def_readwrite("vol", &TransRecord::vol, "成交数量")
      .def_readwrite("direct", &TransRecord::direct, "成交方向")

      .def("is_valid", &TransRecord::isValid, "日期无效时认为为无效记录")

      .def("__str__", TransRecord_to_string)
      .def("__repr__", TransRecord_to_string)
      .def("__eq__", transrecord_eq)

        DEF_PICKLE(TransRecord);

    py::enum_<TransRecord::DIRECT>(trans, "DIRECT")
      .value("BUY", TransRecord::BUY, "买盘")
      .value("SELL", TransRecord::SELL, "卖盘")
      .value("AUCTION", TransRecord::AUCTION, "集合竞价")
      .export_values();

    trans.def(py::init<const Datetime&, price_t, price_t, TransRecord::DIRECT>());
}