/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-26
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/StockManager.h>
#include "pybind_utils.h"
#include "StockType.h"

using namespace hku;
namespace py = pybind11;

void export_StockManager(py::module& m) {
    py::class_<StockManager>(m, "StockManager", "Securities information unified management")
      .def("instance", &StockManager::instance, py::return_value_policy::reference)
      .def("init", &StockManager::init)
      .def("tmpdir", &StockManager::tmpdir, "Get the temporary path of hikyuu.")
      .def("datadir", &StockManager::datadir, "Return the data path of hikyuu.")

      .def("setKDataDriver", &StockManager::setKDataDriver)
      .def("getBaseInfoDriverParameter", &StockManager::getBaseInfoDriverParameter)
      .def("getBlockDriverParameter", &StockManager::getBlockDriverParameter)
      .def("getKDataDriverParameter", &StockManager::getKDataDriverParameter)
      .def("getPreloadParameter", &StockManager::getPreloadParameter)
      .def("getHikyuuParameter", &StockManager::getHikyuuParameter)

      .def(
        "getAllMarket",
        [](const StockManager& sm) { return vector_to_python_list<string>(sm.getAllMarket()); },
        "获取市场简称列表")

      .def("getMarketInfo", &StockManager::getMarketInfo, "获取相应的市场信息")

      .def(
        "getStockTypeInfo",
        [](const StockManager& sm, StockType& tp) { return sm.getStockTypeInfo(tp); },
        "获取证券类型详细信息")

      .def("size", &StockManager::size, "证券数量")
      .def("getStock", &StockManager::getStock,
           "Return the stock object of the specified code, like 'sh000001'")

      .def("getBlock", &StockManager::getBlock, R"(获取预定义的板块

:param str category: 板块分类
:param str name: 板块名称
:return: 板块，如找不到返回一个空 Block )")

      .def(
        "getBlockList",
        [](StockManager& sm, const string& category) {
            return vector_to_python_list<Block>(sm.getBlockList(category));
        },
        R"(获取指定分类的板块列表

:param str category: 板块分类)")

      .def(
        "getBlockList",
        [](StockManager& sm) { return vector_to_python_list<Block>(sm.getBlockList()); },
        "获取全部的板块列表")

      .def("getTradingCalendar", &StockManager::getTradingCalendar, py::arg("query"),
           py::arg("market") = "SH", R"(获取指定市场的交易日日历

:param Query query: Query查询条件
:param str market: 市场简称
:return: 日期列表)")

      .def(
        "addTempCsvStock", &StockManager::addTempCsvStock, py::arg("code"), py::arg("day_filename"),
        py::arg("min_filename"), py::arg("tick") = 0.01, py::arg("tick_value") = 0.01,
        py::arg("precision") = 2, py::arg("min_tradeNumber") = 1,
        py::arg("max_tradeNumber") = 1000000,
        R"(从CSV文件（K线数据）增加临时的Stock，可用于只有CSV格式的K线数据时，进行临时测试。        

CSV文件第一行为标题，需含有 Datetime（或Date、日期）、OPEN（或开盘价）、HIGH（或最高价）、LOW（或最低价）、CLOSE（或收盘价）、AMOUNT（或成交金额）、VOLUME（或VOL、COUNT、成交量）。

:param str code: 自行编号的证券代码，不能和已有的Stock相同，否则将返回Null<Stock>
:param str day_filename: 日线CSV文件名
:param str min_filename: 分钟线CSV文件名
:param float tick: 最小跳动量，默认0.01
:param float tickValue: 最小跳动量价值，默认0.01
:param int precision: 价格精度，默认2
:param int minTradeNumber: 单笔最小交易量，默认1
:param int maxTradeNumber: 单笔最大交易量，默认1000000
:return: 加入的Stock)",
        py::keep_alive<1, 2>())

      .def("removeTempCsvStock", &StockManager::removeTempCsvStock,
           R"(移除增加的临时Stock

:param str code: 创建时自定义的编码)")

      .def("__len__", &StockManager::size, "证券数量")
      .def("__getitem__", &StockManager::getStock,
           "Return the stock object of the specified code, like 'sh000001'")

      .def(
        "__iter__",
        [](const StockManager& sm) {
            return py::make_iterator<py::return_value_policy::reference_internal, StockMapIterator,
                                     StockMapIterator, const Stock&>(sm.begin(), sm.end());
        },
        py::keep_alive<0, 1>());
}
