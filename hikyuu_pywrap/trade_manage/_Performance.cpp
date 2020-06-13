/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_manage/Performance.h>
#include "../convert_Datetime.h"
#include "../pybind_utils.h"

using namespace hku;
namespace py = pybind11;

void export_Performance(py::module& m) {
    py::class_<Performance>(m, "Performance", "简单绩效统计")
      .def(py::init<>())
      .def("reset", &Performance::reset, "复位，清除已计算的结果")
      .def("report", &Performance::report, py::arg("tm"), py::arg("date") = Datetime::now(),
           R"(    简单的文本统计报告，用于直接输出打印
        
    :param TradeManager tm: 指定的交易管理实例
    :param Datetime datetime: 统计截止时刻)")

      .def(
        "statistics", &Performance::statistics, py::arg("tm"), py::arg("date") = Datetime::now(),
        R"(    根据交易记录，统计截至某一时刻的系统绩效, datetime必须大于等于lastDatetime

    :param tm: 指定的交易管理实例
    :param date: 统计截止时刻)")

      .def(
        "get", &Performance::get,
        R"(    按指标名称获取指标值，必须在运行 statistics 或 report 之后生效
        
    :param name: 指标名称)")

      .def("__getitem__", &Performance::get,
           "同 get 方法。按指标名称获取指标值，必须在运行 statistics 或 report 之后生效");
}
