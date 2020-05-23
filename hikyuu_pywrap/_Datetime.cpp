/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-17
 *      Author: fasiondog
 */

#include <pybind11/pybind11.h>
#include <hikyuu/datetime/Datetime.h>
#include <hikyuu/utilities/Null.h>
#include <hikyuu/utilities/util.h>
#include <datetime.h>
#include <memory>
#include <hikyuu/serialization/Datetime_serialization.h>
#include "pickle_support.h"

using namespace hku;
namespace py = pybind11;

std::unique_ptr<Datetime> createFromPyDatetime(const py::object& src) {
    // Lazy initialise the PyDateTime import
    if (!PyDateTimeAPI) {
        PyDateTime_IMPORT;
    }
    long year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, microsecond = 0;

    if (PyDateTime_Check(src.ptr())) {
        second = PyDateTime_DATE_GET_SECOND(src.ptr());
        minute = PyDateTime_DATE_GET_MINUTE(src.ptr());
        hour = PyDateTime_DATE_GET_HOUR(src.ptr());
        day = PyDateTime_GET_DAY(src.ptr());
        month = PyDateTime_GET_MONTH(src.ptr()) - 1;
        year = PyDateTime_GET_YEAR(src.ptr());
        microsecond = PyDateTime_DATE_GET_MICROSECOND(src.ptr());
    } else if (PyDate_Check(src.ptr())) {
        day = PyDateTime_GET_DAY(src.ptr());
        month = PyDateTime_GET_MONTH(src.ptr()) - 1;
        year = PyDateTime_GET_YEAR(src.ptr());
    } else {
        throw std::exception("Type error!");
    }

    if (year < 1400) {
        return std::make_unique<Datetime>(Datetime::min());
    }

    long millisecond = microsecond / 1000;
    microsecond = microsecond - millisecond * 1000;
    return std::make_unique<Datetime>(year, month, day, hour, minute, second, millisecond,
                                      microsecond);
}

