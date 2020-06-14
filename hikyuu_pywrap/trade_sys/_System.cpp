/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/system/build_in.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_System(py::module& m) {
    py::class_<System, SystemPtr>(
      m, "System",
      R"(系统基类。需要扩展或实现更复杂的系统交易行为，可从此类继承。

    系统是指针对针对单个交易对象的完整策略，包括环境判断、系统有效条件、资金管理、止损、
    止盈、盈利目标、移滑价差的完整策略，用于模拟回测。

    公共参数：

        delay=True (bool) : 是否延迟到下一个bar开盘时进行交易
        delay_use_current_price=True (bool) : 延迟操作的情况下，是使用当前交易时bar
                                            的价格计算新的止损价/止赢价/目标价还是
                                            使用上次计算的结果
        max_delay_count=3 (int) : 连续延迟交易请求的限制次数
        tp_monotonic=True (bool) : 止赢单调递增
        tp_delay_n=3 (int) : 止盈延迟开始的天数，即止盈策略判断从实际交易几天后开始生效
        ignore_sell_sg=False (bool) : 忽略卖出信号，只使用止损/止赢等其他方式卖出
        support_borrow_cash=False (bool) : 在现金不足时，是否支持借入现金，融资
        support_borrow_stock=False (bool) : 在没有持仓时，是否支持借入证券，融券)")

      .def(py::init<const string&>())
      .def(py::init<const TradeManagerPtr&, const MoneyManagerPtr&, const EnvironmentPtr&,
                    const ConditionPtr&, const SignalPtr&, const StoplossPtr&, const StoplossPtr&,
                    const ProfitGoalPtr&, const SlippagePtr&, const string&>())

      .def_property("name", py::overload_cast<void>(&System::name, py::const_),
                    py::overload_cast<const string&>(&System::name), "名称")
      .def_property("tm", &System::getTM, &System::setTM, "交易管理实例")
      .def_property("mm", &System::getMM, &System::setMM, "资金管理策略")
      .def_property("ev", &System::getEV, &System::setEV, "市场环境判断策略")
      .def_property("cn", &System::getCN, &System::setCN, "系统有效条件")
      .def_property("sg", &System::getSG, &System::setSG, "信号指示器")
      .def_property("st", &System::getST, &System::setST, "止损策略")
      .def_property("tp", &System::getTP, &System::setTP, "止盈策略")
      .def_property("pg", &System::getPG, &System::setPG, "盈利目标策略")
      .def_property("sp", &System::getSP, &System::setSP, "移滑价差算法")

      .def_property("to", &System::getTO, &System::setTO, "交易对象, KData")
      .def_property_readonly("stock", &System::getStock, "获取关联的证券")

      .def("__str__", to_py_str<System>)
      .def("__repr__", to_py_str<System>)

      .def("getParam", &System::getParam<boost::any>, R"(获取指定的参数
    
    :param str name: 参数名称
    :return: 参数值
    :raises out_of_range: 无此参数)")

      .def("setParam", &System::setParam<boost::any>, R"(设置参数
        
    :param str name: 参数名称
    :param value: 参数值
    :type value: int | bool | float | string
    :raises logic_error: Unsupported type! 不支持的参数类型)")

      .def("haveParam", &System::haveParam, "是否存在指定参数")

      .def(
        "getTradeRecordList",
        [](const System& sys) { return vector_to_python_list(sys.getTradeRecordList()); },
        "获取交易记录")

      .def("getBuyTradeRequest", &System::getBuyTradeRequest, py::return_value_policy::copy,
           "获取买入请求，“delay”模式下查看下一时刻是否存在买入操作")
      .def("getSellTradeRequest", &System::getSellTradeRequest, py::return_value_policy::reference,
           "获取卖出请求，“delay”模式下查看下一时刻是否存在卖出操作")
      //.def("getSellShortTradeRequest", &System::getSellShortTradeRequest,
      //     py::return_value_policy::reference)
      //.def("getBuyShortTradeRequest", &System::getBuyShortTradeRequest,
      //     py::return_value_policy::reference)

      .def(
        "reset", &System::reset,
        R"(复位操作。TM、EV是和具体系统无关的策略组件，可以在不同的系统中进行共享，
    复位将引起系统运行时被重新清空并计算。尤其是在共享TM时需要注意！
        
    :param bool with_tm: 是否复位TM组件
    :param bool with_ev: 是否复位EV组件)")

      .def("clone", &System::clone, R"(克隆操作。
    
    TM、EV是和具体系统无关的策略组件，可以在不同的系统中进行共享。clone将生成新的
    独立实例，此时非共享状态。尤其需要注意TM是否共享的情况！
        
    :param bool with_tm: 是clone还是共享
    :param bool with_ev: 是clone还是共享)")

      .def("run", py::overload_cast<const KQuery&, bool>(&System::run), py::arg("query"),
           py::arg("reset") = true)
      .def("run", py::overload_cast<const Stock&, const KQuery&, bool>(&System::run),
           py::arg("stock"), py::arg("query"), py::arg("reset") = true,
           R"(运行系统，执行回测
        
    :param Stock stock: 交易的证券
    :param Query query: K线数据查询条件
    :param bool reset: 是否同时复位所有组件，尤其是tm实例)")

        DEF_PICKLE(System);

    m.def("SYS_Simple", SYS_Simple, py::arg("tm") = TradeManagerPtr(),
          py::arg("mm") = MoneyManagerPtr(), py::arg("ev") = EnvironmentPtr(),
          py::arg("cn") = ConditionPtr(), py::arg("sg") = SignalPtr(),
          py::arg("sl") = StoplossPtr(), py::arg("tp") = StoplossPtr(),
          py::arg("pg") = ProfitGoalPtr(), py::arg("sp") = SlippagePtr());
}