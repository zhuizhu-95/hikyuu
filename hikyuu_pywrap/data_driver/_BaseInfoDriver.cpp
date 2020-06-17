/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-14
 *      Author: fasiondog
 */

#include "_BaseInfoDriver.h"

void export_BaseInfoDriver(py::module& m) {
    py::class_<BaseInfoDriver, BaseInfoDriverPtr, PyBaseInfoDriver>(
      m, "BaseInfoDriver",
      R"(基本信息数据获取驱动基类

    必须实现的子类接口：
        - _init(self)
        - _loadMarketInfo(self)
        - _loadStockTypeInfo(self)
        - _loadStock(self)

    可选实现的子类接口        
        - getFinanceInfo(self, market, code))")
      .def(py::init<const string&>(), R"(初始化构造方法
    
    :param str name: 驱动名称)")

      .def_property_readonly("name", &BaseInfoDriver::name, py::return_value_policy::copy,
                             "驱动名称")

      .def("__str__", to_py_str<BaseInfoDriver>)
      .def("__repr__", to_py_str<BaseInfoDriver>)

      .def("getParam", &BaseInfoDriver::getParam<boost::any>, "获取指定参数值")
      .def("setParam", &BaseInfoDriver::setParam<boost::any>, "设置指定参数值")
      .def("haveParam", &BaseInfoDriver::haveParam, "是否存在指定参数")

      .def("_init", &BaseInfoDriver::_init, "【子类接口（必须）】驱动初始化")
      .def("_loadMarketInfo", &BaseInfoDriver::_loadMarketInfo, "【子类接口（必须）】加载市场信息")
      .def("_loadStockTypeInfo", &BaseInfoDriver::_loadStockTypeInfo,
           "【子类接口（必须）】加载证券类型信息")
      .def("_loadStock", &BaseInfoDriver::_loadStock, "【子类接口（必须）】加载证券数据")
      .def("getFinanceInfo", &BaseInfoDriver::getFinanceInfo, "【子类接口（可选）】获取财务信息");
}