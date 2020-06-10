/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-11
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <hikyuu/trade_manage/TradeManager.h>
#include "../convert_Datetime.h"
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
      //.def_property_readonly("costFunc", get_costFunc, set_costFunc)
      //.add_property("brokeLastDatetime",
      //        &TradeManager::getBrokerLastDatetime,
      //        &TradeManager::setBrokerLastDatetime)

      DEF_PICKLE(TradeManager);
}