/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/portfolio/build_in.h>
#include "../convert_any.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_Portfolio(py::module& m) {
    py::class_<Portfolio, PFPtr>(m, "Portfolio", R"(实现多标的、多策略的投资组合)")
      .def(py::init<>())
      .def(py::init<const string&>())
      .def(py::init<const TradeManagerPtr&, const SelectorPtr&, const AFPtr&>())

      .def("__str__", to_py_str<Portfolio>)
      .def("__repr__", to_py_str<Portfolio>)

      .def("getParam", &Portfolio::getParam<boost::any>)
      .def("setParam", &Portfolio::setParam<boost::any>)
      .def("haveParam", &Portfolio::haveParam)

      .def_property("name", py::overload_cast<void>(&Portfolio::name, py::const_),
                    py::overload_cast<const string&>(&Portfolio::name), "名称")

      .def_property("tm", &Portfolio::getTM, &Portfolio::setTM, "关联的交易管理实例")
      .def_property("se", &Portfolio::getSE, &Portfolio::setSE, "选择器策略")
      .def("run", &Portfolio::run, R"(运行投资组合策略
        
    :param Query query: 查询条件)")

        DEF_PICKLE(Portfolio);

    m.def("PF_Simple", PF_Simple, py::arg("tm") = TradeManagerPtr(), py::arg("se") = SE_Fixed(),
          py::arg("af") = AF_EqualWeight(),
          R"(创建一个多标的、单系统策略的投资组合
    
    :param TradeManager tm: 交易管理
    :param System sys: 系统策略
    :param SelectorBase se: 选择器)");
}