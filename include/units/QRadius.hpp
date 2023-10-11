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

#include "units/QLength.hpp"
#include <cmath>

namespace units {
QUANTITY_TYPE(0, 1, 0, -1, QRadius)

constexpr QRadius mprad(1.0);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static QRadius toRadius(const QLength iRadius) {
  return QRadius(iRadius.getValue());
}

#pragma GCC diagnostic pop

inline namespace literals {
constexpr QRadius operator"" _mprad(long double x) { return QRadius(x); }

constexpr QRadius operator"" _mprad(unsigned long long int x) {
  return QRadius(static_cast<double>(x));
}
} // namespace literals
} // namespace units
