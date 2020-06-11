/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-25
 *      Author: fasiondog
 */

#pragma once
#ifndef HIKYUU_PYTHON_BIND_UTILS_H
#define HIKYUU_PYTHON_BIND_UTILS_H

#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <vector>
#include <string>

PYBIND11_MAKE_OPAQUE(std::vector<double>);

namespace py = pybind11;

namespace hku {

template <typename T>
std::vector<T> python_list_to_vector(const py::list& obj) {
    auto total = len(obj);
    std::vector<T> vect(total);
    for (auto i = 0; i < total; ++i) {
        vect[i] = obj[i].cast<T>();
    }
    return vect;
}

template <typename T>
py::list vector_to_python_list(const std::vector<T>& vect) {
    py::list obj;
    for (unsigned long i = 0; i < vect.size(); ++i)
        obj.append(vect[i]);
    return obj;
}

template <typename T>
void extend_vector_with_python_list(std::vector<T>& v, const py::list& l) {
    for (const auto& item : l)
        v.push_back(item.cast<T>());
}

}  // namespace hku

#endif  // HIKYUU_PYTHON_BIND_UTILS_H
