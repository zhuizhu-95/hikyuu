/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/selector/build_in.h>
#include "../convert_any.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PySelectorBase : public SelectorBase {
    PY_CLONE(PySelectorBase, SelectorBase)

public:
    using SelectorBase::SelectorBase;

    void _reset() override {
        PYBIND11_OVERLOAD(void, SelectorBase, _reset, );
    }

    SystemList getSelectedSystemList(Datetime date) override {
        PYBIND11_OVERLOAD_PURE(SystemList, SelectorBase, getSelectedSystemList, date);
    }
};

void export_Selector(py::module& m) {
    py::class_<SelectorBase, SEPtr, PySelectorBase>(
      m, "SelectorBase",
      R"(选择器策略基类，实现标的、系统策略的评估和选取算法

    公共参数：

        freq (int | 1)  变化频度，选股的变化周期，以Bar为单位

    自定义选择器策略接口：

        SelectorBase.getSelectedSystemList - 【必须】获取指定时刻选择的系统实例列表
        SelectorBase._reset - 【可选】重置私有属性
        SelectorBase._clone - 【必须】克隆接口)")

      .def(py::init<>())
      .def(py::init<const string&>(), R"(初始化构造函数
        
    :param str name: 名称)")

      .def("__str__", to_py_str<SelectorBase>)
      .def("__repr__", to_py_str<SelectorBase>)

      .def_property("name", py::overload_cast<void>(&SelectorBase::name, py::const_),
                    py::overload_cast<const string&>(&SelectorBase::name), "名称")

      .def("getParam", &SelectorBase::getParam<boost::any>, "获取指定的参数")
      .def("setParam", &SelectorBase::setParam<boost::any>, "设置参数")
      .def("haveParam", &SelectorBase::haveParam, "参数是否存在")

      .def("reset", &SelectorBase::reset, "复位操作")
      .def("clone", &SelectorBase::clone, "克隆操作")
      .def("_reset", &SelectorBase::_reset, R"(【重载接口】子类复位接口，复位内部私有变量)")

      .def("getSelectedSystemList", &SelectorBase::getSelectedSystemList,
           R"(【重载接口】获取指定时刻选取的标的及其权重
        
    :param Datetime datetime: 指定时刻
    :return: 选取的系统交易实例列表
    :rtype: SystemList)")

      .def("addStock", &SelectorBase::addStock,
           R"(加入初始标的及其对应的系统策略原型
        
    :param Stock stock: 加入的初始标的
    :param System sys: 系统策略原型)")

      .def("addStockList", &SelectorBase::addStockList,
           R"(加入初始标的列表及其系统策略原型
        
    :param StockList stk_list: 加入的初始标的列表
    :param System sys: 系统策略原型)")

      .def("clear", &SelectorBase::clear, "清除已加入的所有交易系统实例")

        DEF_PICKLE(SEPtr);

    m.def("SE_Fixed", py::overload_cast<void>(SE_Fixed));
    m.def("SE_Fixed", py::overload_cast<const StockList&, const SystemPtr&>(SE_Fixed),
          R"(固定选择器，即始终选择初始划定的标的及其系统策略原型
    
    :param StockList stocklist: 初始划定的标的
    :param System sys: 系统策略原型
    :return: SE选择器实例)");
}