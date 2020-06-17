/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-18
 *      Author: fasiondog
 */

#pragma once

#include <hikyuu/indicator/build_in.h>
#include <pybind11/pybind11.h>
#include <hikyuu/data_driver/BaseInfoDriver.h>
#include "../convert_any.h"
#include "../pybind_utils.h"

using namespace hku;
namespace py = pybind11;

class PyBaseInfoDriver : public BaseInfoDriver {
public:
    using BaseInfoDriver::BaseInfoDriver;

    Parameter getFinanceInfo(const string& market, const string& code) override {
        PYBIND11_OVERLOAD(Parameter, BaseInfoDriver, getFinanceInfo, market, code);
    }

    bool _init() override {
        PYBIND11_OVERLOAD_PURE(bool, BaseInfoDriver, _init, );
    }

    bool _loadMarketInfo() override {
        PYBIND11_OVERLOAD_PURE(bool, BaseInfoDriver, _loadMarketInfo, );
    }

    bool _loadStockTypeInfo() override {
        PYBIND11_OVERLOAD_PURE(bool, BaseInfoDriver, _loadStockTypeInfo, );
    }

    bool _loadStock() override {
        PYBIND11_OVERLOAD_PURE(bool, BaseInfoDriver, _loadStock, );
    }
};