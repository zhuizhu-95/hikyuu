/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-27
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include "pickle_support.h"
#include "StockType.h"

using namespace hku;
namespace py = pybind11;

void export_StockType(py::module& m) {
    py::enum_<StockType>(m, "StockType")
      .value("BLOCK", StockType::BLOCK, "板块")
      .value("A", StockType::A, "A股")
      .value("INDEX", StockType::INDEX, "指数")
      .value("B", StockType::B, "B股")
      .value("FUND", StockType::FUND, "基金")
      .value("ETF", StockType::ETF, "ETF")
      .value("ND", StockType::ND, "国债")
      .value("BOND", StockType::BOND, "债券")
      .value("GEM", StockType::GEM, "创业板")
      .value("BTC", StockType::BTC, "比特币")
      .value("TMP", StockType::TMP, "用于临时Stock")
      .export_values();
}