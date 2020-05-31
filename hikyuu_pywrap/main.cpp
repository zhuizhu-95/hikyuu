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

void export_Constant(py::module& m);
void export_utils(py::module& m);
void export_hikyuu(py::module& m);
void export_Log(py::module& m);
void export_stl_container(py::module& m);
void export_Parameter(py::module& m);
void export_KRecord(py::module& m);
void export_StockManager(py::module& m);
void export_Stock(py::module& m);
void export_Block(py::module& m);
void export_MarketInfo(py::module& m);
void export_StockTypeInfo(py::module& m);
void export_StockType(py::module& m);
void export_KQuery(py::module& m);
void export_TimeLineRecord(py::module& m);
void export_TransRecord(py::module& m);
void export_StockWeight(py::module& m);
void export_KData(py::module& m);

void export_Indicator(py::module& m);
void export_IndicatorImp(py::module& m);
void export_Indicator_build_in(py::module& m);

PYBIND11_MODULE(core, m) {
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

    export_Constant(m);
    export_utils(m);
    export_hikyuu(m);
    export_Log(m);
    export_stl_container(m);
    export_Parameter(m);
    export_KRecord(m);
    export_StockManager(m);
    export_Stock(m);
    export_Block(m);
    export_MarketInfo(m);
    export_StockType(m);
    export_StockTypeInfo(m);
    export_KQuery(m);
    export_TimeLineRecord(m);
    export_TransRecord(m);
    export_StockWeight(m);
    export_KData(m);

    export_Indicator(m);
    export_IndicatorImp(m);
    export_Indicator_build_in(m);
}