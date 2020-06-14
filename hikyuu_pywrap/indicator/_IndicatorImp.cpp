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
    PY_CLONE(PyIndicatorImp, IndicatorImp)

public:
    using IndicatorImp::IndicatorImp;

    bool check() override {
        PYBIND11_OVERLOAD(bool, IndicatorImp, check, );
    }

    void _calculate(const Indicator& ind) override {
        PYBIND11_OVERLOAD(void, IndicatorImp, _calculate, ind);
    }

    bool isNeedContext() const override {
        PYBIND11_OVERLOAD(bool, IndicatorImp, isNeedContext, );
    }
};

void export_IndicatorImp(py::module& m) {
    py::class_<IndicatorImp, IndicatorImpPtr, PyIndicatorImp>(
      m, "IndicatorImp", R"(指标实现类，定义新指标时，应从此类继承
    
    子类需实现以下接口：

        - _clone() -> IndicatorImp
        - _calculate(ind) ：指标计算
        - isNeedContext(bool) ：是否依赖上下文)")
      .def(py::init<>())

      .def(py::init<const string&>(), R"(
    :param str name: 指标名称)")

      .def(py::init<const string&, size_t>(), R"(
    :param str name: 指标名称
    :param int result_num: 指标结果集数量)")

      .def_property("name", py::overload_cast<void>(&IndicatorImp::name, py::const_),
                    py::overload_cast<const string&>(&IndicatorImp::name), "指标名称")
      .def_property_readonly("discard", &IndicatorImp::discard, "抛弃点数")

      .def("getParameter", &IndicatorImp::getParameter, py::return_value_policy::reference)
      .def("getParam", &IndicatorImp::getParam<boost::any>)
      .def("setParam", &IndicatorImp::setParam<boost::any>)

      .def("setDiscard", &IndicatorImp::setDiscard, "设置需抛弃的数量")

      .def(
        "_set", &IndicatorImp::_set, py::arg("val"), py::arg("pos"), py::arg("num") = 0,
        R"(设置指定位置的指标值。注意，_set 之前，调用 _readyBuffer 准备好数据保存区域！

    :param float val: 指标值
    :param int pos: 索引位置
    :param int num: 所属结果集)")

      .def("_readyBuffer", &IndicatorImp::_readyBuffer, R"(分配数据缓存区域
      
    :param int len: 缓存区长度
    :param int result_num: 结果集数量)")

      .def("getResultNumber", &IndicatorImp::getResultNumber, "获取结果集数量")

      .def("getResultAsPriceList", &IndicatorImp::getResultAsPriceList, "获取指定的结果集")

      .def("check", &IndicatorImp::check, "子类可选接口, 指标参数检查")
      .def("_calculate", &IndicatorImp::_calculate, "子类实现接口，实际指标计算")
      .def("isNeedContext", &IndicatorImp::isNeedContext,
           "子类可选接口，指明该指标是否依赖上下文。默认为不依赖。");
}
