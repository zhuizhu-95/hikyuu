/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-25
 *      Author: fasiondog
 */

#pragma once
#ifndef HIKYUU_PYTHON_CONVERT_TIMEDELTA_H
#define HIKYUU_PYTHON_CONVERT_TIMEDELTA_H

#include <pybind11/pybind11.h>
#include <hikyuu/datetime/TimeDelta.h>
#include "pybind_utils.h"
#include <datetime.h>

using namespace hku;

namespace pybind11 {
namespace detail {

template <>
struct type_caster<TimeDelta> {
public:
    PYBIND11_TYPE_CASTER(TimeDelta, _("datetime.timedelta"));

    /**
     * Conversion part 1 (Python->C++)
     */
    bool load(handle source, bool) {
        if (!source) {
            return false;
        }

        if (!PyDateTimeAPI) {
            PyDateTime_IMPORT;
        }

        PyObject* src = source.ptr();

        if (PyDelta_Check(src)) {
            auto days = PyDateTime_DELTA_GET_DAYS(src);
            auto seconds = PyDateTime_DELTA_GET_SECONDS(src);
            auto microseconds = PyDateTime_DELTA_GET_MICROSECONDS(src);
            value = TimeDelta(days, 0, 0, seconds, 0, microseconds);
            return true;

        } else if (PyFloat_Check(src)) {
            // If invoked with a float we assume it is seconds and convert
            double seconds = PyFloat_AsDouble(src);
            value = TimeDelta::fromTicks(int64_t(seconds * 1000000));
            return true;

        } else {
            return false;
        }
    }

    /**
     * Conversion part 2 (C++ -> Python)
     */
    static handle cast(const TimeDelta& src, return_value_policy /* policy */,
                       handle /* parent */) {
        // Lazy initialise the PyDateTime import
        if (!PyDateTimeAPI) {
            PyDateTime_IMPORT;
        }

        return PyDelta_FromDSU(int(src.days()),
                               int(src.hours() * 3600 + src.minutes() * 60 + src.seconds()),
                               int(src.milliseconds() * 1000 + src.microseconds()));
    }
};

}  // namespace detail
}  // namespace pybind11

#endif  // HIKYUU_PYTHON_CONVERT_TIMEDELTA_H