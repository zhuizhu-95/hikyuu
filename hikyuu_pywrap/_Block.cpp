/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-30
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include "pybind_utils.h"
#include "pickle_support.h"
#include <hikyuu/serialization/Block_serialization.h>

using namespace hku;
namespace py = pybind11;

string Block_to_string(const Block& blk) {
    return fmt::format("{}", blk);
}

void export_Block(py::module& m) {
    const char* get_doc = R"(获取指定的证券

:param str market_code: 市场简称证券代码)";

    py::class_<Block>(m, "Block", "板块类，可视为证券的容器")
      .def(py::init<>())
      .def(py::init<const string&, const string&>(), py::arg("category"), py::arg("name"))
      .def(py::init<const Block&>())

      .def("__str__", Block_to_string)
      .def("__repr__", Block_to_string)
      .def("__len__", &Block::size, "包含的证券数量")

      .def_property("category", py::overload_cast<void>(&Block::category, py::const_),
                    py::overload_cast<const string&>(&Block::category), "板块类别")
      .def_property("name", py::overload_cast<void>(&Block::name, py::const_),
                    py::overload_cast<const string&>(&Block::name), "板块名称")

      .def("size", &Block::size, "包含的证券数量")
      .def("empty", &Block::empty)

      .def("get", &Block::get, get_doc)
      .def("__getitem__", &Block::get, get_doc)

      .def("add", py::overload_cast<const Stock&>(&Block::add), R"(加入指定的证券

:param Stock stock: 待加入的证券
:return: 是否成功加入)",
           py::keep_alive<1, 2>())

      .def("add", py::overload_cast<const string&>(&Block::add),
           R"(根据"市场简称证券代码"加入指定的证券

:param str market_code: 市场简称证券代码
:return: 是否成功加入)",
           py::keep_alive<1, 2>())

      .def("remove", py::overload_cast<const Stock&>(&Block::remove), R"(移除指定证券

:param Stock stock: 指定的证券
:return: 是否成功)")

      .def("remove", py::overload_cast<const string&>(&Block::remove), R"(移除指定证券

:param str market_code: 市场简称证券代码
:return: 是否成功)")

      .def("clear", &Block::clear, "移除包含的所有证券")

      .def(
        "__iter__",
        [](const Block& blk) {
            return py::make_iterator<py::return_value_policy::reference_internal, StockMapIterator,
                                     StockMapIterator, const Stock&>(blk.begin(), blk.end());
        },
        py::keep_alive<0, 1>())

        DEF_PICKLE(Block);
}