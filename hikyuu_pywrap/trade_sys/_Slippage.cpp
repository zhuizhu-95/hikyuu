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
    py::class_<SlippageBase, SPPtr, PySlippageBase>(m, "SlippageBase", R"(移滑价差算法基类

    自定义移滑价差接口：

        SlippageBase.getRealBuyPrice() - 【必须】计算实际买入价格
        SlippageBase.getRealSellPrice() - 【必须】计算实际卖出价格
        SlippageBase._calculate() - 【必须】子类计算接口
        SlippageBase._clone() - 【必须】克隆接口
        SlippageBase._reset() - 【可选】重载私有变量)")

      .def(py::init<>())
      .def(py::init<const string&>(), R"(初始化构造函数
        
    :param str name: 名称)")

      .def("__str__", to_py_str<SlippageBase>)
      .def("__repr__", to_py_str<SlippageBase>)

      .def_property("name", py::overload_cast<void>(&SlippageBase::name, py::const_),
                    py::overload_cast<const string&>(&SlippageBase::name), "名称")
      .def_property("to", &SlippageBase::getTO, &SlippageBase::setTO, "关联交易对象")

      .def("getTO", &SlippageBase::getTO, "获取交易对象")
      .def("setTO", &SlippageBase::setTO, "设置交易对象")

      .def("getParam", &SlippageBase::getParam<boost::any>, R"(获取指定的参数
    
    :param str name: 参数名称
    :return: 参数值
    :raises out_of_range: 无此参数)")

      .def("setParam", &SlippageBase::setParam<boost::any>, R"(设置参数
        
    :param str name: 参数名称
    :param value: 参数值
    :type value: int | bool | float | string
    :raises logic_error: Unsupported type! 不支持的参数类型)")

      .def("haveParam", &SlippageBase::haveParam, "参数是否存在")

      .def("getRealBuyPrice", &SlippageBase::getRealBuyPrice,
           R"(【重载接口】计算实际买入价格
        
    :param Datetime datetime: 买入时间
    :param float price: 计划买入价格
    :return: 实际买入价格
    :rtype: float)")

      .def("getRealSellPrice", &SlippageBase::getRealSellPrice,
           R"(【重载接口】计算实际卖出价格
        
    :param Datetime datetime: 卖出时间
    :param float price: 计划卖出价格
    :return: 实际卖出价格
    :rtype: float)")

      .def("reset", &SlippageBase::reset, "复位操作")
      .def("clone", &SlippageBase::clone, "克隆操作")
      .def("_calculate", &SlippageBase::_calculate, "【重载接口】子类计算接口")
      .def("_reset", &SlippageBase::_reset, "【重载接口】子类复位接口，复位内部私有变量")

        DEF_PICKLE(SPPtr);

    m.def(
      "SP_FixedPercent", SP_FixedPercent, py::arg("p") = 0.001,
      R"(固定百分比移滑价差算法，买入实际价格 = 计划买入价格 * (1 + p)，卖出实际价格 = 计划卖出价格 * (1 - p)
    
    :param float p: 偏移的固定百分比
    :return: 移滑价差算法实例)");

    m.def(
      "SP_FixedValue", SP_FixedValue, py::arg("value") = 0.01,
      R"(固定价格移滑价差算法，买入实际价格 = 计划买入价格 + 偏移价格，卖出实际价格 = 计划卖出价格 - 偏移价格
    
    :param float p: 偏移价格
    :return: 移滑价差算法实例)");
}