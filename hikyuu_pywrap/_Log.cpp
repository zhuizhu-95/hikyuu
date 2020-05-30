/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-30
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/Log.h>

using namespace hku;
namespace py = pybind11;

void export_Log(py::module& m) {
    py::enum_<LOG_LEVEL>(m, "LOG_LEVEL")
      .value("TRACE", LOG_LEVEL::TRACE, "调试")
      .value("DEBUG", LOG_LEVEL::DEBUG, "调试")
      .value("INFO", LOG_LEVEL::INFO, "一般信息")
      .value("WARN", LOG_LEVEL::WARN, "告警")
      .value("ERROR", LOG_LEVEL::ERROR, "错误")
      .value("FATAL", LOG_LEVEL::FATAL, "致命")
      .value("OFF", LOG_LEVEL::OFF, "关闭日志打印")
      .export_values();

    m.def("get_log_level", get_log_level, "获取当前日志级别");
    m.def("set_log_level", set_log_level, "设置日志级别");
}
