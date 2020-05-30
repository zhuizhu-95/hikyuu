/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-30
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/utilities/util.h>

using namespace hku;
namespace py = pybind11;

void export_utils(py::module& m) {
    m.def("roundEx", roundEx, py::arg("number"), py::arg("ndigits") = 0,
          R"(四舍五入，ROUND_HALF_EVEN 银行家舍入法)

:param float number: 待处理数据
:param int ndigits: 保留小数位数
:return: 处理过的数据)");

    m.def("roundUp", roundUp, py::arg("number"), py::arg("ndigits") = 0,
          R"(向上截取，如10.1截取后为11

:param float number: 待处理数据
:param int ndigits: 保留小数位数
:return: 处理过的数据)");

    m.def("roundDown", roundDown, py::arg("number"), py::arg("ndigits") = 0,
          R"(向下截取，如10.1截取后为10

:param float number: 待处理数据
:param int ndigits: 保留小数位数
:return: 处理过的数据)");
}