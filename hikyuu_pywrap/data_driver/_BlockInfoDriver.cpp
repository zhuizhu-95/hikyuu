/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-14
 *      Author: fasiondog
 */

#include <hikyuu/indicator/build_in.h>
#include <pybind11/pybind11.h>
#include <hikyuu/data_driver/BlockInfoDriver.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"

using namespace hku;
namespace py = pybind11;

class PyBlockInfoDriver : public BlockInfoDriver {
public:
    using BlockInfoDriver::BlockInfoDriver;

    bool _init() override {
        PYBIND11_OVERLOAD_PURE(bool, BlockInfoDriver, _init, );
    }

    Block getBlock(const string& category, const string& name) override {
        PYBIND11_OVERLOAD_PURE(Block, BlockInfoDriver, getBlock, category, name);
    }

    BlockList getBlockList(const string& category) {
        auto self = py::cast(this);
        auto py_list = self.attr("_getBlockList")(category);
        return python_list_to_vector<Block>(py_list);
    }

    BlockList getBlockList() {
        auto self = py::cast(this);
        auto py_list = self.attr("_getBlockList")();
        return python_list_to_vector<Block>(py_list);
    }
};

void export_BlockInfoDriver(py::module& m) {
    py::class_<BlockInfoDriver, BlockInfoDriverPtr, PyBlockInfoDriver>(m, "BlockInfoDriver")
      .def(py::init<const string&>())

      .def_property_readonly("name", &BlockInfoDriver::name)

      .def("__str__", to_py_str<BlockInfoDriver>)
      .def("__repr__", to_py_str<BlockInfoDriver>)

      .def("getParam", &BlockInfoDriver::getParam<boost::any>)

      .def("init", &BlockInfoDriver::init)
      .def("_init", &BlockInfoDriver::_init)
      .def("getBlock", &BlockInfoDriver::getBlock);
}