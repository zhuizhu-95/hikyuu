import os

from datetime import *
from hikyuu import *

# ------------------------------------------------------------------
# 常量定义，各种C++中Null值
# ------------------------------------------------------------------
constant = Constant()

#config_file = os.path.expanduser('~') + "/.hikyuu/hikyuu.ini"
config_file = "./test_data/hikyuu_win.ini"
hikyuu_init(config_file)

sm = StockManager.instance()

# ==============================================================================
#
# 引入blocka、blocksh、blocksz、blockg全局变量，便于交互式环境下使用
#
# ==============================================================================

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
