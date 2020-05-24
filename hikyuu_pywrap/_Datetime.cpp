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
#include "pickle_support.h"

#if HKU_PYTHON_SUPPORT_PICKLE
#include <hikyuu/serialization/Datetime_serialization.h>
#include <hikyuu/serialization/Datetime_serialization.h>
#include <boost/serialization/vector.hpp>
#endif

using namespace hku;
namespace py = pybind11;

std::unique_ptr<Datetime> createFromPyDatetime(const py::object& src) {
    // Lazy initialise the PyDateTime import
    if (!PyDateTimeAPI) {
        PyDateTime_IMPORT;
    }
    long year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, microsecond = 0;

    if (src.is_none()) {
        return std::make_unique<Datetime>();
    } else if (PyDateTime_Check(src.ptr())) {
        second = PyDateTime_DATE_GET_SECOND(src.ptr());
        minute = PyDateTime_DATE_GET_MINUTE(src.ptr());
        hour = PyDateTime_DATE_GET_HOUR(src.ptr());
        day = PyDateTime_GET_DAY(src.ptr());
        month = PyDateTime_GET_MONTH(src.ptr());
        year = PyDateTime_GET_YEAR(src.ptr());
        microsecond = PyDateTime_DATE_GET_MICROSECOND(src.ptr());
    } else if (PyDate_Check(src.ptr())) {
        day = PyDateTime_GET_DAY(src.ptr());
        month = PyDateTime_GET_MONTH(src.ptr());
        year = PyDateTime_GET_YEAR(src.ptr());
    } else {
        throw std::exception("Can't convert, only support datetime to Datetime!");
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
                 Datetime("20010101")、Datetime("20010101T232359.001001")
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
           py::arg("second") = 0, py::arg("millisecond") = 0, py::arg("microsecond") = 0,
           "\t year - 1400 .. 9999"
           "\n\t month - 1 .. 12"
           "\n\t day - 1 .. 31"
           "\n\t hour - 0 .. 23"
           "\n\t minute - 0 .. 59"
           "\n\t second - 0 .. 59"
           "\n\t millisecond - 0 .. 999"
           "\n\t microsecond - 0 .. 999")

      .def(py::init(&createFromPyDatetime),
           "\tconstruct from Python datetime.date or datetime.datetime:"
           "\n\t - Datetime(date(2010,1,1))"
           "\n\t - Datetime(datetime(2010,1,1,10)")

      .def("__str__", &Datetime::str)
      .def("__repr__", &Datetime::repr)

      .def_property_readonly(
        "year", &Datetime::year,
        "Between 1400 and 9999 inclusive. If it is the Datetime(), will throw RuntimeError.")
      .def_property_readonly(
        "month", &Datetime::month,
        "Between 1 and 12 inclusive. If it is the Datetime(), will throw RuntimeError.")
      .def_property_readonly("day", &Datetime::day,
                             "Between 1 and the number of days in the given month of the given "
                             "year. If it is the Datetime(), will throw RuntimeError.")
      .def_property_readonly("hour", &Datetime::hour,
                             "In range(24). If it is the Datetime(), will throw RuntimeError.")
      .def_property_readonly("minute", &Datetime::minute,
                             "In range(60). If it is the Datetime(), will throw RuntimeError.")
      .def_property_readonly("second", &Datetime::second,
                             "In range(60). If it is the Datetime(), will throw RuntimeError.")
      .def_property_readonly("millisecond", &Datetime::millisecond,
                             "In range(1000). If it is the Datetime(), will throw RuntimeError.")
      .def_property_readonly("microsecond", &Datetime::microsecond,
                             "In range(1000). If it is the Datetime(), will throw RuntimeError.")

      .def("isNull", &Datetime::isNull,
           "是否是Null值，等效于　d==Datetime()　或　d==Datetime(None)")
      .def("dayOfWeek", &Datetime::dayOfWeek,
           "Return the day of the week, Sunday is 0, Monday is 1.")
      .def("dayOfYear", &Datetime::dayOfYear,
           "Returns the day of the year, 1 January is the first day of the year.")
      .def("startOfDay", &Datetime::startOfDay, "Return to 0:0:00 a.m. on the same day")
      .def("endOfDay", &Datetime::endOfDay, "Return to 23:59:49 on the same day.")
      .def("nextDay", &Datetime::nextDay, "Return the next nature day.")
      .def("nextWeek", &Datetime::nextWeek, "Return to next Monday date.")
      .def("nextMonth", &Datetime::nextMonth, "Returns the first day of the next month.")
      .def("nextQuarter", &Datetime::nextQuarter, "Returns the first day of the next quarter.")
      .def("nextHalfyear", &Datetime::nextHalfyear, "Returns the first day of the next half year.")
      .def("nextYear", &Datetime::nextYear, "Returns the first day of the next year.")
      .def("preDay", &Datetime::preDay, "Returns the date of the previous nature day.")
      .def("preWeek", &Datetime::preWeek, "Return the date of the previous Monday date.")
      .def("preMonth", &Datetime::preMonth, "Return the first date of the previous Month date.")
      .def("preQuarter", &Datetime::preQuarter,
           "Return the first date of the previous Quarter date.")
      .def("preHalfyear", &Datetime::preHalfyear,
           "Return the first date of the previous half year date.")
      .def("preYear", &Datetime::preYear, "Return the first date of the previous year date.")
      .def("dateOfWeek", &Datetime::dateOfWeek,
           "Returns the date of the specified day of the week, Sunday is 0 day and Saturday is 6")
      .def("startOfWeek", &Datetime::startOfWeek, "Return the week start date (Monday).")
      .def("endOfWeek", &Datetime::endOfWeek, "Return the week end date (Sunday).")
      .def("startOfMonth", &Datetime::startOfMonth, "Return the first date of the month.")
      .def("endOfMonth", &Datetime::endOfMonth, "Return the last date of the month.")
      .def("startOfQuarter", &Datetime::startOfQuarter, "Return the first date of the quarter.")
      .def("endOfQuarter", &Datetime::endOfQuarter, "Return the last date of the month.")
      .def("startOfHalfyear", &Datetime::startOfHalfyear, "Return the first date of the half year.")
      .def("endOfHalfyear", &Datetime::endOfHalfyear, "Return the last date of the half-year.")
      .def("startOfYear", &Datetime::startOfYear, "Return the first date of the year.")
      .def("endOfYear", &Datetime::endOfYear, "Return the last date of the year.")

      .def("min", &Datetime::min, "Return the smallest Datetime object. Datetime(1400,1,1)")
      .def("max", &Datetime::max, "Return the largest Datetime object. Datetime(9999, 12, 31)")
      .def("now", &Datetime::now, "Return the current local date and time.")
      .def("today", &Datetime::today, "Return the current local date.")

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

      .def("__eq__", (bool (*)(const Datetime&, const Datetime&)) & operator==)
      .def("__eq__",
           [](Datetime* date, const py::object& pyd) {
               auto pydate = createFromPyDatetime(pyd);
               return (*date) == (*pydate);
           })

      .def("__ne__", (bool (*)(const Datetime&, const Datetime&)) & operator!=)
      .def("__ne__",
           [](Datetime* date, const py::object& pyd) {
               auto pydate = createFromPyDatetime(pyd);
               return (*date) != (*pydate);
           })

      .def("__gt__", (bool (*)(const Datetime&, const Datetime&)) & operator>)
      .def("__gt__",
           [](Datetime* date, const py::object& pyd) {
               auto pydate = createFromPyDatetime(pyd);
               return (*date) > (*pydate);
           })

      .def("__lt__", (bool (*)(const Datetime&, const Datetime&)) & operator<)
      .def("__lt__",
           [](Datetime* date, const py::object& pyd) {
               auto pydate = createFromPyDatetime(pyd);
               return (*date) < (*pydate);
           })

      .def("__ge__", (bool (*)(const Datetime&, const Datetime&)) & operator>=)
      .def("__ge__",
           [](Datetime* date, const py::object& pyd) {
               auto pydate = createFromPyDatetime(pyd);
               return (*date) >= (*pydate);
           })

      .def("__le__", (bool (*)(const Datetime&, const Datetime&)) & operator<=)
      .def("__le__",
           [](Datetime* date, const py::object& pyd) {
               auto pydate = createFromPyDatetime(pyd);
               return (*date) <= (*pydate);
           })

        DEF_PICKLE(Datetime);

    DatetimeList::const_reference (DatetimeList::*datetimeList_at)(DatetimeList::size_type) const =
      &DatetimeList::at;
    void (DatetimeList::*datetimelist_append)(const DatetimeList::value_type& val) =
      &DatetimeList::push_back;
    py::class_<DatetimeList>(m, "DatetimeList")
      //.def("__iter__", py::iterator<DatetimeList>())
      .def("size", &DatetimeList::size)
      .def("__len__", &DatetimeList::size)
      .def("append", datetimelist_append)
      .def("get", datetimeList_at, py::return_value_policy::copy) DEF_PICKLE(DatetimeList);

    m.def("getDateRange", getDateRange, py::arg("start"), py::arg("end"));
}