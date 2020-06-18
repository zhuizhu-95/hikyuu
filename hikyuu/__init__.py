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
import configparser

from datetime import date, datetime, timedelta
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
# 读取配置信息，并初始化
# ------------------------------------------------------------------
#config_file = "./test_data/hikyuu_win.ini"
#hikyuu_init(config_file)
#sm = StockManager.instance()
config_file = os.path.expanduser('~') + "/.hikyuu/hikyuu.ini"
if not os.path.exists(config_file):
    # 检查老版本配置是否存在，如果存在可继续使用，否则异常终止
    data_config_file = os.path.expanduser('~') + "/.hikyuu/data_dir.ini"
    data_config = configparser.ConfigParser()
    data_config.read(data_config_file)
    data_dir = data_config['data_dir']['data_dir']
    if sys.platform == 'win32':
        config_file = data_dir + "\\hikyuu_win.ini"
    else:
        config_file = data_dir + "/hikyuu_linux.ini"
    if not os.path.exists(config_file):
        raise ("未找到配置文件，请先使用数据导入工具导入数据（将自动生成配置文件）！！!")

ini = configparser.ConfigParser()
ini.read(config_file)
hku_param = Parameter()
hku_param.set("tmpdir", ini.get('hikyuu', 'tmpdir'))
if ini.has_option('hikyuu', 'logger'):
    hku_param.set("logger", ini['hikyuu']['logger'])

base_param = Parameter()
base_info_config = ini.options('baseinfo')
for p in base_info_config:
    base_param.set(p, ini.get('baseinfo', p))

block_param = Parameter()
block_config = ini.options('block')
for p in block_config:
    block_param.set(p, ini.get('block', p))

preload_param = Parameter()
preload_config = ini.options('preload')
for p in preload_config:
    # 注意：proload参数是布尔类型
    preload_param.set(p, ini.getboolean('preload', p))

kdata_param = Parameter()
kdata_config = ini.options('kdata')
for p in kdata_config:
    kdata_param.set(p, ini.get('kdata', p))

set_log_level(LOG_LEVEL.TRACE)
sm = StockManager.instance()
sm.init(base_param, block_param, kdata_param, preload_param, hku_param)
set_log_level(LOG_LEVEL.WARN)

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

zsbk_sz50 = sm.getBlock("指数板块", "上证50")
zsbk_sz180 = sm.getBlock("指数板块", "上证180")
zsbk_hs300 = sm.getBlock("指数板块", "沪深300")
zsbk_zz100 = sm.getBlock("指数板块", "沪深300")

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
    K = stk.getKData(query)
    O.setContext(K)
    C.setContext(K)
    H.setContext(K)
    L.setContext(K)
    A.setContext(K)
    V.setContext(K)


def get_global_context():
    """获取当前的全局上下文

        :rtype: KData
    """
    return C.setContext()


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