/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-17
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/hikyuu.h>
#include "pybind_utils.h"
#include "convert_Datetime.h"
#include "convert_TimeDelta.h"

using namespace hku;
namespace py = pybind11;

void export_hikyuu(py::module& m);
void export_stl_container(py::module& m);
void export_Parameter(py::module& m);
void export_StockManager(py::module& m);
void export_Stock(py::module& m);
void export_MarketInfo(py::module& m);

PYBIND11_MODULE(_hikyuu, m) {
    m.def(
      "get_date_range",
      [](Datetime& start, Datetime& end) {
          return vector_to_python_list<Datetime>(getDateRange(start, end));
      },
      py::arg("start"), py::arg("end"),
      "Return a list of calendar dates for the specified range (start, end].");

    m.def(
      "print_datetime", [](const Datetime& d) { fmt::print("{}\n", d); },
      "test convert Python datetime <--> Datetime");

    m.def(
      "print_timedelta", [](const TimeDelta& d) { fmt::print("{}\n", d); },
      "test convert Python timedelta <--> TimeDelta");

    export_hikyuu(m);
    export_stl_container(m);
    export_Parameter(m);
    export_StockManager(m);
    export_Stock(m);
    export_MarketInfo(m);
}