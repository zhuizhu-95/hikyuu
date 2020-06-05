#!/usr/bin/python
# -*- coding: utf8 -*-
# cp936
#
# The MIT License (MIT)
#
# Copyright (c) 2010-2017 fasiondog
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

__copyright__ = """
MIT License

Copyright (c) 2010-2017 fasiondog

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import os
from datetime import *
from .cpp.core import *
from .pyindicator import *
from .talib_wrap import *
from .draw import *


# ------------------------------------------------------------------
# 定义 KData, Indicator 迭代器
# ------------------------------------------------------------------
def KData_iter(kdata):
    for i in range(len(kdata)):
        yield kdata[i]


def indicator_iter(indicator):
    for i in range(len(indicator)):
        yield indicator[i]


KData.__iter__ = KData_iter
Indicator.__iter__ = indicator_iter

# ------------------------------------------------------------------
# 数据初始化
# ------------------------------------------------------------------
#config_file = os.path.expanduser('~') + "/.hikyuu/hikyuu.ini"
config_file = "./test_data/hikyuu_win.ini"
hikyuu_init(config_file)

sm = StockManager.instance()

# ------------------------------------------------------------------
# 引入blocka、blocksh、blocksz、blockg全局变量，便于交互式环境下使用
# ------------------------------------------------------------------

blocka = Block("A", "ALL")
for s in sm:
    if s.type in (StockType.A, StockType.GEM):
        blocka.add(s)
zsbk_a = blocka

blocksh = Block("A", "SH")
for s in blocka:
    if s.market == "SH":
        blocksh.add(s)
zsbk_sh = blocksh

blocksz = Block("A", "SZ")
for s in blocka:
    if s.market == "SZ":
        blocksz.add(s)
zsbk_sz = blocksz

blockg = Block("G", "创业板")
for s in sm:
    if s.type == StockType.GEM:
        blockg.add(s)
zsbk_cyb = blockg

blockzxb = Block("A", "中小板")
for s in blocksz:
    if s.code[:3] == "002":
        blockzxb.add(s)
zsbk_zxb = blockzxb

zsbk_sz50 = sm.get_block("指数板块", "上证50")
zsbk_sz180 = sm.get_block("指数板块", "上证180")
zsbk_hs300 = sm.get_block("指数板块", "沪深300")
zsbk_zz100 = sm.get_block("指数板块", "沪深300")

# ------------------------------------------------------------------
# 设置关键类型简称
# ------------------------------------------------------------------

O = OPEN()
C = CLOSE()
H = HIGH()
L = LOW()
A = AMO()
V = VOL()
D = datetime
K = None
Q = Query


def set_global_context(stk, query):
    """设置全局的 context

        :param Stock stk: 指定的全局Stock
        :param Query query: 指定的查询条件
    """
    global K, O, C, H, L, A, V
    K = stk.get_kdata(query)
    O.set_context(K)
    C.set_context(K)
    H.set_context(K)
    L.set_context(K)
    A.set_context(K)
    V.set_context(K)


def get_global_context():
    """获取当前的全局上下文

        :rtype: KData
    """
    return C.set_context()


set_global_context(sm['sh000001'], Query(-150))

# ------------------------------------------------------------------
# 设置默认绘图引擎
# ------------------------------------------------------------------

use_draw_engine('matplotlib')

# ------------------------------------------------------------------
# 包版本及文档信息
# ------------------------------------------------------------------

__version__ = get_version()

__doc__ = """
Hikyuu Quant Framework is a high-performance open-source quantitative trading 
research framework based on C++.
\nsee more: http://hikyuu.org"""