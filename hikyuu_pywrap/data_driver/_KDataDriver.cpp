/*
 *  Copyright (c) hikyuu.org
 *
 *  Created on: 2020-6-14
 *      Author: fasiondog
 */

#include <hikyuu/indicator/build_in.h>
#include <pybind11/pybind11.h>
#include <hikyuu/data_driver/KDataDriver.h>
#include "../_Parameter.h"
#include "../pybind_utils.h"

using namespace hku;
namespace py = pybind11;

class PyKDataDriver : public KDataDriver {
public:
    using KDataDriver::KDataDriver;

    bool _init() {
        PYBIND11_OVERLOAD(bool, KDataDriver, _init, );
    }

    void loadKData(const string& market, const string& code, KQuery::KType kType, size_t start_ix,
                   size_t end_ix, KRecordListPtr out_buffer) {
        PYBIND11_OVERLOAD(void, KDataDriver, loadKData, market, code, kType, start_ix, end_ix,
                          out_buffer);
    }

    size_t getCount(const string& market, const string& code, KQuery::KType kType) {
        PYBIND11_OVERLOAD(size_t, KDataDriver, getCount, market, code, kType);
    }

    // TODO
    bool getIndexRangeByDate(const string& market, const string& code, const KQuery& query,
                             size_t& out_start, size_t& out_end) {
        PYBIND11_OVERLOAD(bool, KDataDriver, getIndexRangeByDate, market, code, query, out_start,
                          out_end);
    }

    KRecord getKRecord(const string& market, const string& code, size_t pos, KQuery::KType kType) {
        PYBIND11_OVERLOAD(KRecord, KDataDriver, getKRecord, market, code, pos, kType);
    }

    TimeLineList getTimeLineList(const string& market, const string& code, const KQuery& query) {
        auto self = py::cast(this);
        auto py_list = self.attr("_getTimeLineList")(market, code, query);
        return python_list_to_vector<TimeLineRecord>(py_list);
    }

    TransList getTransList(const string& market, const string& code, const KQuery& query) {
        auto self = py::cast(this);
        auto py_list = self.attr("_getTransList")(market, code, query);
        return python_list_to_vector<TransRecord>(py_list);
    }
};

void export_KDataDriver(py::module& m) {
    py::class_<KDataDriver, KDataDriverPtr, PyKDataDriver>(m, "KDataDriver")
      .def(py::init<>())
      .def(py::init<const string&>())

      .def_property_readonly("name", &KDataDriver::name)

      .def("__str__", to_py_str<KDataDriver>)
      .def("__repr__", to_py_str<KDataDriver>)

      .def("getParam", &KDataDriver::getParam<boost::any>)

      .def("_init", &KDataDriver::_init)
      .def("loadKData", &KDataDriver::loadKData)
      .def("getCount", &KDataDriver::getCount)
      .def("getKRecord", &KDataDriver::getKRecord)
      //.def("_getIndexRangeByDate", &KDataDriverWrap::_getIndexRangeByDate,
      //    &KDataDriverWrap::default_getIndexRangeByDate)

      ;
}