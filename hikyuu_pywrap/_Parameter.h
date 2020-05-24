/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-24
 *      Author: fasiondog
 */

#pragma once
#ifndef HIKYUU_PYTHON_PARAMETER_H
#define HIKYUU_PYTHON_PARAMETER_H

#include "convert_any.h"

namespace hku {
namespace py = pybind11;

template <>
inline boost::any Parameter::get<boost::any>(const std::string& name) const {
    param_map_t::const_iterator iter;
    iter = m_params.find(name);
    if (iter == m_params.end()) {
        throw std::out_of_range("out_of_range in Parameter::get : " + name);
    }
    return iter->second;
}

template <>
inline void Parameter::set(const string& name, const boost::any& value) {
    if (!have(name)) {
        if (!support(value)) {
            HKU_THROW_EXCEPTION(std::logic_error, "Unsupport Type! current value type is {}",
                                value.type().name());
        }
        m_params[name] = value;
        return;
    }

    if (m_params[name].type() != value.type()) {
        HKU_THROW_EXCEPTION(
          std::logic_error,
          "Mismatching type! The type of '{}' is {}, but current value type is {}!", name,
          m_params[name].type().name(), value.type().name());
    }

    m_params[name] = value;
}

}  // namespace hku

#endif /* HIKYUU_PYTHON_PARAMETER_H */