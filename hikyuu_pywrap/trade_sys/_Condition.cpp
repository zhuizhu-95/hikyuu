/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/condition/build_in.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PyConditionBase : public ConditionBase {
    PY_CLONE(PyConditionBase, ConditionBase)

public:
    using ConditionBase::ConditionBase;

    void _calculate() override {
        PYBIND11_OVERLOAD_PURE(void, ConditionBase, _calculate, );
    }

    void _reset() override {
        PYBIND11_OVERLOAD(void, ConditionBase, _reset, );
    }
};

void export_Condition(py::module& m) {
    py::class_<ConditionBase, ConditionPtr, PyConditionBase>(m, "ConditionBase")
      .def(py::init<>())
      .def(py::init<const string&>())

      .def("__str__", to_py_str<ConditionBase>)
      .def("__repr__", to_py_str<ConditionBase>)

      .def_property("name", py::overload_cast<void>(&ConditionBase::name, py::const_),
                    py::overload_cast<const string&>(&ConditionBase::name))

      .def("getParam", &ConditionBase::getParam<boost::any>)
      .def("setParam", &ConditionBase::setParam<boost::any>)
      .def("haveParam", &ConditionBase::haveParam)
      .def("isValid", &ConditionBase::isValid)
      .def("setTO", &ConditionBase::setTO)
      .def("getTO", &ConditionBase::getTO)
      .def("setTM", &ConditionBase::setTM)
      .def("getTM", &ConditionBase::getTM)
      .def("setSG", &ConditionBase::setSG)
      .def("getSG", &ConditionBase::getSG)
      .def("reset", &ConditionBase::reset)
      .def("clone", &ConditionBase::clone)
      .def("_addValid", &ConditionBase::_addValid)
      .def("_calculate", &ConditionBase::_calculate)
      .def("_reset", &ConditionBase::_reset)

        DEF_PICKLE(ConditionPtr);

    m.def(
      "CN_OPLine", CN_OPLine,
      R"(固定使用股票最小交易量进行交易，计算权益曲线的op值，当权益曲线高于op时，系统有效，否则无效。

    :param Indicator op: Indicator实例)");
}