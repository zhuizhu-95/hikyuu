import os

from datetime import *
from hikyuu import *

#config_file = os.path.expanduser('~') + "/.hikyuu/hikyuu.ini"
config_file = "./test_data/hikyuu_win.ini"
hikyuu_init(config_file)

sm = StockManager.instance()