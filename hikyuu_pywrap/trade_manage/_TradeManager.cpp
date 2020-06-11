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
    py::class_<TradeManager, TradeManagerPtr>(m, "TradeManager")
      .def(py::init<const Datetime&, price_t, const TradeCostPtr&, const string&>())

      .def("__str__", &TradeManager::toString)
      .def("__repr__", &TradeManager::toString)

      .def_property("name", py::overload_cast<>(&TradeManager::name, py::const_),
                    py::overload_cast<const string&>(&TradeManager::name))
      .def_property_readonly("init_cash", &TradeManager::initCash)
      .def_property_readonly("current_cash", &TradeManager::currentCash)
      .def_property_readonly("init_date", &TradeManager::initDatetime)
      .def_property_readonly("first_date", &TradeManager::firstDatetime)
      .def_property_readonly("last_date", &TradeManager::lastDatetime)
      .def_property_readonly("reinvest", &TradeManager::reinvest)
      .def_property_readonly("precision", &TradeManager::precision)
      .def_property("cost_func", py::overload_cast<void>(&TradeManager::costFunc, py::const_),
                    py::overload_cast<const TradeCostPtr&>(&TradeManager::costFunc))
      .def_property("broke_last_date", &TradeManager::getBrokerLastDatetime,
                    &TradeManager::setBrokerLastDatetime)

      .def("get_param", &TradeManager::getParam<boost::any>)
      .def("set_param", &TradeManager::setParam<boost::any>)
      .def("have_param", &TradeManager::haveParam)

      .def("reset", &TradeManager::reset)
      .def("clone", &TradeManager::clone)

      .def("reg_broker", &TradeManager::regBroker, py::keep_alive<1, 2>())
      .def("clear_broker", &TradeManager::clearBroker)

      //.def("getMarginRate", &TradeManager::getMarginRate)
      .def("have", &TradeManager::have)
      .def("get_stock_number", &TradeManager::getStockNumber)
      //.def("getShortStockNumber", &TradeManager::getShortStockNumber)
      .def("get_hold_number", &TradeManager::getHoldNumber)
      //.def("getShortHoldNumber", &TradeManager::getShortHoldNumber)

      .def(
        "get_trade_list",
        [](const TradeManager& tm, const Datetime& start, const Datetime& end) {
            return vector_to_python_list(tm.getTradeList(start, end));
        },
        py::arg("start") = Datetime::min(), py::arg("end") = Datetime())

      .def("get_position_list",
           [](const TradeManager& tm) { return vector_to_python_list(tm.getPositionList()); })

      .def(
        "get_history_position_list",
        [](const TradeManager& tm) { return vector_to_python_list(tm.getHistoryPositionList()); })

      .def("get_position", &TradeManager::getPosition)
      .def("get_buy_cost", &TradeManager::getBuyCost)
      .def("get_sell_cost", &TradeManager::getSellCost)
      //.def("getBorrowCashCost", &TradeManager::getBorrowCashCost)
      //.def("getReturnCashCost", &TradeManager::getReturnCashCost)
      //.def("getBorrowStockCost", &TradeManager::getBorrowStockCost)
      //.def("getReturnStockCost", &TradeManager::getReturnStockCost)

      .def("cash", &TradeManager::cash, py::arg("date"), py::arg("ktype") = KQuery::DAY)

      .def("get_funds", py::overload_cast<KQuery::KType>(&TradeManager::getFunds, py::const_),
           py::arg("ktype") = KQuery::DAY)
      .def("get_funds", py::overload_cast<const Datetime&, KQuery::KType>(&TradeManager::getFunds),
           py::arg("date"), py::arg("ktype") = KQuery::DAY)

      .def("get_funds_curve", &TradeManager::getFundsCurve, py::arg("dates"),
           py::arg("ktype") = KQuery::DAY)
      .def("get_profit_curve", &TradeManager::getProfitCurve, py::arg("dates"),
           py::arg("ktype") = KQuery::DAY)

      .def("checkin", &TradeManager::checkin)
      .def("checkout", &TradeManager::checkout)
      //.def("checkinStock", &TradeManager::checkinStock)
      //.def("checkoutStock", &TradeManager::checkoutStock)
      //.def("borrowCash", &TradeManager::borrowCash)
      //.def("returnCash", &TradeManager::returnCash)
      //.def("borrowStock", &TradeManager::borrowStock)
      //.def("returnStock", &TradeManager::returnStock)

      //.def("buy", &TradeManager::buy, py::arg("date"), py::arg("stock"), py::arg("real_price"),
      //     py::arg("num"), py::arg("stoploss") = 0.0, py::arg("goal_price") = 0.0,
      //     py::arg("plan_price") = 0.0, py::arg("part") = PART_INVALID)

      DEF_PICKLE(TradeManager);
}