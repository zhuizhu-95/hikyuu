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
    py::class_<ConditionBase, ConditionPtr, PyConditionBase>(m, "ConditionBase",
                                                             R"(系统有效条件基类

    自定义系统有效条件接口：

        ConditionBase._calculate() - 【必须】子类计算接口
        ConditionBase._clone() - 【必须】克隆接口
        ConditionBase._reset() - 【可选】重载私有变量)")

      .def(py::init<>())
      .def(py::init<const string&>(), R"(初始化构造函数
        
    :param str name: 名称)")

      .def("__str__", to_py_str<ConditionBase>)
      .def("__repr__", to_py_str<ConditionBase>)

      .def_property("name", py::overload_cast<void>(&ConditionBase::name, py::const_),
                    py::overload_cast<const string&>(&ConditionBase::name), "名称")
      .def_property("to", &ConditionBase::getTO, &ConditionBase::setTO, "关联交易对象")
      .def_property("tm", &ConditionBase::getTM, &ConditionBase::setTM, "关联交易管理账户")
      .def_property("sg", &ConditionBase::getSG, &ConditionBase::setSG, "关联交易信号指示器")

      .def("getParam", &ConditionBase::getParam<boost::any>, R"(获取指定的参数
    
    :param str name: 参数名称
    :return: 参数值
    :raises out_of_range: 无此参数)")

      .def("setParam", &ConditionBase::setParam<boost::any>, R"(设置参数
        
    :param str name: 参数名称
    :param value: 参数值
    :type value: int | bool | float | string
    :raises logic_error: Unsupported type! 不支持的参数类型)")

      .def("haveParam", &ConditionBase::haveParam, "指定参数是否存在")

      .def("isValid", &ConditionBase::isValid, R"(指定时间系统是否有效
        
    :param Datetime datetime: 指定时间
    :return: True 有效 | False 无效)")

      .def(
        "reset", &ConditionBase::reset,
        R"(复位操作。TM、EV、MM、SP都是和具体系统无关的策略组件，可以在不同的系统中进行共享，
    复位将引起系统运行时被重新清空并计算。尤其是在共享TM时需要注意！
        
    :param bool with_tm: 是否复位TM组件
    :param bool with_ev: 是否复位EV组件
    :param bool with_mm: 是否复位MM组件
    :param bool with_sp: 是否复位SP组件)")

      .def(
        "clone", &ConditionBase::clone,
        R"(克隆操作。TM、EV、MM、SP都是和具体系统无关的策略组件，可以在不同的系统中进行共享。
    clone将生成新的独立实例，此时非共享状态。尤其需要注意TM是否共享的情况！
        
    :param bool with_tm: 是clone还是共享
    :param bool with_ev: 是clone还是共享
    :param bool with_mm: 是clone还是共享
    :param bool with_sp: 是clone还是共享)")

      .def("_addValid", &ConditionBase::_addValid, R"( 加入有效时间，在_calculate中调用
        
    :param datetime: 有效时间)")

      .def("_calculate", &ConditionBase::_calculate, "【重载接口】子类计算接口")
      .def("_reset", &ConditionBase::_reset, "【重载接口】子类复位接口，复位内部私有变量")

        DEF_PICKLE(ConditionPtr);

    m.def(
      "CN_OPLine", CN_OPLine,
      R"(固定使用股票最小交易量进行交易，计算权益曲线的op值，当权益曲线高于op时，系统有效，否则无效。

    :param Indicator op: Indicator实例)");
}