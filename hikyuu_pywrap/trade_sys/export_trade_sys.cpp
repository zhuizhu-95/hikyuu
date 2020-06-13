/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-12
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>

namespace py = pybind11;

// void export_SystemPart(py::module& m);
void export_AllocateFunds(py::module& m);
void export_Condition(py::module& m);
void export_Environment(py::module& m);
void export_MoneyManager(py::module& m);
void export_Portfolio(py::module& m);
void export_Profitgoal(py::module& m);
void export_Selector(py::module& m);
void export_Signal(py::module& m);
void export_Slippage(py::module& m);
void export_Stoploss(py::module& m);
void export_System(py::module& m);

void export_trade_sys(py::module& m) {
    // export_SystemPart(m); // 必须在 export_trade_manage 之前
    export_AllocateFunds(m);
    export_Condition(m);
    export_Environment(m);
    export_MoneyManager(m);
    export_Portfolio(m);
    export_Profitgoal(m);
    export_Selector(m);
    export_Signal(m);
    export_Slippage(m);
    export_Stoploss(m);
    export_System(m);
}