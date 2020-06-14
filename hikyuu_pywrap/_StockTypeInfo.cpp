/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-27
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include "pickle_support.h"
#include <hikyuu/serialization/StockTypeInfo_serialization.h>
#include "StockType.h"

using namespace hku;
namespace py = pybind11;

void export_StockTypeInfo(py::module& m) {
    py::class_<StockTypeInfo>(m, "StockTypeInfo", "证券类型信息")
      .def(py::init<>())
      .def(py::init<uint32, const string&, price_t, price_t, int, double, double>())
      .def("__str__", &StockTypeInfo::toString)
      .def("__repr__", &StockTypeInfo::toString)

      .def_property_readonly(
        "type", [](const StockTypeInfo& info) { return StockType(info.type()); }, "证券类型")
      .def_property_readonly("description", &StockTypeInfo::description,
                             py::return_value_policy::copy, "证券类型描述信息")
      .def_property_readonly("tick", &StockTypeInfo::tick, "最小跳动量")
      .def_property_readonly("tickValue", &StockTypeInfo::tickValue, "每tick价格")
      .def_property_readonly("unit", &StockTypeInfo::unit, "每单位价格 = tickValue / tick")
      .def_property_readonly("precision", &StockTypeInfo::precision, "价格精度")
      .def_property_readonly("minTradeNumber", &StockTypeInfo::minTradeNumber, "每笔最小交易数量")
      .def_property_readonly("maxTradeNumber", &StockTypeInfo::maxTradeNumber, "每笔最大交易数量")

        DEF_PICKLE(StockTypeInfo);
}