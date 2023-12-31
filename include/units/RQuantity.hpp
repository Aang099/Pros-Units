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

#include <cmath>
#include <ratio>

namespace units {
template <typename MassDim, typename LengthDim, typename TimeDim, typename AngleDim> class RQuantity {
    public:
        explicit constexpr RQuantity() : value(0.0) {}

        explicit constexpr RQuantity(double val) : value(val) {}

        explicit constexpr RQuantity(long double val) : value(static_cast<double>(val)) {}

        // The intrinsic operations for a quantity with a unit is addition and
        // subtraction
        constexpr RQuantity const& operator+=(const RQuantity& rhs) {
            value += rhs.value;
            return *this;
        }

        constexpr RQuantity const& operator-=(const RQuantity& rhs) {
            value -= rhs.value;
            return *this;
        }

        constexpr RQuantity operator-() { return RQuantity(value * -1); }

        constexpr RQuantity const& operator*=(const double rhs) {
            value *= rhs;
            return *this;
        }

        constexpr RQuantity const& operator/=(const double rhs) {
            value /= rhs;
            return *this;
        }

        // Returns the value of the quantity in multiples of the specified unit
        constexpr double convert(const RQuantity& rhs) const { return value / rhs.value; }

        // returns the raw value of the quantity (should not be used)
        constexpr double getValue() const { return value; }

        constexpr RQuantity<MassDim, LengthDim, TimeDim, AngleDim> abs() const {
            return RQuantity<MassDim, LengthDim, TimeDim, AngleDim>(std::fabs(value));
        }

        constexpr RQuantity<std::ratio_divide<MassDim, std::ratio<2>>, std::ratio_divide<LengthDim, std::ratio<2>>,
                            std::ratio_divide<TimeDim, std::ratio<2>>, std::ratio_divide<AngleDim, std::ratio<2>>>
        sqrt() const {
            return RQuantity<std::ratio_divide<MassDim, std::ratio<2>>, std::ratio_divide<LengthDim, std::ratio<2>>,
                             std::ratio_divide<TimeDim, std::ratio<2>>, std::ratio_divide<AngleDim, std::ratio<2>>>(
                std::sqrt(value));
        }
    private:
        double value;
};

// Predefined (physical unit) quantity types:
// ------------------------------------------
#define QUANTITY_TYPE(_Mdim, _Ldim, _Tdim, _Adim, name)                                                                \
    typedef RQuantity<std::ratio<_Mdim>, std::ratio<_Ldim>, std::ratio<_Tdim>, std::ratio<_Adim>> name;

// Unitless
QUANTITY_TYPE(0, 0, 0, 0, Number)
constexpr Number number(1.0);

// isRquantity concept makes everything much more readable
template <typename... args> void RQuantityChecker(RQuantity<args...>) {}

template <typename T>
concept isRQuantity = requires { RQuantityChecker(std::declval<T>()); };

constexpr void QAngleChecker(RQuantity<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>) {}

template <typename T>
concept isQAngle = requires { QAngleChecker(std::declval<T>()); };

// Utility macros to make everything even more readable
#define RATIO_ADD std::ratio_add<M1, M2>, std::ratio_add<L1, L2>, std::ratio_add<T1, T2>, std::ratio_add<A1, A2>
#define RATIO_SUBTRACT                                                                                                 \
    std::ratio_subtract<M1, M2>, std::ratio_subtract<L1, L2>, std::ratio_subtract<T1, T2>, std::ratio_subtract<A1, A2>
#define RATIO_INVERSE                                                                                                  \
    std::ratio_subtract<std::ratio<0>, M>, std::ratio_subtract<std::ratio<0>, L>,                                      \
        std::ratio_subtract<std::ratio<0>, T>, std::ratio_subtract<std::ratio<0>, A>
#define RATIO_MULTIPLY(_R)                                                                                             \
    std::ratio_multiply<M, _R>, std::ratio_multiply<L, _R>, std::ratio_multiply<T, _R>, std::ratio_multiply<A, _R>
#define RATIO_DIVIDE(_R)                                                                                               \
    std::ratio_divide<M, _R>, std::ratio_divide<L, _R>, std::ratio_divide<T, _R>, std::ratio_divide<A, _R>

// Standard arithmetic operators:
// ------------------------------
template <isRQuantity T> constexpr T operator+(const T& lhs, const T& rhs) {
    return T(lhs.getValue() + rhs.getValue());
}

template <isRQuantity T> constexpr T operator-(const T& lhs, const T& rhs) {
    return T(lhs.getValue() - rhs.getValue());
}

template <typename M1, typename L1, typename T1, typename A1, typename M2, typename L2, typename T2, typename A2>
constexpr RQuantity<RATIO_ADD> operator*(const RQuantity<M1, L1, T1, A1>& lhs, const RQuantity<M2, L2, T2, A2>& rhs) {
    return RQuantity<RATIO_ADD>(lhs.getValue() * rhs.getValue());
}

template <isRQuantity T> constexpr T operator*(const double& lhs, const T& rhs) { return T(lhs * rhs.getValue()); }

template <isRQuantity T> constexpr T operator*(const T& lhs, const double& rhs) { return T(lhs.getValue() * rhs); }

template <typename M1, typename L1, typename T1, typename A1, typename M2, typename L2, typename T2, typename A2>
constexpr RQuantity<RATIO_SUBTRACT> operator/(const RQuantity<M1, L1, T1, A1>& lhs,
                                              const RQuantity<M2, L2, T2, A2>& rhs) {
    return RQuantity<RATIO_SUBTRACT>(lhs.getValue() / rhs.getValue());
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<RATIO_INVERSE> operator/(const double& x, const T& rhs) {
    return RQuantity<RATIO_INVERSE>(x / rhs.getValue());
}

template <isRQuantity T> constexpr T operator/(const T& rhs, const double& x) { return T(rhs.getValue() / x); }

// Comparison operators for quantities:
// ------------------------------------
template <isRQuantity T> constexpr bool operator==(const T& lhs, const T& rhs) {
    return (lhs.getValue() == rhs.getValue());
}

template <isRQuantity T> constexpr bool operator!=(const T& lhs, const T& rhs) {
    return (lhs.getValue() != rhs.getValue());
}

template <isRQuantity T> constexpr bool operator<=(const T& lhs, const T& rhs) {
    return (lhs.getValue() <= rhs.getValue());
}

template <isRQuantity T> constexpr bool operator>=(const T& lhs, const T& rhs) {
    return (lhs.getValue() >= rhs.getValue());
}

template <isRQuantity T> constexpr bool operator<(const T& lhs, const T& rhs) {
    return (lhs.getValue() < rhs.getValue());
}

template <isRQuantity T> constexpr bool operator>(const T& lhs, const T& rhs) {
    return (lhs.getValue() > rhs.getValue());
}

// Common math functions:
// ------------------------------

template <isRQuantity T> constexpr T abs(const T& rhs) { return T(std::abs(rhs.getValue())); }

template <typename R, typename M, typename L, typename T, typename A>
constexpr RQuantity<RATIO_MULTIPLY(R)> pow(const RQuantity<M, L, T, A>& lhs) {
    return RQuantity<RATIO_MULTIPLY(R)>(std::pow(lhs.getValue(), double(R::num) / R::den));
}

template <int R, typename M, typename L, typename T, typename A>
constexpr RQuantity<RATIO_DIVIDE(std::ratio<R>)> root(const T& lhs) {
    return RQuantity<RATIO_DIVIDE(std::ratio<R>)>(std::pow(lhs.getValue(), 1.0 / R));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<RATIO_DIVIDE(std::ratio<2>)> sqrt(const T& rhs) {
    return RQuantity<RATIO_DIVIDE(std::ratio<2>)>(std::sqrt(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<RATIO_DIVIDE(std::ratio<3>)> cbrt(const T& rhs) {
    return RQuantity<RATIO_DIVIDE(std::ratio<3>)>(std::cbrt(rhs.getValue()));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<RATIO_MULTIPLY(std::ratio<2>)> square(const T& rhs) {
    return RQuantity<RATIO_MULTIPLY(std::ratio<2>)>(std::pow(rhs.getValue(), 2));
}

template <typename M, typename L, typename T, typename A>
constexpr RQuantity<RATIO_MULTIPLY(std::ratio<3>)> cube(const T& rhs) {
    return RQuantity<RATIO_MULTIPLY(std::ratio<3>)>(std::pow(rhs.getValue(), 3));
}

template <isRQuantity T> constexpr T hypot(const T& lhs, const T& rhs) {
    return T(std::hypot(lhs.getValue(), rhs.getValue()));
}

template <isRQuantity T> constexpr T mod(const T& lhs, const T& rhs) {
    return T(std::fmod(lhs.getValue(), rhs.getValue()));
}

template <isRQuantity T1, isRQuantity T2> constexpr T1 copysign(const T1& lhs, const T2& rhs) {
    return T1(std::copysign(lhs.getValue(), rhs.getValue()));
}

template <isRQuantity T> constexpr T ceil(const T& lhs, const T& rhs) {
    return T(std::ceil(lhs.getValue() / rhs.getValue()) * rhs.getValue());
}

template <isRQuantity T> constexpr T floor(const T& lhs, const T& rhs) {
    return T(std::floor(lhs.getValue() / rhs.getValue()) * rhs.getValue());
}

template <isRQuantity T> constexpr T trunc(const T& lhs, const T& rhs) {
    return T(std::trunc(lhs.getValue() / rhs.getValue()) * rhs.getValue());
}

template <isRQuantity T> constexpr T round(const T& lhs, const T& rhs) {
    return T(std::round(lhs.getValue() / rhs.getValue()) * rhs.getValue());
}

// Common trig functions:
// ------------------------------

template <isQAngle T> constexpr Number sin(const T& rhs) { return Number(std::sin(rhs.getValue())); }

template <isQAngle T> constexpr Number cos(const T& rhs) { return Number(std::cos(rhs.getValue())); }

template <isQAngle T> constexpr Number tan(const T& rhs) { return Number(std::tan(rhs.getValue())); }

template <isQAngle T> T asin(const Number& rhs) { return T(std::asin(rhs.getValue())); }

template <isQAngle T> T acos(const Number& rhs) { return T(std::acos(rhs.getValue())); }

template <isQAngle T> T atan(const Number& rhs) { return T(std::atan(rhs.getValue())); }

template <isQAngle T> constexpr Number sinh(const T& rhs) { return Number(std::sinh(rhs.getValue())); }

template <isQAngle T> constexpr Number cosh(const T& rhs) { return Number(std::cosh(rhs.getValue())); }

template <isQAngle T> constexpr Number tanh(const T& rhs) { return Number(std::tanh(rhs.getValue())); }

template <isQAngle T> T asinh(const Number& rhs) { return T(std::asinh(rhs.getValue())); }

template <isQAngle T> T acosh(const Number& rhs) { return T(std::acosh(rhs.getValue())); }

template <isQAngle T> T atanh(const Number& rhs) { return T(std::atanh(rhs.getValue())); }

template <isRQuantity T, isQAngle A> T atan2(const T& lhs, const T& rhs) {
    return A(std::atan2(lhs.getValue(), rhs.getValue()));
}

inline namespace literals {
constexpr long double operator"" _pi(long double x) {
    return static_cast<double>(x) * 3.1415926535897932384626433832795;
}

constexpr long double operator"" _pi(unsigned long long int x) {
    return static_cast<double>(x) * 3.1415926535897932384626433832795;
}
} // namespace literals

} // namespace units

// Conversion macro, which utilizes the string literals
#define ConvertTo(_x, _y) (_x).convert(1.0_##_y)
