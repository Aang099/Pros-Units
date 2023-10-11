/*
 * This code is a modified version of Benjamin Jurke's work in 2015. You can
 * read his blog post here:
 * https://benjaminjurke.com/content/articles/2015/compile-time-numerical-unit-dimension-checking/
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include "units/QAngle.hpp"
#include "units/QLength.hpp"
#include <cmath>

namespace units {
QUANTITY_TYPE(0, -1, 0, 1, QCurvature)

constexpr QCurvature radpm(1.0);
constexpr QCurvature degpm = degree / meter;

inline namespace literals {
constexpr QCurvature operator"" _radpm(long double x) { return QCurvature(x); }

constexpr QCurvature operator"" _radpm(unsigned long long int x) {
  return QCurvature(static_cast<double>(x));
}

constexpr QCurvature operator"" _degpm(long double x) {
  return static_cast<double>(x) * degpm;
}

constexpr QCurvature operator"" _degpm(unsigned long long int x) {
  return static_cast<double>(x) * degpm;
}
} // namespace literals
} // namespace units