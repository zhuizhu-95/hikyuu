/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-10
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_manage/TradeRecord.h>
#include "../convert_Datetime.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_TradeRecord(py::module& m) {
    py::enum_<BUSINESS>(m, "BUSINESS", "业务类型")
      .value("BUSINESS_INIT", BUSINESS_INIT, "建立初始账户")
      .value("BUSINESS_BUY", BUSINESS_BUY, "买入")
      .value("BUSINESS_SELL", BUSINESS_SELL, "卖出")
      .value("BUSINESS_GIFT", BUSINESS_GIFT, "送股")
      .value("BUSINESS_BONUS", BUSINESS_BONUS, "分红")
      .value("BUSINESS_CHECKIN", BUSINESS_CHECKIN, "存入现金")
      .value("BUSINESS_CHECKOUT", BUSINESS_CHECKOUT, "取出现金")
      .value("BUSINESS_CHECKIN_STOCK", BUSINESS_CHECKIN_STOCK, "存入证券资产")
      .value("BUSINESS_CHECKOUT_STOCK", BUSINESS_CHECKOUT_STOCK, "取出证券资产")
      .value("BUSINESS_BORROW_CASH", BUSINESS_BORROW_CASH, "借入资金")
      .value("BUSINESS_RETURN_CASH", BUSINESS_RETURN_CASH, "归还资金")
      .value("BUSINESS_BORROW_STOCK", BUSINESS_BORROW_STOCK, "借入证券资产")
      .value("BUSINESS_RETURN_STOCK", BUSINESS_RETURN_STOCK, "归还证券资产")
      //.value("BUSINESS_SELL_SHORT", BUSINESS_SELL_SHORT, "卖空")
      //.value("BUSINESS_BUY_SHORT", BUSINESS_BUY_SHORT, "卖空后回补")
      .value("BUSINESS_INVALID", BUSINESS_INVALID, "无效业务类型")
      .export_values();

    m.def("getBusinessName", getBusinessName, "获取业务名称，用于打印输出");

    py::class_<TradeRecord>(m, "TradeRecord", "交易记录")
      .def(py::init<>())
      .def(py::init<const Stock&, const Datetime&, BUSINESS, price_t, price_t, price_t, double,
                    const CostRecord&, price_t, price_t, SystemPart>())
      .def("__str__", &TradeRecord::toString)
      .def("__repr__", &TradeRecord::toString)
      .def_readwrite("stock", &TradeRecord::stock, "交易对象")
      .def_readwrite("date", &TradeRecord::datetime, "交易时间")
      .def_readwrite("business", &TradeRecord::business, "业务类型")
      .def_readwrite("plan_price", &TradeRecord::planPrice, "计划交易价格")
      .def_readwrite("real_price", &TradeRecord::realPrice, "实际交易价格")
      .def_readwrite("goal_price", &TradeRecord::goalPrice, "目标价位")
      .def_readwrite("number", &TradeRecord::number, "成交数量")
      .def_readwrite("cost", &TradeRecord::cost, "交易成本")
      .def_readwrite("stoploss", &TradeRecord::stoploss, "止损价")
      .def_readwrite("cash", &TradeRecord::cash, "现金余额")
      .def_readwrite("part", &TradeRecord::from,
                     "辅助记录交易系统部件，区别是哪个部件发出的指示")  // python中不能用from关键字
      DEF_PICKLE(TradeRecord);
}