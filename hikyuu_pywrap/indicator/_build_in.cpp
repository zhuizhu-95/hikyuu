/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-31
 *      Author: fasiondog
 */

#include <hikyuu/indicator/build_in.h>
#include <pybind11/pybind11.h>
#include "../StockType.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_Indicator_build_in(py::module& m) {
    m.def("ABS", py::overload_cast<void>(ABS));
    m.def("ABS", py::overload_cast<price_t>(ABS));
    m.def("ABS", py::overload_cast<const Indicator&>(ABS), "求绝对值");

    m.def("ACOS", py::overload_cast<void>(ACOS));
    m.def("ACOS", py::overload_cast<price_t>(ACOS));
    m.def("ACOS", py::overload_cast<const Indicator&>(ACOS), "求余弦值");

    m.def("AD", py::overload_cast<void>(AD));
    m.def("AD", py::overload_cast<const KData&>(AD), R"(累积/派发线

:param KData k: 关联的 KData 数据)");

    m.def(
      "ADVANCE", ADVANCE, py::arg("query") = KQueryByIndex(-100), py::arg("market") = "SH",
      py::arg("stk_type") = STOCKTYPE_A, py::arg("ignore_context") = false,
      R"(上涨家数。当存在指定上下文且 ignore_context 为 false 时，将忽略 query, market, stk_type 参数。

:param query: 查询条件
:param market: 所属市场，等于 "" 时，获取所有市场
:param stk_type: 证券类型, 大于 constant.STOCKTYPE_TMP 时，获取所有类型证券
:param ignore_context: 是否忽略上下文。忽略时，强制使用 query, market, stk_type 参数。)");
}