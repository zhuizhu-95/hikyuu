/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/slippage/build_in.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PySlippageBase : public SlippageBase {
    PY_CLONE(PySlippageBase, SlippageBase)

public:
    using SlippageBase::SlippageBase;

    void _calculate() override {
        PYBIND11_OVERLOAD_PURE(void, SlippageBase, _calculate, );
    }

    void _reset() override {
        PYBIND11_OVERLOAD(void, SlippageBase, _reset, );
    }

    price_t getRealBuyPrice(const Datetime& datetime, price_t planPrice) override {
        PYBIND11_OVERLOAD_PURE(price_t, SlippageBase, getRealBuyPrice, datetime, planPrice);
    }

    price_t getRealSellPrice(const Datetime& datetime, price_t planPrice) override {
        PYBIND11_OVERLOAD_PURE(price_t, SlippageBase, getRealSellPrice, datetime, planPrice);
    }
};

void export_Slippage(py::module& m) {
    py::class_<SlippageBase, SPPtr, PySlippageBase>(m, "SlippageBase")
      .def(py::init<>())
      .def(py::init<const string&>(), R"(初始化构造函数
        
    :param str name: 名称)")

      .def("__str__", to_py_str<SlippageBase>)
      .def("__repr__", to_py_str<SlippageBase>)

      .def_property("name", py::overload_cast<void>(&SlippageBase::name, py::const_),
                    py::overload_cast<const string&>(&SlippageBase::name), "名称")
      .def_property("to", &SlippageBase::getTO, &SlippageBase::setTO, "关联交易对象")

      .def("getParam", &SlippageBase::getParam<boost::any>)
      .def("setParam", &SlippageBase::setParam<boost::any>)
      .def("haveParam", &SlippageBase::haveParam)
      .def("getRealBuyPrice", &SlippageBase::getRealBuyPrice)
      .def("getRealSellPrice", &SlippageBase::getRealSellPrice)
      .def("reset", &SlippageBase::reset)
      .def("clone", &SlippageBase::clone)
      .def("_calculate", &SlippageBase::_calculate)
      .def("_reset", &SlippageBase::_reset)

        DEF_PICKLE(SPPtr);

    m.def("SL_FixedPercent", SP_FixedPercent, py::arg("p") = 0.001);
    m.def("SL_FixedValue", SP_FixedValue, py::arg("value") = 0.01);
}