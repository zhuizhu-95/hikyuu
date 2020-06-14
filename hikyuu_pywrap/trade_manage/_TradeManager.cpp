/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-11
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_manage/TradeManager.h>
#include "../convert_Datetime.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_TradeManager(py::module& m) {
    py::class_<TradeManager, TradeManagerPtr>(
      m, "TradeManager",
      R"(交易管理类，可理解为一个模拟账户进行模拟交易。一般使用 crtTM 创建交易管理实例。

     公共参数：

          reinvest=False (bool) : 红利是否再投资
          precision=2 (int) : 金额计算精度
          support_borrow_cash=False (bool) : 是否自动融资
          support_borrow_stock=False (bool) : 是否自动融券
          save_action=True (bool) : 是否保存Python命令序列)")

      .def(py::init<const Datetime&, price_t, const TradeCostPtr&, const string&>(),
           py::arg("date") = Datetime(199001010000LL), py::arg("init_cash") = 100000.0,
           py::arg("cost_func") = TC_Zero(), py::arg("name") = "SYS",
           R"(创建交易管理模块，管理帐户的交易记录及资金使用情况

    :param datetime date:  账户建立日期
    :param float init_cash:    初始资金
    :param TradeCost cost_func: 交易成本算法
    :param string name:        账户名称
    :rtype: TradeManager)")

      .def("__str__", &TradeManager::toString)
      .def("__repr__", &TradeManager::toString)

      .def_property("name", py::overload_cast<>(&TradeManager::name, py::const_),
                    py::overload_cast<const string&>(&TradeManager::name), "名称")
      .def_property_readonly("initCash", &TradeManager::initCash, "初始资金")
      .def_property_readonly("currentCash", &TradeManager::currentCash, "返回当前现金")
      .def_property_readonly("initDatetime", &TradeManager::initDatetime, "账户建立日期")
      .def_property_readonly("firstDatetime", &TradeManager::firstDatetime,
                             "第一笔买入交易发生日期，如未发生交易返回 None")
      .def_property_readonly("lastDatetime", &TradeManager::lastDatetime,
                             "最后一笔交易日期，注意和交易类型无关，如未发生交易返回账户建立日期")
      .def_property_readonly("reinvest", &TradeManager::reinvest,
                             R"(红利/股息/送股再投资标志，同公共参数 "reinvest")")
      .def_property_readonly("precision", &TradeManager::precision,
                             R"(价格精度，同公共参数"precision")")
      .def_property("costFunc", py::overload_cast<void>(&TradeManager::costFunc, py::const_),
                    py::overload_cast<const TradeCostPtr&>(&TradeManager::costFunc), "交易成本算法")

      .def_property("getBrokerLastDatetime", &TradeManager::getBrokerLastDatetime,
                    &TradeManager::setBrokerLastDatetime,
                    R"(实际开始订单代理操作的时刻。
        
     默认情况下，TradeManager会在执行买入/卖出操作时，调用订单代理执行代理的买入/卖出动作，
     但这样在实盘操作时会存在问题。因为系统在计算信号指示时，需要回溯历史数据才能得到最新的
     信号，这样TradeManager会在历史时刻就执行买入/卖出操作，此时如果订单代理本身没有对发出
     买入/卖出指令的时刻进行控制，会导致代理发送错误的指令。此时，需要指定在某一个时刻之后，
     才允许指定订单代理的买入/卖出操作。属性 brokeLastDatetime 即用于指定该时刻。)")

      .def("getParam", &TradeManager::getParam<boost::any>, R"(getParam(self, name)

    获取指定的参数
    
    :param str name: 参数名称
    :return: 参数值
    :raises out_of_range: 无此参数)")

      .def("setParam", &TradeManager::setParam<boost::any>, R"(设置参数
        
    :param str name: 参数名称
    :param value: 参数值
    :type value: int | bool | float | string
    :raises logic_error: Unsupported type! 不支持的参数类型)")

      .def("haveParam", &TradeManager::haveParam)

      .def("reset", &TradeManager::reset, "复位，清空交易、持仓记录")
      .def("clone", &TradeManager::clone, "克隆（深复制）实例")

      .def("regBroker", &TradeManager::regBroker, py::keep_alive<1, 2>(),
           R"(注册订单代理。可执行多次该命令注册多个订单代理。
        
    :param OrderBrokerBase broker: 订单代理实例)")

      .def("clearBroker", &TradeManager::clearBroker, "清空所有已注册订单代理")

      //.def("getMarginRate", &TradeManager::getMarginRate)
      .def("have", &TradeManager::have, R"(当前是否持有指定的证券
        
    :param Stock stock: 指定证券
    :rtype: bool)")

      .def("getStockNumber", &TradeManager::getStockNumber,
           "当前持有的证券种类数量，即当前持有几只股票（非各个股票的持仓数）")
      //.def("getShortStockNumber", &TradeManager::getShortStockNumber)
      .def("getHoldNumber", &TradeManager::getHoldNumber,
           R"(获取指定时刻指定证券的持有数量
        
    :param Datetime datetime: 指定时刻
    :param Stock stock: 指定的证券
    :rtype: int)")
      //.def("getShortHoldNumber", &TradeManager::getShortHoldNumber)

      .def(
        "getTradeList",
        [](const TradeManager& tm, const Datetime& start, const Datetime& end) {
            return vector_to_python_list(tm.getTradeList(start, end));
        },
        py::arg("start") = Datetime::min(), py::arg("end") = Datetime(),
        R"(获取交易记录，未指定参数时，获取全部交易记录
        
    :param datetime start: 起始日期
    :param datetime end: 结束日期
    :rtype: list)")

      .def(
        "getPositionList",
        [](const TradeManager& tm) { return vector_to_python_list(tm.getPositionList()); },
        "获取当前全部持仓记录")

      .def(
        "getHistoryPositionList",
        [](const TradeManager& tm) { return vector_to_python_list(tm.getHistoryPositionList()); },
        "获取全部历史持仓记录，即已平仓记录")

      .def(
        "getPosition", &TradeManager::getPosition,
        R"(获取指定证券的当前持仓记录，如当前未持有该票，返回PositionRecord()
        
    :param Stock stock: 指定的证券
    :rtype: PositionRecord)")

      .def("getBuyCost", &TradeManager::getBuyCost, py::arg("date"), py::arg("stock"),
           py::arg("price"), py::arg("num"),
           R"(计算买入成本
        
    :param datetime date: 交易时间
    :param Stock stock:   交易的证券
    :param float price:   买入价格
    :param int num:       买入数量
    :rtype: CostRecord)")

      .def("getSellCost", &TradeManager::getSellCost, py::arg("date"), py::arg("stock"),
           py::arg("price"), py::arg("num"),
           R"(计算卖出成本

    :param datetime date: 交易时间
    :param Stock stock:   交易的证券
    :param float price:   卖出价格
    :param int num:       卖出数量
    :rtype: CostRecord )")

      //.def("getBorrowCashCost", &TradeManager::getBorrowCashCost)
      //.def("getReturnCashCost", &TradeManager::getReturnCashCost)
      //.def("getBorrowStockCost", &TradeManager::getBorrowStockCost)
      //.def("getReturnStockCost", &TradeManager::getReturnStockCost)

      .def(
        "cash", &TradeManager::cash, py::arg("date"), py::arg("ktype") = KQuery::DAY,
        R"(获取指定日期的现金。（注：如果不带日期参数，无法根据权息信息调整持仓。）
        
    :param datetime date: 指定时刻
    :param ktype: K线类型
    :rtype: float)")

      .def("getFunds", py::overload_cast<KQuery::KType>(&TradeManager::getFunds, py::const_),
           py::arg("ktype") = KQuery::DAY, R"(获取账户当前时刻的资产详情
        
    :param KQuery.KType ktype: K线类型
    :rtype: FundsRecord)")

      .def("getFunds", py::overload_cast<const Datetime&, KQuery::KType>(&TradeManager::getFunds),
           py::arg("date"), py::arg("ktype") = KQuery::DAY,
           R"(获取指定时刻的资产市值详情
        
    :param Datetime datetime:  指定时刻
    :param KQuery.KType ktype: K线类型
    :rtype: FundsRecord )")

      .def("getFundsCurve", &TradeManager::getFundsCurve, py::arg("dates"),
           py::arg("ktype") = KQuery::DAY, R"(获取资产净值曲线
        
    :param list dates: 日期列表，根据该日期列表获取其对应的资产净值曲线
    :param Query.KType ktype: K线类型，必须与日期列表匹配
    :return: 资产净值列表
    :rtype: list)")

      .def("getProfitCurve", &TradeManager::getProfitCurve, py::arg("dates"),
           py::arg("ktype") = KQuery::DAY,
           R"(获取收益曲线，即扣除历次存入资金后的资产净值曲线
        
    :param list dates: 日期列表，根据该日期列表获取其对应的收益曲线，应为递增顺序
    :param Query.KType ktype: K线类型，必须与日期列表匹配
    :return: 收益曲线
    :rtype: PriceList)")

      .def("checkin", &TradeManager::checkin, py::arg("date"), py::arg("cash"),
           R"(checkin(self, date, cash)
    
    向账户内存入现金
    
    :param datetime date: 交易时间
    :param float cash: 存入的现金量
    :rtype: TradeRecord)")

      .def("checkout", &TradeManager::checkout, py::arg("date"), py::arg("cash"),
           R"(checkout(self, date, cash)
    
    从账户内取出现金
        
    :param datetime date: 交易时间
    :param float cash: 取出的资金量
    :rtype: TradeRecord)")

      //.def("checkinStock", &TradeManager::checkinStock)
      //.def("checkoutStock", &TradeManager::checkoutStock)
      //.def("borrowCash", &TradeManager::borrowCash)
      //.def("returnCash", &TradeManager::returnCash)
      //.def("borrowStock", &TradeManager::borrowStock)
      //.def("returnStock", &TradeManager::returnStock)

      .def(
        "buy", &TradeManager::buy, py::arg("date"), py::arg("stock"), py::arg("real_price"),
        py::arg("num"), py::arg("stoploss") = 0.0, py::arg("goal_price") = 0.0,
        py::arg("plan_price") = 0.0, py::arg("part") = PART_INVALID,
        R"(buy(self, date, stock, real_price, number[, stoploss=0.0, goal_price=0.0, plan_price=0.0, part=System.INVALID])
    
    买入操作
        
    :param datetime date:    买入时间
    :param Stock stock:      买入的证券
    :param float real_price:  实际买入价格
    :param int num:          买入数量
    :param float stoploss:   止损价
    :param float goal_price:  目标价格
    :param float plan_price:  计划买入价格
    :param SystemPart part:  交易指示来源
    :rtype: TradeRecord)")

      .def(
        "sell", &TradeManager::sell, py::arg("date"), py::arg("stock"), py::arg("real_price"),
        py::arg("number") = MAX_DOUBLE, py::arg("stoploss") = 0.0, py::arg("goal_price") = 0.0,
        py::arg("plan_price") = 0.0, py::arg("part") = PART_INVALID,
        R"(sell(self, date, stock, real_price[, number=MAX_DOUBLE, stoploss=0.0, goalPrice=0.0, planPrice=0.0, part=System.INVALID])
    
    卖出操作
        
    :param datetime date:    卖出时间
    :param Stock stock:      卖出的证券
    :param float realPrice:  实际卖出价格
    :param int num:          卖出数量，如果等于MAX_DOUBLE，表示全部卖出
    :param float stoploss:   新的止损价
    :param float goalPrice:  新的目标价格
    :param float planPrice:  原计划卖出价格
    :param SystemPart part:  交易指示来源
    :rtype: TradeRecord)")

      .def("addTradeRecord", &TradeManager::addTradeRecord, R"(addTradeRecord(self, tr)

    直接加入交易记录，如果加入初始化账户记录，将清除全部已有交易及持仓记录。

    :param TradeRecord tr: 交易记录
    :return: True（成功） | False（失败）
    :rtype: bool)")

      .def("tocsv", &TradeManager::tocsv, R"(tocsv(self, path)
    
    以csv格式输出交易记录、未平仓记录、已平仓记录、资产净值曲线
        
    :param string path: 输出文件所在目录)")

        DEF_PICKLE(TradeManager);
}