/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-31
 *      Author: fasiondog
 */

#include <hikyuu/indicator/Indicator.h>
#include <pybind11/pybind11.h>
#include "../convert_Datetime.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

string Indicator_to_string(const Indicator& ind) {
    return fmt::format("{}", ind);
}

void export_Indicator(py::module& m) {
    py::class_<Indicator>(m, "Indicator", "技术指标")
      .def(py::init<>())
      .def(py::init<IndicatorImpPtr>())

      .def("__str__", Indicator_to_string)
      .def("__repr__", Indicator_to_string)

      .def_property("name", py::overload_cast<void>(&Indicator::name, py::const_),
                    py::overload_cast<const string&>(&Indicator::name), "指标名称")

      .def_property_readonly("long_name", &Indicator::long_name,
                             "待参数的指标长名称，如：Name(param1=val,param2=val,...) ")

      .def_property_readonly("discard", &Indicator::discard, "需抛弃的数量")

      .def("set_discard", &Indicator::setDiscard, "设置抛弃的个数，如果小于原有的 discard 则无效")
      .def("get_param", &Indicator::getParam<boost::any>, "获取指标参数")
      .def("set_param", &Indicator::setParam<boost::any>, "设置指标参数")
      .def("have_param", &Indicator::haveParam, "判断是否存在指定的参数")
      .def("size", &Indicator::size)
      .def("empty", &Indicator::empty, "是否为空")
      .def("clone", &Indicator::clone, "克隆操作")
      .def("formula", &Indicator::formula, "打印指标公式")
      .def("get_result_number", &Indicator::getResultNumber, "获取结果集数量")

      .def("get", &Indicator::get, py::arg("pos"), py::arg("num") = 0,
           R"(获取指定日期数值。如果对应日期无结果，返回 constant.null_price

:param datetime date: 指定日期
:param int num: 指定的结果集)")

      .def("get_pos", &Indicator::getPos, R"(获取指定日期相应的索引位置

:param datetime date: 指定日期)")

      .def("get_date", &Indicator::getDatetime, "获取指定索引位置的日期")

      .def("get_by_date", &Indicator::getByDate, py::arg("date"), py::arg("num") = 0,
           R"(获取指定日期的数据

:param datetime date: 指定日期
:param int num: 第几个结果集)")

      .def("get_result", &Indicator::getResult, R"(获取指定结果集

:param int num: 指定的结果集
:rtype: Indicator)")

      .def("get_result_as_price_list", &Indicator::getResultAsPriceList, R"(获取指定结果集

:param int num: 指定的结果集
:rtype: list)")

      .def("get_date_list", &Indicator::getDatetimeList, "获取日期列表")

      .def("set_context", py::overload_cast<const Stock&, const KQuery&>(&Indicator::setContext),
           R"(设置上下文

:param Stock stock: 指定的 Stock
:param Query query: 指定的查询条件)")

      .def("set_context", py::overload_cast<const KData&>(&Indicator::setContext),
           R"(设置上下文

:param KData kdata: 关联的上下文K线)")

      .def("get_context", &Indicator::getContext, "获取上下文")

      .def("get_imp", &Indicator::getImp, "获取内部实现实例")
      .def("__len__", &Indicator::size)

      .def("__call__", py::overload_cast<void>(&Indicator::operator()))
      .def("__call__", py::overload_cast<const Indicator&>(&Indicator::operator()))
      .def("__call__", py::overload_cast<const KData&>(&Indicator::operator()))

      .def("__getitem__",
           [](const Indicator& ind, long long i) {
               long long len = ind.size();
               long long index = i;
               if (index < 0) {
                   index = len + index;
               }

               if (index < 0 || index >= len) {
                   HKU_THROW_EXCEPTION(std::out_of_range, "Index out of range: {}", i);
               }

               return ind.get(index);
           })

      .def("__getitem__", [](const Indicator& ind, const Datetime& d) { return ind.getByDate(d); })

      .def("__getitem__",
           [](const Indicator& ind, const string& d) { return ind.getByDate(Datetime(d)); })

      .def("__getitem__",
           [](const Indicator& ind, py::slice slice) {
               PriceList result;
               size_t start, stop, step, slicelength;
               if (!slice.compute(ind.size(), &start, &stop, &step, &slicelength)) {
                   throw py::error_already_set();
               }

               result.reserve((size_t)slicelength);
               for (size_t i = 0; i < slicelength; ++i) {
                   result.push_back(ind.get(start));
                   start += step;
               }

               return vector_to_python_list(result);
           })

        DEF_PICKLE(Indicator);
}