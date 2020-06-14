/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-13
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/trade_sys/signal/build_in.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

class PySignalBase : public SignalBase {
    PY_CLONE(PySignalBase, SignalBase)

public:
    using SignalBase::SignalBase;

    void _calculate() override {
        PYBIND11_OVERLOAD_PURE(void, SignalBase, _calculate, );
    }

    void _reset() override {
        PYBIND11_OVERLOAD(void, SignalBase, _reset, );
    }
};

void export_Signal(py::module& m) {
    py::class_<SignalBase, SGPtr, PySignalBase>(
      m, "SignalBase", R"(信号指示器负责产生买入、卖出信号。

    公共参数：

        alternate (bool|True) ：买入和卖出信号是否交替出现。单线型的信号通常通过拐点、
                                斜率等判断信号的产生，此种情况下可能出现连续出现买入
                                信号或连续出现卖出信号的情况，此时可通过该参数控制买入、
                                卖出信号是否交替出现。而双线交叉型的信号通常本身买入和
                                卖出已经是交替出现，此时该参数无效。
                                
    自定义的信号指示器接口：

        SignalBase._calculate() - 【必须】子类计算接口
        SignalBase._clone() - 【必须】克隆接口
        SignalBase._reset() - 【可选】重载私有变量)")

      .def(py::init<>())
      .def(py::init<const string&>(), R"(初始化构造函数
        
    :param str name: 名称)")

      .def("__str__", to_py_str<SignalBase>)
      .def("__repr__", to_py_str<SignalBase>)

      .def_property("name", py::overload_cast<void>(&SignalBase::name, py::const_),
                    py::overload_cast<const string&>(&SignalBase::name), "名称")
      .def_property("to", &SignalBase::getTO, &SignalBase::setTO, "关联交易对象")

      .def("getTO", &SignalBase::getTO, "获取交易对象")
      .def("setTO", &SignalBase::setTO, "设置交易对象")

      .def("getParam", &SignalBase::getParam<boost::any>, "获取指定的参数")
      .def("setParam", &SignalBase::setParam<boost::any>, "设置参数值")
      .def("haveParam", &SignalBase::haveParam, "参数是否存在")

      .def("shouldBuy", &SignalBase::shouldBuy, R"(指定时刻是否可以买入
    
    :param Datetime datetime: 指定时刻
    :rtype: bool)")

      .def("shouldSell", &SignalBase::shouldSell, R"(指定时刻是否可以卖出
        
    :param Datetime datetime: 指定时刻
    :rtype: bool)")

      .def(
        "getBuySignal",
        [](const SignalBase& sg) { return vector_to_python_list(sg.getBuySignal()); },
        R"(获取所有买入指示日期列表)")

      .def(
        "getSellSignal",
        [](const SignalBase& sg) { return vector_to_python_list(sg.getSellSignal()); },
        R"(获取所有卖出指示日期列表)")

      .def("_addBuySignal", &SignalBase::_addBuySignal,
           R"(加入买入信号，在_calculate中调用
        
    :param datetime: 指示买入的日期)")

      .def("_addSellSignal", &SignalBase::_addSellSignal,
           R"(加入卖出信号，在_calculate中调用

    :param datetime: 指示卖出的日期)")

      .def("reset", &SignalBase::reset, "复位操作")
      .def("clone", &SignalBase::clone, "克隆操作")

      .def("_calculate", &SignalBase::_calculate, R"(【重载接口】子类计算接口)")

      .def("_reset", &SignalBase::_reset, R"(【重载接口】子类复位接口，复位内部私有变量)")

        DEF_PICKLE(SGPtr);

    m.def(
      "SG_Bool", SG_Bool, py::arg("buy"), py::arg("sell"), py::arg("kpart") = "CLOSE",
      R"(布尔信号指示器，使用运算结果为类似bool数组的Indicator分别作为买入、卖出指示。
    
    :param Indicator buy: 买入指示（结果Indicator中相应位置>0则代表买入）
    :param Indicator sell: 卖出指示（结果Indicator中相应位置>0则代表卖出）
    :param str kpart: KDATA|OPEN|HIGH|LOW|CLOSE|AMO|VOL
    :return: 信号指示器)");

    m.def(
      "SG_Single", SG_Single, py::arg("ind"), py::arg("filter_n") = 10, py::arg("filter_p") = 0.1,
      py::arg("kpart") = "CLOSE",
      R"(生成单线拐点信号指示器。使用《精明交易者》 [BOOK1]_ 中给出的曲线拐点算法判断曲线趋势，公式见下::

        filter = percentage * STDEV((AMA-AMA[1], N)

        Buy  When AMA - AMA[1] > filter
        or Buy When AMA - AMA[2] > filter
        or Buy When AMA - AMA[3] > filter 
    
    :param Indicator ind:
    :param int filer_n: N日周期
    :param float filter_p: 过滤器百分比
    :param str kpart: KDATA|OPEN|HIGH|LOW|CLOSE|AMO|VOL
    :return: 信号指示器)");

    m.def("SG_Single2", SG_Single2, py::arg("ind"), py::arg("filter_n") = 10,
          py::arg("filter_p") = 0.1, py::arg("kpart") = "CLOSE",
          R"(生成单线拐点信号指示器2 [BOOK1]_::

        filter = percentage * STDEV((AMA-AMA[1], N)

        Buy  When AMA - @lowest(AMA,n) > filter
        Sell When @highest(AMA, n) - AMA > filter
    
    :param Indicator ind:
    :param int filer_n: N日周期
    :param float filter_p: 过滤器百分比
    :param str kpart: KDATA|OPEN|HIGH|LOW|CLOSE|AMO|VOL
    :return: 信号指示器)");

    m.def(
      "SG_Cross", SG_Cross, py::arg("fast"), py::arg("slow"), py::arg("kpart") = "CLOSE",
      R"(双线交叉指示器，当快线从下向上穿越慢线时，买入；当快线从上向下穿越慢线时，卖出。如：5日MA上穿10日MA时买入，5日MA线下穿MA10日线时卖出:: 

        SG_Cross(OP(MA(n=10)), OP(MA(n=30)))

    :param Indicator fast: 快线
    :param Indicator slow: 慢线
    :param str kpart: OPEN|HIGH|LOW|CLOSE|AMO|VOL|KDATA
    :return: 信号指示器)");

    m.def(
      "SG_CrossGold", SG_CrossGold, py::arg("fast"), py::arg("slow"), py::arg("kpart") = "CLOSE",
      R"(金叉指示器，当快线从下向上穿越慢线且快线和慢线的方向都是向上时为金叉，买入；
    当快线从上向下穿越慢线且快线和慢线的方向都是向下时死叉，卖出。::
    
        SG_CrossGold(OP(MA(n=10)), OP(MA(n=30)))
    
    :param Indicator fast: 快线
    :param Indicator slow: 慢线
    :param str kpart: OPEN|HIGH|LOW|CLOSE|AMO|VOL|KDATA
    :return: 信号指示器  )");

    m.def(
      "SG_Flex", SG_Flex, py::arg("op"), py::arg("slow_n"), py::arg("kpart") = "CLOSE",
      R"(使用自身的EMA(slow_n)作为慢线，自身作为快线，快线向上穿越慢线买入，快线向下穿越慢线卖出。

    :param Indicator ind:
    :param int slow_n: 慢线EMA周期
    :param str kpart: KDATA|OPEN|HIGH|LOW|CLOSE|AMO|VOL
    :return: 信号指示器)");
}