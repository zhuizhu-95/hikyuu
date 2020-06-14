/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-30
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include "convert_Datetime.h"
#include "pickle_support.h"
#include <hikyuu/serialization/StockWeight_serialization.h>

using namespace hku;
namespace py = pybind11;

string StockWeight_to_string(const StockWeight& sw) {
    return fmt::format("StockWeight({}, {:<.4f}, {:<.4f},{:<.4f},{:<.4f},{:<.4f},{:<.4f},{:<.4f})",
                       sw.datetime(), sw.countAsGift(), sw.countForSell(), sw.priceForSell(),
                       sw.bonus(), sw.increasement(), sw.totalCount(), sw.freeCount());
}

void export_StockWeight(py::module& m) {
    py::class_<StockWeight>(m, "StockWeight", "权息数据")
      .def(py::init<>())
      .def(py::init<const Datetime&>())
      .def(
        py::init<const Datetime&, price_t, price_t, price_t, price_t, price_t, price_t, price_t>())

      .def("__str__", StockWeight_to_string)
      .def("__repr__", StockWeight_to_string)

      .def_property_readonly("date", &StockWeight::datetime, "权息日期")
      .def_property_readonly("countAsGift", &StockWeight::countAsGift, "每10股送X股")
      .def_property_readonly("countForSell", &StockWeight::countForSell, "每10股配X股")
      .def_property_readonly("priceForSell", &StockWeight::priceForSell, "配股价")
      .def_property_readonly("bonus", &StockWeight::bonus, "每10股红利")
      .def_property_readonly("increasement", &StockWeight::increasement, "每10股转增X股")
      .def_property_readonly("totalCount", &StockWeight::totalCount, "总股本（万股）")
      .def_property_readonly("freeCount", &StockWeight::freeCount, "流通股（万股）")

        DEF_PICKLE(StockWeight);
}