/*
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2020-5-24
 *      Author: fasiondog
 */

#pragma once
#ifndef HIKYUU_PYTHON_OPAQUE_TYPES_H
#define HIKYUU_PYTHON_OPAQUE_TYPES_H

#include <string>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <hikyuu/datetime/Datetime.h>

PYBIND11_MAKE_OPAQUE(std::vector<double>);

#endif /* HIKYUU_PYTHON_OPAQUE_TYPES_H */