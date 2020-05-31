/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-31
 *      Author: fasiondog
 */

#include <hikyuu/indicator/Indicator.h>
#include <pybind11/pybind11.h>
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PyIndicatorImp : public IndicatorImp {
public:
    using IndicatorImp::IndicatorImp;
    IndicatorImpPtr operator()(const Indicator& ind) override {
        PYBIND11_OVERLOAD(IndicatorImpPtr, IndicatorImp, operator(), ind);
    }

    bool check() override {
        PYBIND11_OVERLOAD(bool, IndicatorImp, check, );
    }

    void _calculate(const Indicator& ind) override {
        PYBIND11_OVERLOAD(void, IndicatorImp, _calculate, ind);
    }

    IndicatorImpPtr _clone() override {
        PYBIND11_OVERLOAD(IndicatorImpPtr, IndicatorImp, _clone, );
    }

    bool isNeedContext() const override {
        PYBIND11_OVERLOAD(bool, IndicatorImp, isNeedContext, );
    }
};

void export_IndicatorImp(py::module& m) {
    py::class_<IndicatorImp, PyIndicatorImp, IndicatorImpPtr>(m, "IndicatorImp")
      .def(py::init<>())
      .def(py::init<const string&>())
      .def(py::init<const string&, size_t>())
      .def_property("name", py::overload_cast<void>(&IndicatorImp::name, py::const_),
                    py::overload_cast<const string&>(&IndicatorImp::name))
      .def_property_readonly("discard", &IndicatorImp::discard)

      // TODO
      //.def("getParameter", &IndicatorImp::getParameter,
      // return_value_policy<copy_const_reference>())

      .def("get_param", &IndicatorImp::getParam<boost::any>)
      .def("set_param", &IndicatorImp::setParam<boost::any>)
      .def("set_discard", &IndicatorImp::setDiscard)
      .def("_set", &IndicatorImp::_set, py::arg("val"), py::arg("pos"), py::arg("num") = 0)
      .def("_ready_buffer", &IndicatorImp::_readyBuffer)
      .def("get_result_number", &IndicatorImp::getResultNumber)
      .def("get_result_as_price_list", &IndicatorImp::getResultAsPriceList)
      .def("calculate", &IndicatorImp::calculate)
      .def("check", &IndicatorImp::check)
      .def("clone", &IndicatorImp::clone)
      .def("_calculate", &IndicatorImp::_calculate)
      .def("__call__", &IndicatorImp::operator())
      .def("_clone", &IndicatorImp::_clone)
      .def("isNeedContext", &IndicatorImp::isNeedContext);
}
