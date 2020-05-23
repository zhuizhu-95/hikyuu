/*
 * pickle_template.h
 *
 *  Created on: 2013-4-28
 *      Author: fasiondog
 */

#ifndef PICKLE_SUPPORT_H_
#define PICKLE_SUPPORT_H_

#include <hikyuu/config.h>

#if HKU_SUPPORT_BINARY_ARCHIVE || HKU_SUPPORT_XML_ARCHIVE || HKU_SUPPORT_TEXT_ARCHIVE
#define HKU_PYTHON_SUPPORT_PICKLE 1
#endif

#if HKU_PYTHON_SUPPORT_PICKLE
#if HKU_SUPPORT_BINARY_ARCHIVE
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#define OUTPUT_ARCHIVE boost::archive::binary_oarchive
#define INPUT_ARCHIVE boost::archive::binary_iarchive

#else
#if HKU_SUPPORT_XML_ARCHIVE
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#define OUTPUT_ARCHIVE boost::archive::xml_oarchive
#define INPUT_ARCHIVE boost::archive::xml_iarchive

#else
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#define OUTPUT_ARCHIVE boost::archive::text_oarchive
#define INPUT_ARCHIVE boost::archive::text_iarchive
#endif /* HKU_SUPPORT_XML_ARCHIVE */

#endif /* HKU_SUPPORT_BINARY_ARCHIVE */
#endif /* HKU_PYTHON_SUPPORT_PICKLE */

#if HKU_PYTHON_SUPPORT_PICKLE
#include <string>
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;

#define DEF_PICKLE(classname)                    \
    .def(py::pickle(                             \
      [](const classname& p) {                   \
          std::ostringstream os;                 \
          OUTPUT_ARCHIVE oa(os);                 \
          oa << p;                               \
          return py::str(os.str());              \
      },                                         \
      [](py::str t) {                            \
          std::string st(t.cast<std::string>()); \
          std::istringstream is(st);             \
          INPUT_ARCHIVE ia(is);                  \
          classname result;                      \
          ia >> result;                          \
          return result;                         \
      }))

#else
#define DEF_PICKLE(classname)
#endif /* HKU_PYTHON_SUPPORT_PICKLE */

#endif /* PICKLE_SUPPORT_H_ */
