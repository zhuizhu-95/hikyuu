/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-14
 *      Author: fasiondog
 */

#include "_KDataDriver.h"

void export_KDataDriver(py::module& m) {
    py::class_<KDataDriver, KDataDriverPtr, PyKDataDriver>(m, "KDataDriver",
                                                           R"(K线数据驱动基类
    
  子类接口:
    - _init(self)
    - getCount(self, market, code, ktype)
    - getKRecord(self, market, code, pos, ktype)
    - _loadKDate(self, market, code, ktype, startix, endix)
    - _getIndexRangeByDate(self, market, code, query)
    - _getTimeLineList(self, market, code, query)
    _ _getTransList(self, market, code, query)
  
  )")
      .def(py::init<>())
      .def(py::init<const string&>(), R"(构造函数)
      
    :param str name: 驱动名称)")

      .def_property_readonly("name", &KDataDriver::name, py::return_value_policy::copy, "驱动名称")

      .def("__str__", to_py_str<KDataDriver>)
      .def("__repr__", to_py_str<KDataDriver>)

      .def("getParam", &KDataDriver::getParam<boost::any>, "获取指定参数的值")
      .def("setParam", &KDataDriver::setParam<boost::any>, "设置参数")
      .def("haveParam", &KDataDriver::haveParam, "指定参数是否存在")

      .def("_init", &KDataDriver::_init, "【子类接口】初始化驱动")
      .def("getCount", &KDataDriver::getCount, py::arg("market"), py::arg("code"), py::arg("ktype"),
           R"(【子类接口】获取K线记录数量
    
    :param str markt: 市场简称
    :param str code: 证券代码
    :param Query.KType ktype: K线类型
    :rtype int)")

      .def("getKRecord", &KDataDriver::getKRecord, py::arg("markt"), py::arg("code"),
           py::arg("pos"), py::arg("ktype"), R"(【子类接口】获取指定的K线记录
      
    :param str market: 市场简称
    :param str code: 证券代码
    :param int pos: K线记录索引位置
    :param Query.KType ktype: K线类型)");
}