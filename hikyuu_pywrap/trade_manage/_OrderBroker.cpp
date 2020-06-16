/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-11
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <hikyuu/trade_manage/OrderBrokerBase.h>
#include "../convert_Datetime.h"
#include "../convert_any.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PyOrderBrokerBase : public OrderBrokerBase {
public:
    using OrderBrokerBase::OrderBrokerBase;

    Datetime _buy(Datetime datetime, const string& market, const string& code, price_t price,
                  double num) override {
        PYBIND11_OVERLOAD_PURE(Datetime, OrderBrokerBase, _buy, datetime, market, code, price, num);
    }

    Datetime _sell(Datetime datetime, const string& market, const string& code, price_t price,
                   double num) override {
        PYBIND11_OVERLOAD_PURE(Datetime, OrderBrokerBase, datetime, market, code, price, num);
    }
};

void export_OrderBroker(py::module& m) {
    py::class_<OrderBrokerBase, OrderBrokerPtr, PyOrderBrokerBase>(
      m, "OrderBrokerBase",
      R"(Python中非必须使用 OrderBrokerBase 来实现自定义的订单代理。只要 Python 的对象
    包含 buy、sell方法，其方法参数规则与 :py:class:`OrderBrokerWrap` 中的 _buy、_sell 方法相同。

    自定义订单代理接口：

        :py:meth:`OrderBrokerBase._buy` - 【必须】执行实际买入操作
        :py:meth:`OrderBrokerBase._sell` - 【必须】执行实际卖出操作)")

      .def(py::init<>())
      .def(py::init<const string&>(), R"(
    :param str name: 代理名称)")

      .def("__str__", to_py_str<OrderBrokerBase>)
      .def("__repr__", to_py_str<OrderBrokerBase>)

      .def_property("name", py::overload_cast<void>(&OrderBrokerBase::name, py::const_),
                    py::overload_cast<const string&>(&OrderBrokerBase::name), "名称（可读写）")

      .def("buy", &OrderBrokerBase::buy, R"(buy(self, code, price, num)

    执行买入操作
    
    :param str code: 证券代码
    :param float price: 买入价格
    :param int num: 买入数量
    :return: 买入操作的执行时刻
    :rtype: datetime)")

      .def("sell", &OrderBrokerBase::sell, R"(sell(self, code, price, num)
    
    执行卖出操作
    
    :param str code: 证券代码
    :param float price: 卖出价格
    :param int num: 卖出数量
    :return: 卖出操作的执行时刻
    :rtype: datetime)")

      .def("_buy", &OrderBrokerBase::_buy, R"(_buy(self, code, price, num)

    【重载接口】执行实际买入操作
    
    :param str code: 证券代码
    :param float price: 买入价格
    :param int num: 买入数量
    :rtype: datetime)")

      .def("_sell", &OrderBrokerBase::_sell, R"(_sell(self, code, price, num)
    
    【重载接口】执行实际卖出操作
    
    :param str code: 证券代码
    :param float price: 卖出价格
    :param int num: 卖出数量
    :rtype: datetime)");
}
