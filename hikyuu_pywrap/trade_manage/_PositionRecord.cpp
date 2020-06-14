/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-10
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <hikyuu/trade_manage/PositionRecord.h>
#include "../convert_Datetime.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_PositionRecord(py::module& m) {
    py::class_<PositionRecord>(m, "PositionRecord", "持仓记录")
      .def(py::init<>())
      .def(py::init<const Stock&, const Datetime&, const Datetime&, double, price_t, price_t,
                    double, price_t, price_t, price_t, price_t>(),
           py::arg("stock"), py::arg("take_date"), py::arg("clean_date"), py::arg("number"),
           py::arg("stoploss"), py::arg("goal_price"), py::arg("total_number"),
           py::arg("buy_money"), py::arg("total_cost"), py::arg("total_risk"),
           py::arg("sell_money"))

      .def("__str__", &PositionRecord::toString)
      .def("__repr__", &PositionRecord::toString)

      .def_readwrite("stock", &PositionRecord::stock, "交易对象")
      .def_readwrite("takeDatetime", &PositionRecord::takeDatetime, "初次建仓日期")
      .def_readwrite("cleanDatetime", &PositionRecord::cleanDatetime, "平仓日期")
      .def_readwrite("number", &PositionRecord::number, "当前持仓数量")
      .def_readwrite("stoploss", &PositionRecord::stoploss, "当前止损价")
      .def_readwrite("goalPrice", &PositionRecord::goalPrice, "当前的目标价格")
      .def_readwrite("totalNumber", &PositionRecord::totalNumber, "累计持仓数量")
      .def_readwrite("buyMoney", &PositionRecord::buyMoney, "累计买入资金")
      .def_readwrite("totalCost", &PositionRecord::totalCost, "累计交易总成本")
      .def_readwrite("totalRisk", &PositionRecord::totalRisk,
                     "累计交易风险 = 各次 （买入价格-止损)*买入数量, 不包含交易成本")
      .def_readwrite("sellMoney", &PositionRecord::sellMoney, "累计卖出资金")

        DEF_PICKLE(PositionRecord);
}
