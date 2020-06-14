/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/stoploss/build_in.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PyStoplossBase : public StoplossBase {
    PY_CLONE(PyStoplossBase, StoplossBase)

public:
    using StoplossBase::StoplossBase;

    void _calculate() override {
        PYBIND11_OVERLOAD_PURE(void, StoplossBase, _calculate, );
    }

    void _reset() override {
        PYBIND11_OVERLOAD(void, StoplossBase, _reset, );
    }

    price_t getPrice(const Datetime& datetime, price_t price) override {
        PYBIND11_OVERLOAD_PURE(price_t, StoplossBase, getPrice, datetime, price);
    }

    price_t getShortPrice(const Datetime& datetime, price_t price) override {
        PYBIND11_OVERLOAD(price_t, StoplossBase, getShortPrice, datetime, price);
    }
};

void export_Stoploss(py::module& m) {
    py::class_<StoplossBase, StoplossPtr, PyStoplossBase>(m, "StoplossBase",
                                                          R"(止损/止赢算法基类

    自定义止损/止赢策略接口：

        SignalBase._calculate() - 【必须】子类计算接口
        SignalBase._clone() - 【必须】克隆接口
        SignalBase._reset() - 【可选】重载私有变量)")

      .def(py::init<>())
      .def(py::init<const string&>(), R"(初始化构造函数
        
    :param str name: 名称)")

      .def("__str__", to_py_str<StoplossBase>)
      .def("__repr__", to_py_str<StoplossBase>)

      .def_property("name", py::overload_cast<void>(&StoplossBase::name, py::const_),
                    py::overload_cast<const string&>(&StoplossBase::name), "名称")
      .def_property("tm", &StoplossBase::getTM, &StoplossBase::setTM, "关联交易管理实例")
      .def_property("to", &StoplossBase::getTO, &StoplossBase::setTO, "关联交易对象")

      .def("getTO", &StoplossBase::getTO, "获取交易对象")
      .def("setTO", &StoplossBase::setTO, "设置交易对象")

      .def("getParam", &StoplossBase::getParam<boost::any>, R"(获取指定的参数
    
    :param str name: 参数名称
    :return: 参数值
    :raises out_of_range: 无此参数)")

      .def("setParam", &StoplossBase::setParam<boost::any>, R"(设置参数
        
    :param str name: 参数名称
    :param value: 参数值
    :type value: int | bool | float | string
    :raises logic_error: Unsupported type! 不支持的参数类型)")

      .def("haveParam", &StoplossBase::haveParam, "是否存在指定参数")

      .def(
        "getPrice", &StoplossBase::getPrice,
        R"(【重载接口】获取本次预期交易（买入）时的计划止损价格，如果不存在止损价，则返回0。
    用于系统在交易执行前向止损策略模块查询本次交易的计划止损价。
        
    .. note::
        一般情况下，止损/止赢的算法可以互换，但止损的getPrice可以传入计划交易的
        价格，比如以买入价格的30%做为止损。而止赢则不考虑传入的price参数，即认为
        price 为 0.0。实际上，即使止损也不建议使用price参数，如可以使用前日最低
        价的 30% 作为止损，则不需要考虑price参数。
        
    :param Datetime datetime: 交易时间
    :param float price: 计划买入的价格
    :return: 止损价格
    :rtype: float)")

      .def(
        "getShortPrice", &StoplossBase::getShortPrice,
        R"(获取本次预期交易（卖空）时的计划止损价格，如果不存在止损价，则返回0。

    用于系统在交易执行前向止损策略模块查询本次交易的计划止损价。
    note: 默认实现时，和getPrice返回结果相同
    
    :param datetime 交易日期
    :param price 计划交易的价格)")

      .def("reset", &StoplossBase::reset, "复位操作")
      .def("clone", &StoplossBase::clone, "克隆操作")
      .def("_calculate", &StoplossBase::_calculate, "【重载接口】子类计算接口")
      .def("_reset", &StoplossBase::_reset, "【重载接口】子类复位接口，复位内部私有变量")

        DEF_PICKLE(StoplossPtr);

    m.def(
      "ST_FixedPercent", ST_FixedPercent, py::arg("p") = 0.03,
      R"(固定百分比止损策略，即当价格低于买入价格的某一百分比时止损
    
    :param float p: 百分比(0,1]
    :return: 止损/止赢策略实例)");

    m.def("ST_Indicator", ST_Indicator, py::arg("op"), py::arg("kpart") = "CLOSE",
          R"(使用技术指标作为止损价。如使用10日EMA作为止损：::
    
        ST_Indicator(EMA(n=10))

    :param Indicator op:
    :param string kpart: KDATA|OPEN|HIGH|LOW|CLOSE|AMO|VOL
    :return: 止损/止赢策略实例)");

    m.def(
      "ST_Saftyloss", ST_Saftyloss, py::arg("n1") = 10, py::arg("n2") = 3, py::arg("p") = 2.0,
      R"(参见《走进我的交易室》（2007年 地震出版社） 亚历山大.艾尔德(Alexander Elder) P202
    计算说明：在回溯周期内（一般为10到20天），将所有向下穿越的长度相加除以向下穿越的次数，
    得到噪音均值（即回溯期内所有最低价低于前一日最低价的长度除以次数），并用今日
    最低价减去（前日噪音均值乘以一个倍数）得到该止损线。为了抵消波动并且保证止损线的
    上移，在上述结果的基础上再取起N日（一般为3天）内的最高值

    :param int n1: 计算平均噪音的回溯时间窗口，默认为10天
    :param int n2: 对初步止损线去n2日内的最高值，默认为3
    :param double p: 噪音系数，默认为2
    :return: 止损/止赢策略实例)");
}