/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/allocatefunds/build_in.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PyAllocateFundsBase : public AllocateFundsBase {
    PY_CLONE(PyAllocateFundsBase, AllocateFundsBase)

public:
    using AllocateFundsBase::AllocateFundsBase;

    void _reset() override {
        PYBIND11_OVERLOAD(void, AllocateFundsBase, _reset, );
    }

    SystemWeightList _allocateWeight(const Datetime& date, const SystemList& se_list) override {
        PYBIND11_OVERLOAD_PURE(SystemWeightList, AllocateFundsBase, _allocateWeight, date, se_list);
    }
};

void export_AllocateFunds(py::module& m) {
    py::class_<AllocateFundsBase, AFPtr, PyAllocateFundsBase>(m, "AllocateFundsBase")
      .def(py::init<>())

      .def("__str__", to_py_str<AllocateFundsBase>)
      .def("__repr__", to_py_str<AllocateFundsBase>)

      .def_property("name", py::overload_cast<void>(&AllocateFundsBase::name, py::const_),
                    py::overload_cast<const string&>(&AllocateFundsBase::name))
      .def("getParam", &AllocateFundsBase::getParam<boost::any>)
      .def("setParam", &AllocateFundsBase::setParam<boost::any>)
      .def("haveParam", &AllocateFundsBase::haveParam)

      .def("reset", &AllocateFundsBase::reset)
      .def("clone", &AllocateFundsBase::clone)
      .def("_reset", &AllocateFundsBase::_reset)
      .def("_allocateWeight", &AllocateFundsBase::_allocateWeight)

        DEF_PICKLE(AFPtr);
}