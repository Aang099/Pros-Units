#pragma once

#include "units/QAcceleration.hpp"
#include "units/QAngle.hpp"
#include "units/QLength.hpp"
#include "units/QSpeed.hpp"
#include "units/RQuantity.hpp"

namespace units {
template <isRQuantity T> class Vector2D {
    private:
        T x, y;
    public:
        Vector2D() : x(0.0), y(0.0) {}

        Vector2D(T nx, T ny) : x(nx), y(ny) {}

        static Vector2D fromPolar(QAngle t, T m) {
            m = m.abs();
            t = constrainAngle(t);
            return Vector2D<T>(m * cos(t), m * sin(t));
        }

        T getX() { return x; }

        T getY() { return y; }

        Vector2D<T> operator+(Vector2D<T>& other) { return Vector2D<T>(x + other.x, y + other.y); }

        Vector2D<T> operator-(Vector2D<T>& other) { return Vector2D<T>(x - other.x, y - other.y); }

        Vector2D<T> operator*(double factor) { return Vector2D<T>(x * factor, y * factor); }

        Vector2D<T> operator/(double factor) { return Vector2D<T>(x / factor, y / factor); }

        Vector2D<T>& operator+=(Vector2D<T>& other) {
            x += other.x;
            y += other.y;
            return (*this);
        }

        Vector2D<T>& operator-=(Vector2D<T>& other) {
            x -= other.x;
            y -= other.y;
            return (*this);
        }

        Vector2D<T>& operator*=(double factor) {
            x *= factor;
            y *= factor;
            return (*this);
        }

        Vector2D<T>& operator/=(double factor) {
            x /= factor;
            y /= factor;
            return (*this);
        }

        double dot(Vector2D<T>& other) { return ((x * other.x) + (y * other.y)).getValue(); }

        QAngle theta() { return atan2(y, x); }

        T magnitude() { return sqrt(square(x) + square(y)); }

        Vector2D<T> vectorTo(Vector2D<T>& other) { return Vector2D<T>(other.x - x, other.y - y); }

        QAngle angleTo(Vector2D<T>& other) { return atan2(other.y - y, other.x - x); }

        T distance(Vector2D<T>& other) { return sqrt(square(x - other.x, 2) + square(y - other.y, 2)); }

        void rotateBy(QAngle angle) {
            T m = magnitude();
            QAngle t = theta() + angle;
            x = m * cos(t);
            y = m * sin(t);
        }

        void rotateTo(QAngle angle) {
            T m = magnitude();
            x = m * cos(angle);
            y = m * sin(angle);
        }

        Vector2D<T> rotatedBy(QAngle angle) {
            T m = magnitude();
            QAngle t = theta() + angle;
            return fromPolar(t, m);
        }

        Vector2D<T> rotatedTo(QAngle angle) {
            T m = magnitude();
            return fromPolar(angle, m);
        }
};

typedef Vector2D<QLength> V2Position;
typedef Vector2D<QSpeed> V2Velocity;
typedef Vector2D<QAcceleration> V2Acceleration;
} // namespace units