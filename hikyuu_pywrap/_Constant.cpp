/*
 * _Constant.cpp
 *
 *  Created on: 2013-4-10
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/DataType.h>
#include "pickle_support.h"

using namespace hku;
namespace py = pybind11;

struct Constant {
    Constant()
    : inf(std::numeric_limits<double>::infinity()),
      nan(std::numeric_limits<double>::quiet_NaN()),
      max_double(std::numeric_limits<double>::max()),
      null_double(Null<double>()),
      null_price(Null<price_t>()),
      null_int(Null<int>()),
      null_size(Null<size_t>()),
      null_int64(Null<int64>()) {
#if HKU_PYTHON_SUPPORT_PICKLE
        pickle_support = true;
#else
        pickle_support = false;
#endif /* HKU_PYTHON_SUPPORT_PICKLE */
    }

    double inf;
    double nan;
    double null_double;
    double max_double;
    double null_price;
    int null_int;
    size_t null_size;
    int64 null_int64;
    bool pickle_support;  //是否支持pickle
};

void export_Constant(py::module& m) {
    py::class_<Constant>(m, "Constant")
      .def(py::init<>())
      .def_readonly("inf", &Constant::inf, "无限大")
      .def_readonly("nan", &Constant::nan, "无效数字类型")
      .def_readonly("null_double", &Constant::null_double, "无效 double 类型")
      .def_readonly("max_double", &Constant::max_double, "最大 double 值")
      .def_readonly("null_price", &Constant::null_price, "无效价格类型")
      .def_readonly("null_int", &Constant::null_int, "无效 int 类型")
      .def_readonly("null_size", &Constant::null_size, "无效 size_t 类型")
      .def_readonly("null_int64", &Constant::null_int64, "无效64位整型")
      .def_readonly("pickle_support", &Constant::pickle_support, "是否支持 pickle");

    m.attr("constant") = Constant();
}
