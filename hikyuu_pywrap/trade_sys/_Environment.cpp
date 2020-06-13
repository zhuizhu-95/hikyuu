/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/environment/build_in.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PyEnvironmentBase : public EnvironmentBase {
    PY_CLONE(PyEnvironmentBase, EnvironmentBase)

public:
    using EnvironmentBase::EnvironmentBase;

    void _calculate() override {
        PYBIND11_OVERLOAD_PURE(void, EnvironmentBase, _calculate, );
    }

    void _reset() override {
        PYBIND11_OVERLOAD(void, EnvironmentBase, _reset, );
    }
};

void export_Environment(py::module& m) {
    py::class_<EnvironmentBase, EnvironmentPtr, PyEnvironmentBase>(
      m, "EnvironmentBase", R"(市场环境判定策略基类

    自定义市场环境判定策略接口：

        EnvironmentBase._calculate() - 【必须】子类计算接口
        EnvironmentBase._clone() - 【必须】克隆接口
        EnvironmentBase._reset() - 【可选】重载私有变量)")

      .def(py::init<>())
      .def(py::init<const string&>(), R"(初始化构造函数
        
    :param str name: 名称)")

      .def("__str__", to_py_str<EnvironmentBase>)
      .def("__repr__", to_py_str<EnvironmentBase>)

      .def_property("name", py::overload_cast<void>(&EnvironmentBase::name, py::const_),
                    py::overload_cast<const string&>(&EnvironmentBase::name), "名称")

      .def("getParam", &EnvironmentBase::getParam<boost::any>, R"(获取指定的参数
    
    :param str name: 参数名称
    :return: 参数值
    :raises out_of_range: 无此参数)")

      .def("setParam", &EnvironmentBase::setParam<boost::any>, R"(设置参数
        
    :param str name: 参数名称
    :param value: 参数值
    :type value: int | bool | float | string
    :raises logic_error: Unsupported type! 不支持的参数类型)")

      .def("haveParam", &EnvironmentBase::haveParam, "是否存在指定参数")

      .def("setQuery", &EnvironmentBase::setQuery, R"(设置查询条件
    
    :param Query query:  查询条件)")

      .def("getQuery", &EnvironmentBase::getQuery, "获取查询条件")
      .def("isValid", &EnvironmentBase::isValid, R"(指定时间系统是否有效
        
    :param Datetime datetime: 指定时间
    :return: True 有效 | False 无效)")

      .def("_addValid", &EnvironmentBase::_addValid, R"(加入有效时间，在_calculate中调用
        
    :param datetime: 有效时间)")

      .def("reset", &EnvironmentBase::reset, "复位操作")
      .def("clone", &EnvironmentBase::clone, "克隆操作")
      .def("_reset", &EnvironmentBase::_reset, "【重载接口】子类复位接口，用于复位内部私有变量")
      .def("_calculate", &EnvironmentBase::_calculate, "【重载接口】子类计算接口")

        DEF_PICKLE(EnvironmentPtr);

    m.def(
      "EV_TwoLine", EV_TwoLine, py::arg("fast"), py::arg("slow"), py::arg("market") = "SH",
      R"(快慢线判断策略，市场指数的快线大于慢线时，市场有效，否则无效。

    :param Indicator fast: 快线指标
    :param Indicator slow: 慢线指标
    :param str market: 市场名称)");
}