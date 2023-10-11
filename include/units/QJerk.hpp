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
#include "units/QTime.hpp"
#include "units/RQuantity.hpp"

namespace units {
QUANTITY_TYPE(0, 1, -3, 0, QJerk)

constexpr QJerk mps3 = meter / (second * second * second);
} // namespace units