/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/profitgoal/build_in.h>
#include "../convert_any.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PyProfitGoalBase : public ProfitGoalBase {
    PY_CLONE(PyProfitGoalBase, ProfitGoalBase)

public:
    using ProfitGoalBase::ProfitGoalBase;

    void buyNotify(const TradeRecord& tr) override {
        PYBIND11_OVERLOAD(void, ProfitGoalBase, buyNotify, tr);
    }

    void sellNotify(const TradeRecord& tr) override {
        PYBIND11_OVERLOAD(void, ProfitGoalBase, sellNotify, tr);
    }

    price_t getGoal(const Datetime& datetime, price_t price) override {
        PYBIND11_OVERLOAD_PURE(price_t, ProfitGoalBase, getGoal, datetime, price);
    }

    price_t getShortGoal(const Datetime& date, price_t price) override {
        PYBIND11_OVERLOAD(price_t, ProfitGoalBase, getShortGoal, date, price);
    }

    void _reset() override {
        PYBIND11_OVERLOAD(void, ProfitGoalBase, _reset, );
    }

    void _calculate() override {
        PYBIND11_OVERLOAD_PURE(void, ProfitGoalBase, _calculate, );
    };
};

void export_Profitgoal(py::module& m) {
    py::class_<ProfitGoalBase, PGPtr, PyProfitGoalBase>(m, "ProfitGoalBase",
                                                        R"(盈利目标策略基类

    自定义盈利目标策略接口：

        ProfitGoalBase.getGoal() - 【必须】获取目标价格
        ProfitGoalBase._calculate() - 【必须】子类计算接口
        ProfitGoalBase._clone() - 【必须】克隆接口
        ProfitGoalBase._reset() - 【可选】重载私有变量)")

      .def(py::init<>())
      .def(py::init<const string&>(), R"(初始化构造函数
        
    :param str name: 名称)")

      .def("__str__", to_py_str<ProfitGoalBase>)
      .def("__repr__", to_py_str<ProfitGoalBase>)

      .def_property("name", py::overload_cast<void>(&ProfitGoalBase::name, py::const_),
                    py::overload_cast<const string&>(&ProfitGoalBase::name), "名称")
      .def_property("tm", &ProfitGoalBase::getTM, &ProfitGoalBase::setTM, "关联交易管理账户")
      .def_property("to", &ProfitGoalBase::getTO, &ProfitGoalBase::setTO, "关联交易对象")

      .def("getTO", &ProfitGoalBase::getTO, "获取交易对象")
      .def("setTO", &ProfitGoalBase::setTO, "设置交易对象")

      .def("getParam", &ProfitGoalBase::getParam<boost::any>, "获取指定的参数")
      .def("setParam", &ProfitGoalBase::setParam<boost::any>, "设置参数")
      .def("haveParam", &ProfitGoalBase::haveParam, "是否存在指定参数")

      .def(
        "buyNotify", &ProfitGoalBase::buyNotify,
        R"(【重载接口】交易系统发生实际买入操作时，通知交易变化情况，一般存在多次增减仓的情况才需要重载
        
    :param TradeRecord trade_record: 发生实际买入时的实际买入交易记录)")

      .def(
        "sellNotify", &ProfitGoalBase::sellNotify,
        R"(【重载接口】交易系统发生实际卖出操作时，通知实际交易变化情况，一般存在多次增减仓的情况才需要重载
        
    :param TradeRecord trade_record: 发生实际卖出时的实际卖出交易记录)")

      .def(
        "getGoal", &ProfitGoalBase::getGoal,
        R"(【重载接口】获取盈利目标价格，返回constant.null_price时，表示未限定目标；
    返回0意味着需要卖出。
        
    :param Datetime datetime: 买入时间
    :param float price: 买入价格
    :return: 目标价格
    :rtype: float)")

      .def("getShortGoal", &ProfitGoalBase::getShortGoal)

      .def("reset", &ProfitGoalBase::reset, "复位操作")
      .def("clone", &ProfitGoalBase::clone, "克隆操作")
      .def("_calculate", &ProfitGoalBase::_calculate, "【重载接口】子类计算接口")
      .def("_reset", &ProfitGoalBase::_reset, "【重载接口】子类复位接口，复位内部私有变量")

        DEF_PICKLE(PGPtr);

    m.def("PG_NoGoal", PG_NoGoal, R"(无盈利目标策略，通常为了进行测试或对比。)");

    m.def("PG_FixedPercent", PG_FixedPercent, py::arg("p") = 0.2,
          R"(固定百分比盈利目标，目标价格 = 买入价格 * (1 + p)
    
    :param float p: 百分比
    :return: 盈利目标策略实例)");

    m.def("PG_FixedHoldDays", PG_FixedHoldDays, py::arg("days") = 5,
          R"(固定持仓天数盈利目标策略
    
    :param int days: 允许持仓天数（按交易日算）,默认5天
    :return: 盈利目标策略实例)");
}