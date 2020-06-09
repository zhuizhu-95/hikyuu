/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-31
 *      Author: fasiondog
 */

#include <hikyuu/indicator/Indicator.h>
#include <pybind11/pybind11.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PyIndicatorImp : public IndicatorImp {
public:
    using IndicatorImp::IndicatorImp;

    bool check() override {
        PYBIND11_OVERLOAD(bool, IndicatorImp, check, );
    }

    void _calculate(const Indicator& ind) override {
        PYBIND11_OVERLOAD(void, IndicatorImp, _calculate, ind);
    }

    IndicatorImpPtr _clone() override {
        // 直接使用 pybind11 重载 _clone，在 C++ 中会丢失 python 中的类型
        // 参考：https://github.com/pybind/pybind11/issues/1049 进行修改
        // PYBIND11_OVERLOAD(IndicatorImpPtr, IndicatorImp, _clone, );
        auto self = py::cast(this);
        auto cloned = self.attr("_clone")();

        auto keep_python_state_alive = std::make_shared<py::object>(cloned);
        auto ptr = cloned.cast<PyIndicatorImp*>();

        // aliasing shared_ptr: points to `A_trampoline* ptr` but refcounts the Python object
        return std::shared_ptr<IndicatorImp>(keep_python_state_alive, ptr);
    }

    bool isNeedContext() const override {
        PYBIND11_OVERLOAD(bool, IndicatorImp, isNeedContext, );
    }
};

void export_IndicatorImp(py::module& m) {
    py::class_<IndicatorImp, IndicatorImpPtr, PyIndicatorImp>(m, "IndicatorImp")
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
      .def("check", &IndicatorImp::check)
      .def("_calculate", &IndicatorImp::_calculate)
      .def("is_need_context", &IndicatorImp::isNeedContext);
}
