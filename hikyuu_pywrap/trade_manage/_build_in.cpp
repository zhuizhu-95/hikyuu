/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-11
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_manage/build_in.h>
#include "../convert_Datetime.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_trade_manage_buildin(py::module& m) {
    m.def("crtTM", crtTM, py::arg("date") = Datetime(199001010000LL), py::arg("init_cash") = 100000,
          py::arg("cost_func") = TC_Zero(), py::arg("name") = "SYS",
          R"(crtTM([date = datetime(1990, 1, 1), init_cash = 100000, cost_func = TC_Zero(), name =
    "SYS"])

    创建交易管理模块，管理帐户的交易记录及资金使用情况

    :param datetime date:  账户建立日期
    :param float initcash:    初始资金
    :param TradeCost costfunc: 交易成本算法
    :param string name:        账户名称
    :rtype: TradeManager)");

    m.def("TC_TestStub", TC_TestStub);

    m.def(
      "TC_FixedA", TC_FixedA, py::arg("commission") = 0.0018, py::arg("lowest_commission") = 5.0,
      py::arg("stamptax") = 0.001, py::arg("transferfee") = 0.001,
      py::arg("lowest_transferfee") = 1.0,
      R"(TC_FixedA([commission=0.0018, lowest_commission=5.0, stamptax=0.001, transferfee=0.001,
    lowest_transferfee=1.0])

    2015年8月1日之前的A股交易成本算法。上证过户费为交易数量的千分之一，不足1元，按1元计。

    计算规则如下::

    1）上证交易所
        买入：佣金＋过户费
        卖出：佣金＋过户费＋印花税
    2）深证交易所：
        买入：佣金
        卖出：佣金＋印花税

    其中，佣金最低5元

    :param float commission: 佣金比例
    :param float lowest_commission: 最低佣金值
    :param float stamptax: 印花税
    :param float transferfee: 过户费
    :param float lowest_transferfee: 最低过户费
    :return: :py:class:`TradeCostBase` 子类实例)");

    m.def("TC_FixedA2015", TC_FixedA2015, py::arg("commission") = 0.0018,
          py::arg("lowest_commission") = 5.0, py::arg("stamptax") = 0.001,
          py::arg("transferfee") = 0.00002,
          R"(TC_FixedA2015([commission=0.0018, lowest_commission=5.0, stamptax=0.001,
    transferfee=0.00002])

    2015年8月1日及之后的A股交易成本算法，上证过户费改为成交金额的千分之0.02

    计算规则如下::

    1）上证交易所
        买入：佣金＋过户费
        卖出：佣金＋过户费＋印花税
    2）深证交易所：
        买入：佣金
        卖出：佣金＋印花税

    其中，佣金最低5元

    :param float commission: 佣金比例
    :param float lowest_commission: 最低佣金值
    :param float stamptax: 印花税
    :param float transferfee: 过户费
    :return: :py:class:`TradeCostBase` 子类实例)");

    m.def("TC_FixedA2017", TC_FixedA2017, py::arg("commission") = 0.0018,
          py::arg("lowest_commission") = 5.0, py::arg("stamptax") = 0.001,
          py::arg("transferfee") = 0.00002,
          R"(TC_FixedA2017([commission=0.0018, lowest_commission=5.0, stamptax=0.001,
    transferfee=0.00002])

    2017年1月1日及之后的A股交易成本算法，上证过户费改为成交金额的千分之0.02

     1）上证交易所
        买入：佣金＋过户费
        卖出：佣金＋过户费＋印花税
     2）深证交易所：
        买入：佣金
        卖出：佣金＋印花税
     其中：当前佣金比例为千分之1.8（最低5元），印花税为千分之一
          2015年后上证过户费为成交金额的0.00002

    :param float commission: 佣金比例
    :param float lowest_commission: 最低佣金值
    :param float stamptax: 印花税
    :param float transferfee: 过户费
    :return: :py:class:`TradeCostBase` 子类实例)");

    m.def("TC_Zero", TC_Zero, "零成本算法");
}