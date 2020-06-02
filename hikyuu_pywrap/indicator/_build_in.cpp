/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-31
 *      Author: fasiondog
 */

#include <hikyuu/indicator/build_in.h>
#include <pybind11/pybind11.h>
#include "../StockType.h"
#include "../convert_Datetime.h"
#include "../pybind_utils.h"
#include "../pickle_support.h"

using namespace hku;
namespace py = pybind11;

void export_Indicator_build_in(py::module& m) {
    m.def("ABS", py::overload_cast<void>(ABS));
    m.def("ABS", py::overload_cast<price_t>(ABS), py::arg("data"));
    m.def("ABS", py::overload_cast<const Indicator&>(ABS), py::arg("data"), R"(ABS([data])

    求绝对值

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("ACOS", py::overload_cast<void>(ACOS));
    m.def("ACOS", py::overload_cast<price_t>(ACOS), py::arg("data"));
    m.def("ACOS", py::overload_cast<const Indicator&>(ACOS), py::arg("data"), R"(ACOS([data])

    反余弦值

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("AD", py::overload_cast<void>(AD));
    m.def("AD", py::overload_cast<const KData&>(AD), R"(AD(kdata)

   累积/派发线
   
   :param KData kdata: k线数据
   :rtype: Indicator)");

    m.def(
      "ADVANCE",
      [](const KQuery& query = KQueryByIndex(-100), const string& market = "SH",
         StockType stk_type = StockType::A,
         bool ignore_context = false) { return ADVANCE(query, market, stk_type, ignore_context); },
      py::arg("query") = KQueryByIndex(-100), py::arg("market") = "SH",
      py::arg("stk_type") = StockType::A, py::arg("ignore_context") = false,
      R"(ADVANCE([query=Query(-100), market='SH', stk_type=StockType.A])
      
      上涨家数。当存在指定上下文且 ignore_context 为 false 时，将忽略 query, market, stk_type 参数。

      :param Query query: 查询条件
      :param str market: 所属市场，等于 "" 时，获取所有市场
      :param StockType stk_type: 证券类型, 等于 StockType.INVALID 时，获取所有类型证券
      :param bool ignore_context: 是否忽略上下文。忽略时，强制使用 query, market, stk_type 参数。)");

    m.def("ALIGN", py::overload_cast<const Indicator&, const Indicator&>(ALIGN), py::arg("data"),
          py::arg("ref"));
    m.def("ALIGN", py::overload_cast<const Indicator&, const KData&>(ALIGN), py::arg("data"),
          py::arg("ref"));
    m.def(
      "ALIGN",
      [](const py::list& ref) {
          auto date_list = python_list_to_vector<Datetime>(ref);
          return ALIGN(date_list);
      },
      py::arg("ref"));
    m.def(
      "ALIGN",
      [](const Indicator& ind, const py::list& ref) {
          auto date_list = python_list_to_vector<Datetime>(ref);
          return ALIGN(ind, date_list);
      },
      py::arg("data"), py::arg("ref"),
      R"(ALIGN(data, ref):

    按指定的参考日期对齐

    :param Indicator data: 输入数据
    :param ref: 指定做为日期参考的 datetime list、Indicator 或 KData
    :retype: Indicator)");

    m.def("AMA", py::overload_cast<int, int, int>(AMA), py::arg("n") = 10, py::arg("fast_n") = 2,
          py::arg("slow_n") = 30);
    m.def("AMA", py::overload_cast<const Indicator&, int, int, int>(AMA), py::arg("data"),
          py::arg("n") = 10, py::arg("fast_n") = 2, py::arg("slow_n") = 30,
          R"(AMA([data, n=10, fast_n=2, slow_n=30])

    佩里.J 考夫曼（Perry J.Kaufman）自适应移动平均 [BOOK1]_
    
    :param Indicator data: 输入数据
    :param int n: 计算均值的周期窗口，必须为大于2的整数
    :param int fast_n: 对应快速周期N
    :param int slow_n: 对应慢速EMA线的N值
    :return: Indicator
    
    * result(0): AMA
    * result(1): ER)");

    m.def("ASIN", py::overload_cast<void>(ASIN));
    m.def("ASIN", py::overload_cast<price_t>(ASIN), py::arg("data"));
    m.def("ASIN", py::overload_cast<const Indicator&>(ASIN), py::arg("data"), R"(ASIN([data])

    反正弦值

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("ATAN", py::overload_cast<void>(ATAN));
    m.def("ATAN", py::overload_cast<price_t>(ATAN));
    m.def("ATAN", py::overload_cast<const Indicator&>(ATAN), R"(ATAN([data])

    反正切值

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("ATR", py::overload_cast<int>(ATR), py::arg("n") = 14);
    m.def("ATR", py::overload_cast<const Indicator&, int>(ATR), py::arg("ind"), py::arg("n") = 14,
          R"(平均真实波幅(Average True Range)

:param Indicator ind: 待计算的源数据
:param int n: 计算均值的周期窗口，必须为大于1的整数)");

    m.def("AVEDEV", AVEDEV, py::arg("data"), py::arg("n") = 22,
          R"(AVEDEV(data[, n=22])

    平均绝对偏差，求X的N日平均绝对偏差

    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("BACKSET", py::overload_cast<int>(BACKSET), py::arg("n") = 2);
    m.def("BACKSET", py::overload_cast<const Indicator&, int>(BACKSET), py::arg("data"),
          py::arg("n") = 2, R"(BACKSET([data, n=2])

    向前赋值将当前位置到若干周期前的数据设为1。

    用法：BACKSET(X,N),X非0,则将当前位置到N周期前的数值设为1。

    例如：BACKSET(CLOSE>OPEN,2)若收阳则将该周期及前一周期数值设为1,否则为0

    :param Indicator data: 输入数据
    :param int n: N周期
    :rtype: Indicator)");

    m.def("BARSCOUNT", py::overload_cast<void>(BARSCOUNT));
    m.def("BARSCOUNT", py::overload_cast<const Indicator&>(BARSCOUNT), py::arg("data"),
          R"(BARSCOUNT([data])

    有效值周期数, 求总的周期数。

    用法：BARSCOUNT(X)第一个有效数据到当前的天数。

    例如：BARSCOUNT(CLOSE)对于日线数据取得上市以来总交易日数，对于1分钟线取得当日交易分钟数。

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("BARSLAST", py::overload_cast<void>(BARSLAST));
    m.def("BARSLAST", py::overload_cast<price_t>(BARSLAST));
    m.def("BARSLAST", py::overload_cast<const Indicator&>(BARSLAST), py::arg("data"),
          R"(BARSLAST([data])

    上一次条件成立位置 上一次条件成立到当前的周期数。

    用法：BARSLAST(X): 上一次 X 不为 0 到现在的天数。

    例如：BARSLAST(CLOSE/REF(CLOSE,1)>=1.1) 表示上一个涨停板到当前的周期数

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("BARSSINCE", py::overload_cast<void>(BARSSINCE));
    m.def("BARSSINCE", py::overload_cast<price_t>(BARSSINCE));
    m.def("BARSSINCE", py::overload_cast<const Indicator&>(BARSSINCE), py::arg("data"),
          R"(BARSSINCE([data])

    第一个条件成立位置到当前的周期数。

    用法：BARSSINCE(X):第一次X不为0到现在的天数。

    例如：BARSSINCE(HIGH>10)表示股价超过10元时到当前的周期数

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("BETWEEN", py::overload_cast<price_t, price_t, price_t>(BETWEEN), py::arg("a"),
          py::arg("b"), py::arg("c"));
    m.def("BETWEEN", py::overload_cast<price_t, price_t, const Indicator&>(BETWEEN), py::arg("a"),
          py::arg("b"), py::arg("c"));
    m.def("BETWEEN", py::overload_cast<price_t, const Indicator&, price_t>(BETWEEN), py::arg("a"),
          py::arg("b"), py::arg("c"));
    m.def("BETWEEN", py::overload_cast<const Indicator&, price_t, price_t>(BETWEEN), py::arg("a"),
          py::arg("b"), py::arg("c"));
    m.def("BETWEEN", py::overload_cast<const Indicator&, price_t, const Indicator&>(BETWEEN),
          py::arg("a"), py::arg("b"), py::arg("c"));
    m.def("BETWEEN", py::overload_cast<const Indicator&, const Indicator&, price_t>(BETWEEN),
          py::arg("a"), py::arg("b"), py::arg("c"));
    m.def("BETWEEN",
          py::overload_cast<const Indicator&, const Indicator&, const Indicator&>(BETWEEN),
          py::arg("a"), py::arg("b"), py::arg("c"), R"(BETWEEN(a, b, c)

    介于(介于两个数之间)

    用法：BETWEEN(A,B,C)表示A处于B和C之间时返回1，否则返回0

    例如：BETWEEN(CLOSE,MA(CLOSE,10),MA(CLOSE,5))表示收盘价介于5日均线和10日均线之间

    :param Indicator a: A
    :param Indicator b: B
    :param Indicator c: C
    :rtype: Indicator)");

    m.def("CEILING", py::overload_cast<void>(CEILING));
    m.def("CEILING", py::overload_cast<price_t>(CEILING), py::arg("data"));
    m.def("CEILING", py::overload_cast<const Indicator&>(CEILING), py::arg("data"),
          R"(CEILING([data])

    向上舍入 (向数值增大方向舍入)

    用法：CEILING(A)返回沿A数值增大方向最接近的整数
    例如：CEILING(12.3)求得13；CEILING(-3.5)求得-3)");

    m.def("COS", py::overload_cast<void>(COS));
    m.def("COS", py::overload_cast<price_t>(COS));
    m.def("COS", py::overload_cast<const Indicator&>(COS), py::arg("ind"), R"(COS([data])

    余弦值

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("COST", py::overload_cast<double>(COST), py::arg("x") = 10.0);
    m.def("COST", py::overload_cast<const KData&, double>(COST), py::arg("k"), py::arg("x") = 10.0,
          R"(COST(k, x)

    成本分布

    用法：COST(k, X) 表示X%获利盘的价格是多少

    例如：COST(k, 10),表示10%获利盘的价格是多少，即有10%的持仓量在该价格以下，其余90%在该价格以上，为套牢盘 该函数仅对日线分析周期有效

    :param KData k: 关联的K线数据
    :param float x: x%获利价格, 0~100
    :rtype: Indicator)");

    m.def("COUNT", py::overload_cast<int>(COUNT), py::arg("n") = 20);
    m.def("COUNT", py::overload_cast<const Indicator&, int>(COUNT), py::arg("data"),
          py::arg("n") = 20, R"(COUNT([data, n=20])

    统计满足条件的周期数。
    
    用法：COUNT(X,N),统计N周期中满足X条件的周期数,若N=0则从第一个有效值开始。
    
    例如：COUNT(CLOSE>OPEN,20)表示统计20周期内收阳的周期数
    
    :param Indicator data: 条件
    :param int n: 周期
    :rtype: Indicator)");

    m.def("CROSS", py::overload_cast<price_t, price_t>(CROSS), py::arg("x"), py::arg("y"));
    m.def("CROSS", py::overload_cast<const Indicator&, price_t>(CROSS), py::arg("x"), py::arg("y"));
    m.def("CROSS", py::overload_cast<price_t, const Indicator&>(CROSS), py::arg("x"), py::arg("y"));
    m.def("CROSS", py::overload_cast<const Indicator&, const Indicator&>(CROSS), py::arg("x"),
          py::arg("y"), R"(CROSS(x, y)

    交叉函数

    :param x: 变量或常量，判断交叉的第一条线
    :param y: 变量或常量，判断交叉的第二条线
    :rtype: Indicator)");

    m.def("CVAL", py::overload_cast<double, size_t>(CVAL), py::arg("value"),
          py::arg("discard") = 0);
    m.def("CVAL", py::overload_cast<const Indicator&, double, int>(CVAL), py::arg("data"),
          py::arg("value") = 0.0, py::arg("discard") = 0,
          R"(CVAL([data, value=0.0, discard=0])
    
    data 为 Indicator 实例，创建和 data 等长的常量指标，其值和为value，抛弃长度discard和data一样
    
    :param Indicator data: Indicator实例
    :param float value: 常数值
    :param int len: 长度
    :param int discard: 抛弃数量
    :return: Indicator)");

    m.def(
      "DECLINE",
      [](const KQuery& query = KQueryByIndex(-100), const string& market = "SH",
         StockType stk_type = StockType::A,
         bool ignore_context = false) { return DECLINE(query, market, stk_type, ignore_context); },
      py::arg("query") = KQueryByIndex(-100), py::arg("market") = "SH",
      py::arg("stk_type") = StockType::A, py::arg("ignore_context") = false,
      R"(DECLINE([query=Query(-100), market='SH', stk_type=StockType.A])

    下跌家数。当存在指定上下文且 ignore_context 为 false 时，将忽略 query, market, stk_type 参数。

    :param Query query: 查询条件
    :param str market: 所属市场，等于 "" 时，获取所有市场
    :param StockType stk_type: 证券类型, 等于 StockType.INVALID 时，获取所有类型证券
    :param bool ignore_context: 是否忽略上下文。忽略时，强制使用 query, market, stk_type 参数。)");

    m.def("DEVSQ", py::overload_cast<int>(DEVSQ), py::arg("n") = 10);
    m.def("DEVSQ", py::overload_cast<const Indicator&, int>(DEVSQ), py::arg("data"),
          py::arg("n") = 10, R"(DEVSQ([data, n=10])

    数据偏差平方和，求X的N日数据偏差平方和

    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("DIFF", py::overload_cast<void>(DIFF));
    m.def("DIFF", py::overload_cast<const Indicator&>(DIFF), py::arg("data"),
          R"(DIFF([data])

    差分指标，即data[i] - data[i-1]
    
    :param Indicator data: 输入数据
    :return: Indicator)");

    m.def("DMA", DMA, py::arg("ind"), py::arg("a"), R"(DMA(ind, a)

    动态移动平均

    用法：DMA(X,A),求X的动态移动平均。

    算法：若Y=DMA(X,A) 则 Y=A*X+(1-A)*Y',其中Y'表示上一周期Y值。

    例如：DMA(CLOSE,VOL/CAPITAL)表示求以换手率作平滑因子的平均价

    :param Indicator ind: 输入数据
    :param Indicator a: 动态系数
    :rtype: Indicator)");

    m.def("DOWNNDAY", DOWNNDAY, py::arg("ind"), py::arg("n") = 3,
          R"(DOWNNDAY(data[, n=3])

    连跌周期数, DOWNNDAY(CLOSE,M)表示连涨M个周期

    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("DROPNA", py::overload_cast<void>(DROPNA));
    m.def("DROPNA", py::overload_cast<const Indicator&>(DROPNA), R"(DROPNA([data])

    删除 nan 值

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("EMA", py::overload_cast<int>(EMA), py::arg("n") = 22);
    m.def("EMA", py::overload_cast<const Indicator&, int>(EMA), py::arg("data"), py::arg("n") = 22,
          R"(EMA([data, n=22])

    指数移动平均线(Exponential Moving Average)

    :param Indicator data: 输入数据
    :param int n: 计算均值的周期窗口，必须为大于0的整数
    :return: Indicator )");

    m.def("EVERY", py::overload_cast<int>(EVERY), py::arg("n") = 20);
    m.def("EVERY", py::overload_cast<const Indicator&, int>(EVERY), py::arg("data"),
          py::arg("n") = 20, R"(EVERY([data, n=20])

    一直存在

    用法：EVERY (X,N) 表示条件X在N周期一直存在

    例如：EVERY(CLOSE>OPEN,10) 表示前10日内一直是阳线

    :param data: 输入数据
    :param int n: 计算均值的周期窗口，必须为大于0的整数 
    :rtype: Indicator)");

    m.def("EXIST", py::overload_cast<int>(EXIST), py::arg("n") = 20);
    m.def("EXIST", py::overload_cast<const Indicator&, int>(EXIST), py::arg("ind"),
          py::arg("n") = 20, R"(存在, EXIST(X,N) 表示条件X在N周期有存在

    :param data: 输入数据
    :param int n: 计算均值的周期窗口，必须为大于0的整数 
    :rtype: Indicator)");

    m.def("EXP", py::overload_cast<void>(EXP));
    m.def("EXP", py::overload_cast<price_t>(EXP));
    m.def("EXP", py::overload_cast<const Indicator&>(EXP), R"(EXP([data])

    EXP(X)为e的X次幂

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("FILTER", py::overload_cast<int>(FILTER), py::arg("n") = 5);
    m.def("FILTER", py::overload_cast<const Indicator&, int>(FILTER), py::arg("data"),
          py::arg("n") = 5, R"(FILTER([data, n=5])

    信号过滤, 过滤连续出现的信号。

    用法：FILTER(X,N): X 满足条件后，删除其后 N 周期内的数据置为 0。

    例如：FILTER(CLOSE>OPEN,5) 查找阳线，5 天内再次出现的阳线不被记录在内。

    :param Indicator data: 输入数据
    :param int n: 过滤周期
    :rtype: Indicator)");

    m.def("FLOOR", py::overload_cast<void>(FLOOR));
    m.def("FLOOR", py::overload_cast<price_t>(FLOOR));
    m.def("FLOOR", py::overload_cast<const Indicator&>(FLOOR),
          R"(FLOOR([data])

    向下舍入(向数值减小方向舍入)取整
    
    用法：FLOOR(A)返回沿A数值减小方向最接近的整数
    
    例如：FLOOR(12.3)求得12

    :param data: 输入数据
    :rtype: Indicator)");

    m.def("HHV", py::overload_cast<int>(HHV), py::arg("n") = 20);
    m.def("HHV", py::overload_cast<const Indicator&, int>(HHV), py::arg("data"), py::arg("n") = 20,
          R"(HHV([data, n=20])

    N日内最高价, N=0则从第一个有效值开始。

    :param Indicator data: 输入数据
    :param int n: N日时间窗口
    :return: Indicator)");

    m.def("HHVBARS", py::overload_cast<int>(HHVBARS), py::arg("n") = 20);
    m.def("HHVBARS", py::overload_cast<const Indicator&, int>(HHVBARS), py::arg("ind"),
          py::arg("n") = 20,
          R"(HHVBARS([data, n=20])

    上一高点位置 求上一高点到当前的周期数。

    用法：HHVBARS(X,N):求N周期内X最高值到当前周期数N=0表示从第一个有效值开始统计

    例如：HHVBARS(HIGH,0)求得历史新高到到当前的周期数

    :param Indicator data: 输入数据
    :param int n: N日时间窗口
    :rtype: Indicator)");

    m.def("HSL", py::overload_cast<void>(HSL));
    m.def("HSL", py::overload_cast<const KData&>(HSL),
          R"(HSL(kdata)

    获取换手率，等于 VOL(k) / CAPITAL(k)
    
    :param KData kdata: k线数据
    :rtype: Indicator)");

    m.def("IF", py::overload_cast<const Indicator&, price_t, price_t>(IF), py::arg("x"),
          py::arg("a"), py::arg("b"));
    m.def("IF", py::overload_cast<const Indicator&, const Indicator&, price_t>(IF), py::arg("x"),
          py::arg("a"), py::arg("b"));
    m.def("IF", py::overload_cast<const Indicator&, price_t, const Indicator&>(IF), py::arg("x"),
          py::arg("a"), py::arg("b"));
    m.def("IF", py::overload_cast<const Indicator&, const Indicator&, const Indicator&>(IF),
          py::arg("x"), py::arg("a"), py::arg("b"), R"(IF(x, a, b)

    条件函数, 根据条件求不同的值。
    
    用法：IF(X,A,B)若X不为0则返回A,否则返回B
    
    例如：IF(CLOSE>OPEN,HIGH,LOW)表示该周期收阳则返回最高值,否则返回最低值
    
    :param Indicator x: 条件指标
    :param Indicator a: 待选指标 a
    :param Indicator b: 待选指标 b
    :rtype: Indicator)");

    m.def("INTPART", py::overload_cast<void>(INTPART));
    m.def("INTPART", py::overload_cast<price_t>(INTPART), py::arg("data"));
    m.def("INTPART", py::overload_cast<const Indicator&>(INTPART), py::arg("data"),
          R"(INTPART([data])

    取整(绝对值减小取整，即取得数据的整数部分)
    
    :param data: 输入数据
    :rtype: Indicator)");

    m.def("KDATA", py::overload_cast<void>(KDATA));
    m.def("KDATA", py::overload_cast<const KData&>(KDATA),
          R"(KDATA([data])

    包装KData成Indicator，用于其他指标计算

    :param data: KData 或 具有6个返回结果的Indicator（如KDATA生成的Indicator）
    :return: Indicator)");

    m.def("OPEN", py::overload_cast<void>(OPEN));
    m.def("OPEN", py::overload_cast<const KData&>(OPEN),
          R"(OPEN([data])

    获取开盘价，包装KData的开盘价成Indicator
    
    :param data: 输入数据（KData 或 Indicator） 
    :return: Indicator)");

    m.def("HIGH", py::overload_cast<void>(HIGH));
    m.def("HIGH", py::overload_cast<const KData&>(HIGH),
          R"(HIGH([data])

    获取最高价，包装KData的最高价成Indicator

    :param data: 输入数据（KData 或 Indicator） 
    :return: Indicator)");

    m.def("LOW", py::overload_cast<void>(LOW));
    m.def("LOW", py::overload_cast<const KData&>(LOW),
          R"(LOW([data])

    获取最低价，包装KData的最低价成Indicator
    
    :param data: 输入数据（KData 或 Indicator） 
    :return: Indicator)");

    m.def("CLOSE", py::overload_cast<void>(CLOSE));
    m.def("CLOSE", py::overload_cast<const KData&>(CLOSE),
          R"(CLOSE([data])
   
    获取收盘价，包装KData的收盘价成Indicator
    
    :param data: 输入数据（KData 或 Indicator）
    :return: Indicator)");

    m.def("AMO", py::overload_cast<void>(AMO));
    m.def("AMO", py::overload_cast<const KData&>(AMO),
          R"(AMO([data])

    获取成交金额，包装KData的成交金额成Indicator
    
    :param data: 输入数据（KData 或 Indicator）
    :return: Indicator)");

    m.def("VOL", py::overload_cast<void>(VOL));
    m.def("VOL", py::overload_cast<const KData&>(VOL),
          R"(VOL([data])

    获取成交量，包装KData的成交量成Indicator

    :param data: 输入数据（KData 或 Indicator）
    :return: Indicator)");

    m.def("KDATA_PART", py::overload_cast<const string&>(KDATA_PART));
    m.def("KDATA_PART", py::overload_cast<const KData&, const string&>(KDATA_PART), py::arg("data"),
          py::arg("kpart"),
          R"(KDATA_PART([data, kpart])

    根据字符串选择返回指标KDATA/OPEN/HIGH/LOW/CLOSE/AMO/VOL，如:KDATA_PART("CLOSE")等同于CLOSE()

    :param data: 输入数据（KData 或 Indicator） 
    :param string kpart: KDATA|OPEN|HIGH|LOW|CLOSE|AMO|VOL
    :return: Indicator)");

    m.def("LAST", py::overload_cast<int, int>(LAST), py::arg("m") = 10, py::arg("n") = 5);
    m.def("LAST", py::overload_cast<const Indicator&, int, int>(LAST), py::arg("ind"),
          py::arg("m") = 10, py::arg("n") = 5, R"(LAST([data, m=10, n=5])

    区间存在。

    用法：LAST (X,M,N) 表示条件 X 在前 M 周期到前 N 周期存在。

    例如：LAST(CLOSE>OPEN,10,5) 表示从前10日到前5日内一直阳线。

    :param data: 输入数据
    :param int m: m周期
    :param int n: n周期
    :rtype: Indicator)");

    m.def("LIUTONGPAN", py::overload_cast<void>(LIUTONGPAN));
    m.def("LIUTONGPAN", py::overload_cast<const KData&>(LIUTONGPAN), R"(LIUTONGPAN(kdata)

   获取流通盘
   
   :param KData kdata: k线数据
   :rtype: Indicator)");

    m.def("CAPITAL", py::overload_cast<void>(LIUTONGPAN));
    m.def("CAPITAL", py::overload_cast<const KData&>(LIUTONGPAN), R"(LIUTONGPAN(kdata)

   获取流通盘
   
   :param KData kdata: k线数据
   :rtype: Indicator)");

    m.def("LLV", py::overload_cast<int>(LLV), py::arg("n") = 20);
    m.def("LLV", py::overload_cast<const Indicator&, int>(LLV), py::arg("data"), py::arg("n") = 20,
          R"(LLV([data, n=20])

    N日内最低价, N=0则从第一个有效值开始。

    :param data: 输入数据
    :param int n: N日时间窗口
    :return: Indicator)");

    m.def("LLVBARS", py::overload_cast<int>(LLVBARS), py::arg("n") = 20);
    m.def("LLVBARS", py::overload_cast<const Indicator&, int>(LLVBARS), py::arg("data"),
          py::arg("n") = 20, R"(LLVBARS([data, n=20])
                
    上一低点位置 求上一低点到当前的周期数。
    用法：LLVBARS(X,N):求N周期内X最低值到当前周期数N=0表示从第一个有效值开始统计
    例如：LLVBARS(HIGH,20)求得20日最低点到当前的周期数)

    :param Indicator data: 输入数据
    :param int n: 周期窗口)");

    m.def("LN", py::overload_cast<void>(LN));
    m.def("LN", py::overload_cast<price_t>(LN));
    m.def("LN", py::overload_cast<const Indicator&>(LN), R"(LN([data])

    求自然对数, LN(X)以e为底的对数

    :param data: 输入数据
    :rtype: Indicator)");

    m.def("LOG", py::overload_cast<void>(LOG));
    m.def("LOG", py::overload_cast<price_t>(LOG));
    m.def("LOG", py::overload_cast<const Indicator&>(LOG), R"(LOG([data])

    以10为底的对数

    :param data: 输入数据
    :rtype: Indicator)");

    m.def("LONGCROSS", py::overload_cast<price_t, price_t, int>(LONGCROSS), py::arg("a"),
          py::arg("b"), py::arg("n") = 3);
    m.def("LONGCROSS", py::overload_cast<price_t, const Indicator&, int>(LONGCROSS), py::arg("a"),
          py::arg("b"), py::arg("n") = 3);
    m.def("LONGCROSS", py::overload_cast<const Indicator&, price_t, int>(LONGCROSS), py::arg("a"),
          py::arg("b"), py::arg("n") = 3);
    m.def("LONGCROSS", py::overload_cast<const Indicator&, const Indicator&, int>(LONGCROSS),
          py::arg("a"), py::arg("b"), py::arg("n") = 3, R"(LONGCROSS(a, b[, n=3])

    两条线维持一定周期后交叉

    用法：LONGCROSS(A,B,N)表示A在N周期内都小于B，本周期从下方向上穿过B时返 回1，否则返回0
    例如：LONGCROSS(MA(CLOSE,5),MA(CLOSE,10),5)表示5日均线维持5周期后与10日均线交金叉

    :param Indicator a:
    :param Indicator b:
    :param int n:)");

    m.def("MA", py::overload_cast<int>(MA), py::arg("n") = 22);
    m.def("MA", py::overload_cast<const Indicator&, int>(MA), py::arg("data"), py::arg("n") = 22,
          R"(MA([data, n=22])

    简单移动平均
    
    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("MACD", py::overload_cast<int, int, int>(MACD), py::arg("n1") = 12, py::arg("n2") = 26,
          py::arg("n3") = 9);
    m.def("MACD", py::overload_cast<const Indicator&, int, int, int>(MACD), py::arg("data"),
          py::arg("n1") = 12, py::arg("n2") = 26, py::arg("n3") = 9,
          R"(MACD([data, n1=12, n2=26, n3=9])

    平滑异同移动平均线
    
    :param Indicator data: 输入数据
    :param int n1: 短期EMA时间窗
    :param int n2: 长期EMA时间窗
    :param int n3: （短期EMA-长期EMA）EMA平滑时间窗
    :return: 具有三个结果集的 Indicator

    * result(0): MACD_BAR：MACD直柱，即MACD快线－MACD慢线
    * result(1): DIFF: 快线,即（短期EMA-长期EMA）
    * result(2): DEA: 慢线，即快线的n3周期EMA平滑)");

    m.def("MAX", py::overload_cast<const Indicator&, price_t>(MAX), py::arg("ind1"),
          py::arg("ind2"));
    m.def("MAX", py::overload_cast<price_t, const Indicator&>(MAX), py::arg("ind1"),
          py::arg("ind2"));
    m.def("MAX", py::overload_cast<const Indicator&, const Indicator&>(MAX), py::arg("ind1"),
          py::arg("ind2"), R"(MAX(ind1, ind2)

    求最大值, MAX(A,B)返回A和B中的较大值。
    
    :param Indicator ind1: A
    :param Indicator ind2: B)");

    m.def("MIN", py::overload_cast<const Indicator&, price_t>(MIN), py::arg("ind1"),
          py::arg("ind2"));
    m.def("MIN", py::overload_cast<price_t, const Indicator&>(MIN), py::arg("ind1"),
          py::arg("ind2"));
    m.def("MIN", py::overload_cast<const Indicator&, const Indicator&>(MIN), py::arg("ind1"),
          py::arg("ind2"), R"(MIN(ind1, ind2)

    求最小值, MIN(A,B)返回A和B中的较小值。
    
    :param Indicator ind1: A
    :param Indicator ind2: B)");

    m.def("MOD", py::overload_cast<price_t, price_t>(MOD));
    m.def("MOD", py::overload_cast<const Indicator&, price_t>(MOD));
    m.def("MOD", py::overload_cast<price_t, const Indicator&>(MOD));
    m.def("MOD", py::overload_cast<const Indicator&, const Indicator&>(MOD), R"(MOD(ind1, ind2)

    取整后求模。该函数仅为兼容通达信。实际上，指标求模可直接使用 % 操作符
    用法：MOD(A,B)返回A对B求模
    例如：MOD(26,10) 返回 6

    :param Indicator ind1:
    :param Indicator ind2:)");

    m.def("NDAY", NDAY, py::arg("x"), py::arg("y"), py::arg("n") = 3, R"(NDAY(x, y[, n=3])

    连大, NDAY(X,Y,N)表示条件X>Y持续存在N个周期

    :param Indicator x:
    :param Indicator y:
    :param int n: 时间窗口)");

    m.def("NOT", py::overload_cast<void>(NOT));
    m.def("NOT", py::overload_cast<const Indicator&>(NOT), R"(NOT([data])

    求逻辑非。NOT(X)返回非X,即当X=0时返回1，否则返回0。
    
    :param Indicator data: 输入数据)");

    m.def("POS", POS);

    m.def("POW", py::overload_cast<int>(POW), py::arg("n"));
    m.def("POW", py::overload_cast<price_t, int>(POW), py::arg("data"), py::arg("n"));
    m.def("POW", py::overload_cast<const Indicator&, int>(POW), py::arg("data"), py::arg("n"),
          R"(POW(data, n)

    乘幂
    用法：POW(A,B)返回A的B次幂
    例如：POW(CLOSE,3)求得收盘价的3次方
    
    :param data: 输入数据
    :param int n: 幂)");

    m.def(
      "PRICELIST",
      [](const py::list& data, int discard = 0) {
          return PRICELIST(python_list_to_vector<price_t>(data), discard);
      },
      py::arg("data"), py::arg("discard") = 0, R"(将 Python list 对象转换为 PRICELIST 指标
      
    :param data 源数据
    :param discard 前端抛弃的数据点数，抛弃的值使用 nan 填充)");

    m.def("PRICELIST", py::overload_cast<const Indicator&, int>(PRICELIST), py::arg("ind"),
          py::arg("result_index") = 0, R"(将某指标转化为PRICELIST
    :param Indicator ind: 源数据
    :param int result_index: 源数据中指定的结果集)");

    m.def("PRICELIST", py::overload_cast<int>(PRICELIST), py::arg("result_index") = 0);

    m.def("REF", py::overload_cast<int>(REF));
    m.def("REF", py::overload_cast<const Indicator&, int>(REF), R"(REF([data, n])

    向前引用 （即右移），引用若干周期前的数据。
    用法：REF(X，A)　引用A周期前的X值。
    
    :param Indicator data: 输入数据
    :param int n: 引用n周期前的值，即右移n位)");

    m.def("REVERSE", py::overload_cast<void>(REVERSE));
    m.def("REVERSE", py::overload_cast<price_t>(REVERSE));
    m.def("REVERSE", py::overload_cast<const Indicator&>(REVERSE), R"(REVERSE([data])

    求相反数，REVERSE(X)返回-X

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("ROC", py::overload_cast<int>(ROC), py::arg("n") = 10);
    m.def("ROC", py::overload_cast<const Indicator&, int>(ROC), py::arg("data"), py::arg("n") = 10,
          R"(ROC([data, n=10])

    变动率指标: ((price / prevPrice)-1)*100

    :param data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("ROCP", py::overload_cast<int>(ROCP), py::arg("n") = 10);
    m.def("ROCP", py::overload_cast<const Indicator&, int>(ROCP), py::arg("data"),
          py::arg("n") = 10, R"(ROCP([data, n=10])

    变动率指标: (price - prevPrice) / prevPrice

    :param data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("ROCR", py::overload_cast<int>(ROCR), py::arg("n") = 10);
    m.def("ROCR", py::overload_cast<const Indicator&, int>(ROCR), py::arg("data"),
          py::arg("n") = 10, R"(ROCR([data, n=10])

    变动率指标: (price / prevPrice)

    :param data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("ROCR100", py::overload_cast<int>(ROCR100), py::arg("n") = 10);
    m.def("ROCR100", py::overload_cast<const Indicator&, int>(ROCR100), py::arg("data"),
          py::arg("n") = 10, R"(ROCR([data, n=10])

    变动率指标: (price / prevPrice) * 100

    :param data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("ROUND", py::overload_cast<int>(ROUND), py::arg("ndigits") = 2);
    m.def("ROUND", py::overload_cast<price_t, int>(ROUND), py::arg("data"), py::arg("ndigits") = 2);
    m.def("ROUND", py::overload_cast<const Indicator&, int>(ROUND), py::arg("data"),
          py::arg("ndigits") = 2, R"(ROUND([data, ndigits=2])

    四舍五入

    :param data: 输入数据
    :param int ndigits: 保留的小数点后位数
    :rtype: Indicator)");

    m.def("ROUNDDOWN", py::overload_cast<int>(ROUNDDOWN), py::arg("ndigits") = 2);
    m.def("ROUNDDOWN", py::overload_cast<price_t, int>(ROUNDDOWN), py::arg("data"),
          py::arg("ndigits") = 2);
    m.def("ROUNDDOWN", py::overload_cast<const Indicator&, int>(ROUNDDOWN), py::arg("data"),
          py::arg("ndigits") = 2, R"(ROUNDDOWN([data, ndigits=2])

    向下截取，如10.1截取后为10

    :param data: 输入数据
    :param int ndigits: 保留的小数点后位数
    :rtype: Indicator)");

    m.def("ROUNDUP", py::overload_cast<int>(ROUNDUP), py::arg("ndigits") = 2);
    m.def("ROUNDUP", py::overload_cast<price_t, int>(ROUNDUP), py::arg("data"),
          py::arg("ndigits") = 2);
    m.def("ROUNDUP", py::overload_cast<const Indicator&, int>(ROUNDUP), py::arg("data"),
          py::arg("ndigits") = 2, R"(ROUNDUP([data, ndigits=2])

    向上截取，如10.1截取后为11

    :param data: 输入数据
    :param int ndigits: 保留的小数点后位数
    :rtype: Indicator)");

    m.def("SAFTYLOSS", py::overload_cast<int, int, double>(SAFTYLOSS), py::arg("n1") = 10,
          py::arg("n2") = 3, py::arg("p") = 2.0);
    m.def("SAFTYLOSS", py::overload_cast<const Indicator&, int, int, double>(SAFTYLOSS),
          py::arg("data"), py::arg("n1") = 10, py::arg("n2") = 3, py::arg("p") = 2.0,
          R"(SAFTYLOSS([data, n1=10, n2=3, p=2.0])

    亚历山大 艾尔德安全地带止损线，参见 [BOOK2]_
    
    计算说明：在回溯周期内（一般为10到20天），将所有向下穿越的长度相加除以向下穿越的次数，得到噪音均值（即回溯期内所有最低价低于前一日最低价的长度除以次数），并用今日最低价减去（前日噪音均值乘以一个倍数）得到该止损线。为了抵消波动并且保证止损线的上移，在上述结果的基础上再取起N日（一般为3天）内的最高值

    :param Indicator data: 输入数据
    :param int n1: 计算平均噪音的回溯时间窗口
    :param int n2: 对初步止损线去n2日内的最高值
    :param float p: 噪音系数
    :return: Indicator)");

    m.def("SGN", py::overload_cast<void>(SGN));
    m.def("SGN", py::overload_cast<price_t>(SGN), py::arg("data"));
    m.def("SGN", py::overload_cast<const Indicator&>(SGN), py::arg("data"), R"(SGN([data])

    求符号值, SGN(X)，当 X>0, X=0, X<0分别返回 1, 0, -1。

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("SIN", py::overload_cast<void>(SIN));
    m.def("SIN", py::overload_cast<price_t>(SIN), py::arg("data"));
    m.def("SIN", py::overload_cast<const Indicator&>(SIN), py::arg("data"), R"(SIN([data])

    正弦值

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("SMA", py::overload_cast<int, double>(SMA), py::arg("n") = 22, py::arg("m") = 2.0);
    m.def("SMA", py::overload_cast<const Indicator&, int, double>(SMA), py::arg("data"),
          py::arg("n") = 22, py::arg("m") = 2.0, R"(SMA([data, n=22, m=2])

    求移动平均

    用法：若Y=SMA(X,N,M) 则 Y=[M*X+(N-M)*Y')/N,其中Y'表示上一周期Y值
    
    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :param float m: 系数
    :rtype: Indicator)");

    m.def("SQRT", py::overload_cast<void>(SQRT));
    m.def("SQRT", py::overload_cast<price_t>(SQRT), py::arg("data"));
    m.def("SQRT", py::overload_cast<const Indicator&>(SQRT), py::arg("data"), R"(SQRT([data])

    开平方

    用法：SQRT(X)为X的平方根
    例如：SQRT(CLOSE)收盘价的平方根

    :param data: 输入数据
    :rtype: Indicator)");

    m.def("STDEV", py::overload_cast<int>(STDEV), py::arg("n") = 10);
    m.def("STDEV", py::overload_cast<const Indicator&, int>(STDEV), py::arg("data"),
          py::arg("n") = 10, R"(STDEV([data, n=10])

    计算N周期内样本标准差
    
    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :return: Indicator)");

    m.def("STD", py::overload_cast<int>(STDEV), py::arg("n") = 10);
    m.def("STD", py::overload_cast<const Indicator&, int>(STDEV), py::arg("data"),
          py::arg("n") = 10, R"(STDEV([data, n=10])

    计算N周期内样本标准差
    
    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :return: Indicator)");

    m.def("STDP", py::overload_cast<int>(STDP), py::arg("n") = 10);
    m.def("STDP", py::overload_cast<const Indicator&, int>(STDP), py::arg("data"),
          py::arg("n") = 10, R"(STDP([data, n=10])

    总体标准差，STDP(X,N)为X的N日总体标准差
    
    :param data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("SUM", py::overload_cast<int>(SUM), py::arg("n") = 20);
    m.def("SUM", py::overload_cast<const Indicator&, int>(SUM), py::arg("data"), py::arg("n") = 20,
          R"(SUM([data, n=20])

    求总和。SUM(X,N),统计N周期中X的总和,N=0则从第一个有效值开始。

    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("SUMBARS", py::overload_cast<double>(SUMBARS), py::arg("a"));
    m.def("SUMBARS", py::overload_cast<const Indicator&, double>(SUMBARS), py::arg("data"),
          py::arg("a"), R"(SUMBARS([data,] a)

    累加到指定周期数, 向前累加到指定值到现在的周期数

    用法：SUMBARS(X,A):将X向前累加直到大于等于A,返回这个区间的周期数

    例如：SUMBARS(VOL,CAPITAL)求完全换手到现在的周期数

    :param Indicator data: 输入数据
    :param float a: 指定累加和
    :rtype: Indicator)");

    m.def("TAN", py::overload_cast<void>(TAN));
    m.def("TAN", py::overload_cast<price_t>(TAN), py::arg("data"));
    m.def("TAN", py::overload_cast<const Indicator&>(TAN), py::arg("data"), R"(TAN([data])

    正切值

    :param Indicator data: 输入数据
    :rtype: Indicator)");

    m.def("TIMELINE", py::overload_cast<void>(TIMELINE));
    m.def("TIMELINE", py::overload_cast<const KData&>(TIMELINE), py::arg("k"), R"(TIMELINE([k])

    分时价格数据

    :param KData k: 上下文
    :rtype: Indicator)");

    m.def("TIMELINEVOL", py::overload_cast<void>(TIMELINEVOL));
    m.def("TIMELINEVOL", py::overload_cast<const KData&>(TIMELINEVOL), py::arg("k"),
          R"(TIMELINEVOL([k])

    分时成交量数据

    :param KData k: 上下文
    :rtype: Indicator)");

    m.def("UPNDAY", UPNDAY, py::arg("data"), py::arg("n") = 3, R"(UPNDAY(data[, n=3])

    连涨周期数, UPNDAY(CLOSE,M)表示连涨M个周期

    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("VAR", py::overload_cast<int>(VAR), py::arg("n") = 10);
    m.def("VAR", py::overload_cast<const Indicator&, int>(VAR), py::arg("data"), py::arg("n") = 10,
          R"(VAR([data, n=10])

    估算样本方差, VAR(X,N)为X的N日估算样本方差

    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("VARP", py::overload_cast<int>(VARP), py::arg("n") = 10);
    m.def("VARP", py::overload_cast<const Indicator&, int>(VARP), py::arg("data"),
          py::arg("n") = 10, R"(VARP([data, n=10])

    总体样本方差, VARP(X,N)为X的N日总体样本方差

    :param Indicator data: 输入数据
    :param int n: 时间窗口
    :rtype: Indicator)");

    m.def("VIGOR", py::overload_cast<int>(VIGOR), py::arg("n") = 2);
    m.def("VIGOR", py::overload_cast<const KData&, int>(VIGOR), py::arg("data"), py::arg("n") = 2,
          R"(VIGOR([kdata, n=2])

    亚历山大.艾尔德力度指数 [BOOK2]_
    
    计算公式：（收盘价今－收盘价昨）＊成交量今
    
    :param KData data: 输入数据
    :param int n: EMA平滑窗口
    :return: Indicator)");

    m.def("WEAVE", WEAVE, R"(WEAVE(ind1, ind2)

    将ind1和ind2的结果组合在一起放在一个Indicator中。如ind = WEAVE(ind1, ind2), 则此时ind包含多个结果，按ind1、ind2的顺序存放。
    
    :param Indicator ind1: 指标1
    :param Indicator ind2: 指标2
    :rtype: Indicator)");
}