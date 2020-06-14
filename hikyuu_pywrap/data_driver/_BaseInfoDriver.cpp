/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-14
 *      Author: fasiondog
 */

#include <hikyuu/indicator/build_in.h>
#include <pybind11/pybind11.h>
#include <hikyuu/data_driver/BaseInfoDriver.h>
#include "../_Parameter.h"
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

void export_BaseInfoDriver(py::module& m) {
    py::class_<BaseInfoDriver, BaseInfoDriverPtr, PyBaseInfoDriver>(m, "BaseInfoDriver")
      .def(py::init<const string&>())

      .def_property_readonly("name", &BaseInfoDriver::name)

      .def("__str__", to_py_str<BaseInfoDriver>)
      .def("__repr__", to_py_str<BaseInfoDriver>)

      .def("getParam", &BaseInfoDriver::getParam<boost::any>)

      .def("init", &BaseInfoDriver::init)
      .def("loadBaseInfo", &BaseInfoDriver::loadBaseInfo)
      .def("_init", &BaseInfoDriver::_init)
      .def("_loadMarketInfo", &BaseInfoDriver::_loadMarketInfo)
      .def("_loadStockTypeInfo", &BaseInfoDriver::_loadStockTypeInfo)
      .def("_loadStock", &BaseInfoDriver::_loadStock)
      .def("getFinanceInfo", &BaseInfoDriver::getFinanceInfo);
}