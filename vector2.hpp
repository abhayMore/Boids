#pragma once

#include <iostream>
#include <cmath>


template <class T>
class Vector2 {
 public:
  using value_type = T;
  using class_type = Vector2<T>;
  value_type x;
  value_type y;

  Vector2(T value) : x(value), y(value) {}
  Vector2(T x = 0, T y = 0) : x(x), y(y) {}
  Vector2(const class_type& vector) : x(vector.x), y(vector.y) {}

  auto operator+(const class_type& right) const {
    return class_type(x + right.x, y + right.y);
  }
  auto operator-(const class_type& right) const {
    return class_type(x - right.x, y - right.y);
  }
  auto operator+(T n) const { return class_type(x + n, y + n); }
  auto operator*(T n) { return class_type(x * n, y * n); }
  auto operator/(T n) { return class_type(x / n, y / n); }

  auto operator==(const class_type& right) const {
    return x == right.x && y == right.y;
  }
  auto operator!=(const class_type& right) const {
    return x != right.x || y != right.y;
  }
  auto operator=(const class_type& right) {
    x = right.x;
    y = right.y;
    return *this;
  }

  auto operator*= (const T &n) {
    x *= n;
    y *= n;
    return *this;
  }
  auto operator/= (const T &n) {
    x /= n;
    y /= n;
    return *this;
  }
  auto operator+= (const T &n) {
    x += n;
    y += n;
    return *this;
  }
  auto operator-= (const T &n) {
    x -= n;
    y -= n;
    return *this;
  }
  auto operator-= (const class_type &n) {
    x -= n.x;
    y -= n.y;
    return *this;
  }
  auto operator+= (const class_type &n) {
    x += n.x;
    y += n.y;
    return *this;
  }

  auto distance(const class_type& right) {
    auto dx = x - right.x;
    auto dy = y - right.y;
    return sqrt(dx * dx + dy * dy);
  }

  auto magnitude() const { return sqrt(x * x + y * y); };
  auto normalize() const {
    const auto mag = magnitude();
    return class_type(x / mag, y / mag);
  }
  auto setMag(float magnitude) {
    const auto mag = this->magnitude();
    return class_type((x * magnitude) / mag, (y * magnitude) / mag);
  }
  auto limit(float max) {
    if (magnitude() > max) {
      return setMag(max);
    }
    return *this;
  }
  auto heading() const { return atan2(y, x); }
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;
