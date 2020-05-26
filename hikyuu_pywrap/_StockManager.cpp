/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-26
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/StockManager.h>
#include "pybind_utils.h"

using namespace hku;
namespace py = pybind11;

void export_StockManager(py::module& m) {
    py::class_<StockManager>(m, "StockManager", "Securities information unified management")
      .def("instance", &StockManager::instance, py::return_value_policy::reference)
      .def("init", &StockManager::init)
      .def("tmpdir", &StockManager::tmpdir, "Get the temporary path of hikyuu.")
      .def("setKDataDriver", &StockManager::setKDataDriver)
      .def("getBaseInfoDriverParameter", &StockManager::getBaseInfoDriverParameter)
      .def("getBlockDriverParameter", &StockManager::getBlockDriverParameter)
      .def("getKDataDriverParameter", &StockManager::getKDataDriverParameter)
      .def("getPreloadParameter", &StockManager::getPreloadParameter)
      .def("getHikyuuParameter", &StockManager::getHikyuuParameter)

      .def("get_all_market",
           [](const StockManager& sm) { return vector_to_python_list<string>(sm.getAllMarket()); })
      .def("get_market_info", &StockManager::getMarketInfo)

      .def("getStockTypeInfo", &StockManager::getStockTypeInfo)
      .def("size", &StockManager::size)
      .def("get_stock", &StockManager::getStock,
           "Return the stock object of the specified code, like 'sh000001'")
      .def("getBlock", &StockManager::getBlock)
      .def("getBlockList", py::overload_cast<const string&>(&StockManager::getBlockList))
      .def("getBlockList", py::overload_cast<>(&StockManager::getBlockList))

      .def("getTradingCalendar", &StockManager::getTradingCalendar, py::arg("query"),
           py::arg("market") = "SH")
      .def("addTempCsvStock", &StockManager::addTempCsvStock, py::arg("code"),
           py::arg("day_filename"), py::arg("min_filename"), py::arg("tick") = 0.01,
           py::arg("tick_value") = 0.01, py::arg("precision") = 2, py::arg("min_tradeNumber") = 1,
           py::arg("max_tradeNumber") = 1000000)
      .def("removeTempCsvStock", &StockManager::removeTempCsvStock)

      .def("__len__", &StockManager::size)
      .def("__getitem__", &StockManager::getStock,
           "Return the stock object of the specified code, like 'sh000001'")

      ;
}
