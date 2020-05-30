/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-30
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include "convert_Datetime.h"
#include "pickle_support.h"
#include <hikyuu/serialization/TimeLineRecord_serialization.h>

using namespace hku;
namespace py = pybind11;

bool (*timelinerecord_eq)(const TimeLineRecord&, const TimeLineRecord&) = operator==;

std::string TimeLineRecord_to_string(const TimeLineRecord& tr) {
    return fmt::format("TimeLineRecord({}, {:<.4f}, {:.4f})", tr.datetime.str(), tr.price, tr.vol);
}

void export_TimeLineRecord(py::module& m) {
    py::class_<TimeLineRecord>(m, "TimeLineRecord", "分时线记录")
      .def(py::init<>())
      .def(py::init<const Datetime&, price_t, price_t>())

      .def("__str__", TimeLineRecord_to_string)
      .def("__repr__", TimeLineRecord_to_string)

      .def_readwrite("date", &TimeLineRecord::datetime, "日期时间")
      .def_readwrite("price", &TimeLineRecord::price, "价格")
      .def_readwrite("vol", &TimeLineRecord::vol, "成交数量")

      .def("is_valid", &TimeLineRecord::isValid, "当日期无效时为无效记录")
      .def("__eq__", timelinerecord_eq)

        DEF_PICKLE(TimeLineRecord);
}