void export_Datetime(py::module& m) {
    py::class_<Datetime>(m, "Datetime",
                         R"(日期时间类（精确到微秒），通过以下方式构建:
    
    - 通过字符串：Datetime("2010-1-1 10:00:00")、Datetime("2001-1-1")
                 Datetime("20010101")、Datetime("20010101T232359)
    - 通过 Python 的date：Datetime(date(2010,1,1))
    - 通过 Python 的datetime：Datetime(datetime(2010,1,1,10)
    - 通过 YYYYMMDDHHMM　或　YYYYMMDD 形式的整数：Datetime(201001011000)、Datetime(20010101)
    - Datetime(year, month, day, hour=0, minute=0, second=0, millisecond=0, microsecond=0))")

      .def(py::init<>())
      .def(py::init<const std::string&>(),
           "\t通过如下形式的字符串构建:"
           "\n\t - Datetime('2010-1-1 10:00:00')"
           "\n\t - Datetime('2001-1-1')"
           "\n\t - Datetime('20010101')"
           "\n\t - Datetime('20010101T232359')")

      .def(py::init<unsigned long long>(),
           "\t通过 YYYYMMDDHHMM　或　YYYYMMDD 形式的整数构建："
           "\n\t - Datetime(201001011000)"
           "\n\t - Datetime(20010101)")

      .def(py::init<long, long, long, long, long, long, long, long>(), py::arg("year"),
           py::arg("month"), py::arg("day"), py::arg("hour") = 0, py::arg("minute") = 0,
           py::arg("second") = 0, py::arg("millisecond") = 0, py::arg("microsecond") = 0)

      .def(py::init(&createFromPyDatetime),
           "\tconstruct from Python datetime.date or datetime.datetime:"
           "\n\t - Datetime(date(2010,1,1))"
           "\n\t - Datetime(datetime(2010,1,1,10)")

      .def("__str__", &Datetime::str)
      .def("__repr__", &Datetime::repr)
      .def_property_readonly("year", &Datetime::year)
      .def_property_readonly("month", &Datetime::month)
      .def_property_readonly("day", &Datetime::day)
      .def_property_readonly("hour", &Datetime::hour)
      .def_property_readonly("minute", &Datetime::minute)
      .def_property_readonly("second", &Datetime::second)
      .def_property_readonly("millisecond", &Datetime::millisecond)
      .def_property_readonly("microsecond", &Datetime::microsecond)
      //.def_property_readonly("number", &Datetime::number)

      .def("isNull", &Datetime::isNull)
      .def("dayOfWeek", &Datetime::dayOfWeek)
      .def("dayOfYear", &Datetime::dayOfYear)
      .def("startOfDay", &Datetime::startOfDay)
      .def("endOfDay", &Datetime::endOfDay)
      .def("nextDay", &Datetime::nextDay)
      .def("nextWeek", &Datetime::nextWeek)
      .def("nextMonth", &Datetime::nextMonth)
      .def("nextQuarter", &Datetime::nextQuarter)
      .def("nextHalfyear", &Datetime::nextHalfyear)
      .def("nextYear", &Datetime::nextYear)
      .def("preDay", &Datetime::preDay)
      .def("preWeek", &Datetime::preWeek)
      .def("preMonth", &Datetime::preMonth)
      .def("preQuarter", &Datetime::preQuarter)
      .def("preHalfyear", &Datetime::preHalfyear)
      .def("preYear", &Datetime::preYear)
      .def("dateOfWeek", &Datetime::dateOfWeek)
      .def("startOfWeek", &Datetime::startOfWeek)
      .def("endOfWeek", &Datetime::endOfWeek)
      .def("startOfMonth", &Datetime::startOfMonth)
      .def("endOfMonth", &Datetime::endOfMonth)
      .def("startOfQuarter", &Datetime::startOfQuarter)
      .def("endOfQuarter", &Datetime::endOfQuarter)
      .def("startOfHalfyear", &Datetime::startOfHalfyear)
      .def("endOfHalfyear", &Datetime::endOfHalfyear)
      .def("startOfYear", &Datetime::startOfYear)
      .def("endOfYear", &Datetime::endOfYear)

      .def("min", &Datetime::min)
      .def("max", &Datetime::max)
      .def("now", &Datetime::now)
      .def("today", &Datetime::today)

      .def(
        "date",
        [](Datetime* d) {
            // Lazy initialise the PyDateTime import
            if (!PyDateTimeAPI) {
                PyDateTime_IMPORT;
            }

            return d->isNull() ? py::reinterpret_borrow<py::object>(py::none())
                               : py::reinterpret_steal<py::object>(
                                   PyDate_FromDate(d->year(), d->month(), d->day()));
        },
        "Return python datetime.date object with same year, month and day. "
        "If Datetime is null, will return None.")

      .def(
        "datetime",
        [](Datetime* d) {
            // Lazy initialise the PyDateTime import
            if (!PyDateTimeAPI) {
                PyDateTime_IMPORT;
            }

            return d->isNull() ? py::reinterpret_borrow<py::object>(py::none())
                               : py::reinterpret_steal<py::object>(PyDateTime_FromDateAndTime(
                                   d->year(), d->month(), d->day(), d->hour(), d->minute(),
                                   d->second(), d->millisecond() * 1000 + d->microsecond()));
        },
        "Return python datetime.datetime object. If Datetime is null, will return None.")

      .def("__hash__",
           [](Datetime* d) {
               if (d->isNull()) {
                   return PyObject_Hash(py::none().ptr());
               }

               uint64_t o = 0x0;
               uint64_t ymd = d->year() * 10000 + d->month() * 100 + d->day();
               o = ymd << 40;
               uint64_t hms = ((uint64_t)d->hour() * 3600ULL + (uint64_t)d->minute() * 60ULL +
                               (uint64_t)d->second()) *
                                1000000ULL +
                              (uint64_t)d->millisecond() * 1000ULL + (uint64_t)d->microsecond();
               o |= hms;

               return (Py_hash_t)o;
           })

        DEF_PICKLE(Datetime);
}