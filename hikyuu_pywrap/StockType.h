/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-27
 *      Author: fasiondog
 */

#pragma once
#ifndef HIKYUU_PYTHON_STOCK_TYPE_H
#define HIKYUU_PYTHON_STOCK_TYPE_H

#include <hikyuu/StockTypeInfo.h>

namespace hku {

enum StockType {
    BLOCK = STOCKTYPE_BLOCK,
    A = STOCKTYPE_A,
    INDEX = STOCKTYPE_INDEX,
    B = STOCKTYPE_B,
    FUND = STOCKTYPE_FUND,
    ETF = STOCKTYPE_ETF,
    ND = STOCKTYPE_ND,
    BOND = STOCKTYPE_BOND,
    GEM = STOCKTYPE_GEM,
    BTC = STOCKTYPE_BTC,
    TMP = STOCKTYPE_TMP
};

}  // namespace hku

#endif  // HIKYUU_PYTHON_STOCK_TYPE_H