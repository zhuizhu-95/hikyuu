/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-14
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/system/build_in.h>
#include "../convert_Datetime.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_TradeRequest(py::module& m) {
    py::class_<TradeRequest>(
      m, "TradeRequest",
      R"(交易请求记录。系统内部在实现延迟操作时登记的交易请求信息。暴露该结构的主要目的是用于
在“delay”模式（延迟到下一个bar开盘时进行交易）的情况下，系统实际已知下一个Bar将要
进行交易，此时可通过 System.getBuyTradeRequest() 、 System.getSellTradeRequest()
来获知下一个BAR是否需要买入/卖出。主要用于提醒或打印下一个Bar需要进行操作。对于系统
本身的运行没有影响。)")

      .def(py::init<>())
      .def_readwrite("valid", &TradeRequest::valid, R"(该交易请求记录是否有效（True | False）)")
      .def_readwrite("business", &TradeRequest::business, "交易业务类型")
      .def_readwrite("date", &TradeRequest::datetime, "发出交易请求的时刻")
      .def_readwrite("stoploss", &TradeRequest::stoploss, "发出交易请求时刻的止损价")
      .def_readwrite("part", &TradeRequest::from, "发出交易请求的来源系统部件")
      .def_readwrite("count", &TradeRequest::count, "因操作失败，连续延迟的次数")

        DEF_PICKLE(TradeRequest);
}