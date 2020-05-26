/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-25
 *      Author: fasiondog
 */

#pragma once
#ifndef HIKYUU_PYTHON_CONVERT_DATETIME_H
#define HIKYUU_PYTHON_CONVERT_DATETIME_H

#include <pybind11/pybind11.h>
#include <hikyuu/datetime/Datetime.h>
#include "pybind_utils.h"
#include <datetime.h>

using namespace hku;

namespace pybind11 {
namespace detail {

template <>
struct type_caster<Datetime> {
public:
    PYBIND11_TYPE_CASTER(Datetime, _("datetime.datetime"));

    /**
     * Conversion part 1 (Python->C++)
     */
    bool load(handle source, bool) {
        if (source.is_none()) {
            value = Datetime();
            return true;
        }

        if (!PyDateTimeAPI) {
            PyDateTime_IMPORT;
        }

        PyObject* src = source.ptr();

        long year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, microsecond = 0;
        if (PyDateTime_Check(src)) {
            second = PyDateTime_DATE_GET_SECOND(src);
            minute = PyDateTime_DATE_GET_MINUTE(src);
            hour = PyDateTime_DATE_GET_HOUR(src);
            day = PyDateTime_GET_DAY(src);
            month = PyDateTime_GET_MONTH(src);
            year = PyDateTime_GET_YEAR(src);
            microsecond = PyDateTime_DATE_GET_MICROSECOND(src);

        } else if (PyDate_Check(src)) {
            day = PyDateTime_GET_DAY(src);
            month = PyDateTime_GET_MONTH(src);
            year = PyDateTime_GET_YEAR(src);

        } else if (PyTime_Check(src)) {
            second = PyDateTime_TIME_GET_SECOND(src);
            minute = PyDateTime_TIME_GET_MINUTE(src);
            hour = PyDateTime_TIME_GET_HOUR(src);
            day = 1;      // This date (day, month, year) = (1, 0, 70)
            month = 1;    // represents 1-Jan-1940, which is the first
            year = 1400;  // earliest available date for Datetime, not Python datetime
            microsecond = PyDateTime_TIME_GET_MICROSECOND(src);

        } else {
            return false;
        }

        // Datetime 最小只到 1400年 1 月 1日，最大只到 9999 年 12月 31 日 0点
        if (year < 1400) {
            value = Datetime::min();
        } else if (Datetime(year, month, day) == Datetime::max()) {
            value = Datetime::max();
        } else {
            long millisecond = microsecond / 1000;
            microsecond = microsecond - millisecond * 1000;
            value = Datetime(year, month, day, hour, minute, second, millisecond, microsecond);
        }

        return true;
    }

    /**
     * Conversion part 2 (C++ -> Python)
     */
    static handle cast(const Datetime& src, return_value_policy /* policy */, handle /* parent */) {
        if (src.isNull()) {
            return py::none();
        }

        // Lazy initialise the PyDateTime import
        if (!PyDateTimeAPI) {
            PyDateTime_IMPORT;
        }

        return PyDateTime_FromDateAndTime(src.year(), src.month(), src.day(), src.hour(),
                                          src.minute(), src.second(),
                                          src.millisecond() * 1000 + src.microsecond());
    }
};

}  // namespace detail
}  // namespace pybind11

#endif  // HIKYUU_PYTHON_CONVERT_DATETIME_H