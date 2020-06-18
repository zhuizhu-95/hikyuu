/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-14
 *      Author: fasiondog
 */

#include "_BlockInfoDriver.h"

void export_BlockInfoDriver(py::module& m) {
    py::class_<BlockInfoDriver, BlockInfoDriverPtr, PyBlockInfoDriver>(m, "BlockInfoDriver",
                                                                       R"(板块数据驱动基类
    
    子类接口：
        - _init(self) (必须)
        _ getBlock(self, category, name) （必须）
        _ _getBlockList(self, category=None) （必须）
    )")
      .def(py::init<const string&>(), R"(初始化

    :param str name: 驱动名称)")

      .def_property_readonly("name", &BlockInfoDriver::name, py::return_value_policy::copy,
                             "驱动名称")

      .def("__str__", to_py_str<BlockInfoDriver>)
      .def("__repr__", to_py_str<BlockInfoDriver>)

      .def("getParam", &BlockInfoDriver::getParam<boost::any>, "获取指定参数")
      .def("setParam", &BlockInfoDriver::setParam<boost::any>, "设置指定参数")
      .def("haveParam", &BlockInfoDriver::haveParam, "指定参数是否存在")

      .def("_init", &BlockInfoDriver::_init, "【子类接口（必须）】驱动初始化")
      .def("getBlock", &BlockInfoDriver::getBlock, py::arg("category"), py::arg("name"),
           R"(【子类接口（必须）】获取指定板块

    :param str category: 指定的板块分类
    :param str name: 板块名称)");
}