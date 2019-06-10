/* =======================================================================
   $File: sfs.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

//C++ Includes
#include <cassert>
#include <cmath>

namespace ost {

  template <typename T>
  tVec2<T>::tVec2()
    : x(static_cast<T>(0.0f)), y(static_cast<T>(0.0f)) { }

  template <typename T>
  tVec2<T>::tVec2(T t_a, T t_b)
    : x(t_a), y(t_b) { }

  template <typename T>
  tVec2<T>::tVec2(const tVec2<T> &rhs)
    : x(rhs.x), y(rhs.y) { }

  template <typename T>
  tVec2<T>::tVec2(T scalar)
    : x(scalar), y(scalar) { }

  template <typename T>
  T &tVec2<T>::operator[](unsigned int index) {

    assert(index < 2);
    if ( index == 0 ) return x;
    if ( index == 1 ) return y;
  }

  template <typename T>
  const T &tVec2<T>::operator[](unsigned int index) const {

    assert(index < 2);
    if ( index == 0 ) return x;
    if ( index == 1 ) return y;
  }

  template <typename T>
  void tVec2<T>::set(T a, T b) {

    this->x = static_cast<T>(a);
    this->y = static_cast<T>(b);
  }

  template <typename T>
  T tVec2<T>::length() const {

    T _x = this->x;
    T _y = this->y;
    return static_cast<T> (sqrt(_x * _x + _y * _y));
  }

  template <typename T>
  T tVec2<T>::squareLength() const {

    T _x = this->x;
    T _y = this->y;
    return _x * _x + _y * _y;
  }

  template <typename T>
  tVec2<T> &tVec2<T>::operator++() {

    ++this->x;
    ++this->y;
    return *this;
  }

  template <typename T>
  tVec2<T> &tVec2<T>::operator--() {

    --this->x;
    --this->y;
    return *this;
  }

  template <typename T>
  const tVec2<T> tVec2<T>::operator++(int) {

    tVec2<T> result(*this);
    ++*this;
    return result;
  }

  template <typename T>
  const tVec2<T> tVec2<T>::operator--(int) {

    tVec2 result(*this);
    --*this;
    return result;
  }

  template <typename T>
  template <typename U>
  tVec2<T> &tVec2<T>::operator=(const tVec2<U> &rhs) {

    this->x = rhs.x;
    this->y = rhs.y;
    return *this;
  }

  template <typename T>
  template <typename U>
  tVec2<T> &tVec2<T>::operator+=(U scalar) {

    this->x += scalar;
    this->y += scalar;
    return *this;

  }

  template <typename T>
  template <typename U>
  tVec2<T> &tVec2<T>::operator+=(const tVec2<U> &rhs) {

    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
  }

  template <typename T>
  template <typename U>
  tVec2<T> &tVec2<T>::operator-=(U scalar) {

    this->x -= scalar;
    this->y -= scalar;
    return *this;

  }

  template <typename T>
  template <typename U>
  tVec2<T> &tVec2<T>::operator-=(const tVec2<U> &rhs) {

    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;

  }

  template <typename T>
  template <typename U>
  tVec2<T> &tVec2<T>::operator*=(U scalar) {

    this->x *= scalar;
    this->y *= scalar;
    return *this;

  }

  template <typename T>
  template <typename U>
  tVec2<T> &tVec2<T>::operator*=(const tVec2<U> &rhs) {

    this->x *= rhs.x;
    this->y *= rhs.y;
    return *this;

  }

  template <typename T>
  template <typename U>
  tVec2<T> &tVec2<T>::operator/=(U scalar) {

    this->x /= scalar;
    this->y /= scalar;
    return *this;
  }

  template <typename T>
  template <typename U>
  tVec2<T> &tVec2<T>::operator/=(const tVec2<U> &rhs) {

    this->x /= rhs.x;
    this->y /= rhs.y;
    return *this;
  }

  template <typename T>
  tVec2<T> operator+(const tVec2<T> &lhs, T scalar) {

    return tVec2<T>(
      lhs.x + scalar,
      lhs.y + scalar
    );
  }

  template <typename T>
  tVec2<T> operator+(T scalar, const tVec2<T> &rhs) {

    return tVec2<T>(
      scalar + rhs.x,
      scalar + rhs.y
    );
  }

  template <typename T>
  tVec2<T> operator+(const tVec2<T> &lhs, const tVec2<T> &rhs) {

    return tVec2<T>(
      lhs.x + rhs.x,
      lhs.y + rhs.y
    );
  }

  template <typename T>
  tVec2<T> operator-(const tVec2<T> &lhs, T scalar) {

    return tVec2<T>(
      lhs.x - scalar,
      lhs.y - scalar
    );
  }

  template <typename T>
  tVec2<T> operator-(T scalar, const tVec2<T> &rhs) {

    return tVec2<T>(
      scalar - rhs.x,
      scalar - rhs.y
    );
  }

  template <typename T>
  tVec2<T> operator-(const tVec2<T> &lhs, const tVec2<T> &rhs) {

    return tVec2<T>(
      lhs.x - rhs.x,
      lhs.y - rhs.y
    );
  }

  template <typename T>
  tVec2<T> operator*(const tVec2<T> &lhs, T scalar) {

    return tVec2<T>(
      lhs.x * scalar,
      lhs.y * scalar
    );
  }

  template <typename T>
  tVec2<T> operator*(T scalar, const tVec2<T> &rhs) {

    return tVec2<T>(
      scalar * rhs.x,
      scalar * rhs.y
    );
  }

  template <typename T>
  tVec2<T> operator*(const tVec2<T> &lhs, const tVec2<T> &rhs) {

    return tVec2<T>(
      lhs.x * rhs.x,
      lhs.y * rhs.y
    );
  }

  template <typename T>
  tVec2<T> operator/(const tVec2<T> &lhs, T scalar) {

    return tVec2<T>(
      lhs.x / scalar,
      lhs.y / scalar
    );
  }

  template <typename T>
  tVec2<T> operator/(T scalar, const tVec2<T> &rhs) {

    return tVec2<T>(
      scalar / rhs.x,
      scalar / rhs.y
    );
  }

  template <typename T>
  tVec2<T> operator/(const tVec2<T> &lhs, const tVec2<T> &rhs) {

    return tVec2<T>(
      lhs.x / rhs.x,
      lhs.y / rhs.y
    );
  }

  template <typename T>
  bool operator==(const tVec2<T> &lhs, const tVec2<T> &rhs) {

    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
  }

  template <typename T>
  bool operator!=(const tVec2<T> &lhs, const tVec2<T> &rhs) {

    return (lhs.x != rhs.x) || (lhs.y != rhs.y);
  }

  template <typename T>
  inline T dotProduct(const tVec2<T> &lhs, const tVec2<T> &rhs) {

    return lhs.x * rhs.x + lhs.y * rhs.y;
  }

  template <typename T>
  tVec3<T>::tVec3()
    : x(static_cast<T>(0.0f)), y(static_cast<T>(0.0f)), z(static_cast<T>(0.0f)) { }

  template <typename T>
  tVec3<T>::tVec3(T t_a, T t_b, T t_c)
    : x(static_cast<T>(t_a)), y(static_cast<T>(t_b)), z(static_cast<T>(t_c)) { }

  template <typename T>
  tVec3<T>::tVec3(const tVec2<T> &lhs, T t_c)
    : x(lhs.x), y(lhs.y), z(static_cast<T>(t_c)) { }

  template <typename T>
  tVec3<T>::tVec3(const tVec3<T> &rhs)
    : x(rhs.x), y(rhs.y), z(rhs.z) { }

  template <typename T>
  tVec3<T>::tVec3(T scalar)
    : x(scalar), y(scalar), z(scalar) { }

  template <typename T>
  tVec3<T>& tVec3<T>::operator =(const std::array<T, 3>& array) {
    this->x = array[0];
    this->y = array[1];
    this->z = array[2];

    return *this;
  }

  template <typename T>
  tVec3<T>::tVec3(const std::array<T, 3>& array)
    : x(array[0]), y(array[1]), z(array[2]) { }

  template <typename T>
  void tVec3<T>::set(T t_a, T t_b, T t_c) {

    this->x = static_cast<T>(t_a);
    this->y = static_cast<T>(t_b);
    this->z = static_cast<T>(t_c);
  }

  template <typename T>
  void tVec3<T>::set(const tVec2<T> &lhs, T t_c) {

    this->x = lhs.x;
    this->y = lhs.y;
    this->z = static_cast<T>(t_c);
  }

  template <typename T>
  tVec2<T> tVec3<T>::xy() const {

    return tVec2<T>(
      x,
      y
    );
  }

  template <typename T>
  T tVec3<T>::length() const {

    T _x = this->x;
    T _y = this->y;
    T _z = this->z;
    return static_cast<T>(sqrt(_x * _x + _y * _y + _z * _z));
  }

  template <typename T>
  T tVec3<T>::squaredLength() const {

    T _x = this->x;
    T _y = this->y;
    T _z = this->z;
    return _x * _x + _y * _y + _z * _z;
  }

  template <typename T>
  void tVec3<T>::normalize() {

    T _x = this->x;
    T _y = this->y;
    T _z = this->z;
    T inv_len = 1 / static_cast<float>(sqrt(_x * _x + _y * _y + _z * _z));
    _x *= inv_len;
    _y *= inv_len;
    _z *= inv_len;
    this->x = _x;
    this->y = _y;
    this->z = _z;
  }

  template <typename T>
  tVec3<T> tVec3<T>::normalized() const {

    float _x = this->x;
    float _y = this->y;
    float _z = this->z;
    float inv_len = 1 / static_cast<float>(sqrt(_x * _x + _y * _y + _z * _z));
    _x *= inv_len;
    _y *= inv_len;
    _z *= inv_len;
    return tVec3<T>(_x, _y, _z);
  }

  template <typename T>
  tVec3<T>::operator std::array<T, 3>() {

    std::array<T, 3> tmp = { x, y, z };
    return tmp;
  }

  template <typename T>
  T &tVec3<T>::operator[](unsigned int index) {

    assert(index < 3);

//        if (index == 0) return x;
//        if (index == 1) return y;
//        if (index == 2) return z;
    return (&x)[index];
  }

  template <typename T>
  const T &tVec3<T>::operator[](unsigned int index) const {

    assert(index < 3);

//        if (index == 0) return x;
//        if (index == 1) return y;
//        if (index == 2) return z;
    return (&x)[index];
  }

  template <typename T>
  template <typename U>
  tVec3<T> &tVec3<T>::operator=(const tVec3<U> &rhs) {

    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;

    return *this;

  }

  template <typename T>
  template <typename U>
  tVec3<T> &tVec3<T>::operator+=(U scalar) {

    this->x += scalar;
    this->y += scalar;
    this->z += scalar;
    return *this;

  }

  template <typename T>
  template <typename U>
  tVec3<T> &tVec3<T>::operator+=(const tVec3<U> &rhs) {

    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
  }

  template <typename T>
  template <typename U>
  tVec3<T> &tVec3<T>::operator-=(U scalar) {

    this->x -= scalar;
    this->y -= scalar;
    this->z -= scalar;
    return *this;
  }

  template <typename T>
  template <typename U>
  tVec3<T> &tVec3<T>::operator-=(const tVec3<U> &rhs) {

    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
  }

  template <typename T>
  template <typename U>
  tVec3<T> &tVec3<T>::operator*=(U scalar) {

    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
  }

  template <typename T>
  template <typename U>
  tVec3<T> &tVec3<T>::operator*=(const tVec3<U> &rhs) {

    this->x *= rhs.x;
    this->y *= rhs.y;
    this->z *= rhs.z;
    return *this;
  }

  template <typename T>
  template <typename U>
  tVec3<T> &tVec3<T>::operator/=(U scalar) {

    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
    return *this;
  }

  template <typename T>
  template <typename U>
  tVec3<T> &tVec3<T>::operator/=(const tVec3<U> &rhs) {

    this->x /= rhs.x;
    this->y /= rhs.y;
    this->z /= rhs.z;
    return *this;
  }

  template <typename T>
  tVec3<T> &tVec3<T>::operator-() {

    -this->x;
    -this->y;
    -this->z;
    return *this;
  }


  template <typename T>
  tVec3<T> &tVec3<T>::operator++() {

    ++this->x;
    ++this->y;
    ++this->z;
    return *this;
  }

  template <typename T>
  tVec3<T> &tVec3<T>::operator--() {

    --this->x;
    --this->y;
    --this->z;
    return *this;
  }

  template <typename T>
  const tVec3<T> tVec3<T>::operator++(int) {

    tVec3<T> Result(*this);
    ++*this;
    return Result;
  }

  template <typename T>
  const tVec3<T> tVec3<T>::operator--(int) {

    tVec3 Result(*this);
    --*this;
    return Result;
  }

  template <typename T>
  tVec3<T> operator+(const tVec3<T> &lhs, T scalar) {

    return tVec3<T>(
      lhs.x + scalar,
      lhs.y + scalar,
      lhs.z + scalar
    );
  }

  template <typename T>
  tVec3<T> operator+(T scalar, const tVec3<T> &rhs) {

    return tVec3<T>(
      scalar + rhs.x,
      scalar + rhs.y,
      scalar + rhs.z
    );
  }

  template <typename T>
  tVec3<T> operator+(const tVec3<T> &lhs, const tVec3<T> &rhs) {

    return tVec3<T>(
      lhs.x + rhs.x,
      lhs.y + rhs.y,
      lhs.z + rhs.z
    );
  }

  template <typename T>
  tVec3<T> operator-(const tVec3<T> &lhs, T scalar) {

    return tVec3<T>(
      lhs.x - scalar,
      lhs.y - scalar,
      lhs.z - scalar
    );
  }

  template <typename T>
  tVec3<T> operator-(T scalar, const tVec3<T> &rhs) {

    return tVec3<T>(
      scalar - rhs.x,
      scalar - rhs.y,
      scalar - rhs.z
    );
  }

  template <typename T>
  tVec3<T> operator-(const tVec3<T> &lhs, const tVec3<T> &rhs) {

    return tVec3<T>(
      lhs.x - rhs.x,
      lhs.y - rhs.y,
      lhs.z - rhs.z
    );
  }

  template <typename T>
  tVec3<T> operator*(const tVec3<T> &lhs, T scalar) {

    return tVec3<T>(
      lhs.x * scalar,
      lhs.y * scalar,
      lhs.z * scalar
    );
  }

  template <typename T>
  tVec3<T> operator*(T scalar, const tVec3<T> &rhs) {

    return tVec3<T>(
      scalar * rhs.x,
      scalar * rhs.y,
      scalar * rhs.z
    );
  }

  template <typename T>
  tVec3<T> operator*(const tVec3<T> &lhs, const tVec3<T> &rhs) {

    return tVec3<T>(
      lhs.x * rhs.x,
      lhs.y * rhs.y,
      lhs.z * rhs.z
    );
  }

  template <typename T>
  tVec3<T> operator/(const tVec3<T> &lhs, T scalar) {

    return tVec3<T>(
      lhs.x / scalar,
      lhs.y / scalar,
      lhs.z / scalar
    );
  }

  template <typename T>
  tVec3<T> operator/(T scalar, const tVec3<T> &rhs) {

    return tVec3<T>(
      scalar / rhs.x,
      scalar / rhs.y,
      scalar / rhs.z
    );
  }

  template <typename T>
  tVec3<T> operator/(const tVec3<T> &lhs, const tVec3<T> &rhs) {

    return tVec3<T>(
      lhs.x / rhs.x,
      lhs.y / rhs.y,
      lhs.z / rhs.z
    );
  }

  template <typename T>
  bool operator==(const tVec3<T> &lhs, const tVec3<T> &rhs) {

    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
  }

  template <typename T>
  bool operator!=(const tVec3<T> &lhs, const tVec3<T> &rhs) {

    return (lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z);
  }

  template <typename T>
  inline T dotProduct(const tVec3<T> &lhs, const tVec3<T> &rhs) {

    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
  }

  template <typename T>
  inline tVec3<T> crossProduct(const tVec3<T> &lhs, const tVec3<T> &rhs) {

    return tVec3<T>(lhs.y * rhs.z - lhs.z * rhs.y,
                    lhs.z * rhs.x - lhs.x * rhs.z,
                    lhs.x * rhs.y - lhs.y * rhs.x);
  }

  template <typename T>
  inline void lerp(const tVec3<T> &lhs, const tVec3<T> &rhs, tVec3<T> *out, T val) {
    /*
    * Linearly interpolates between two vectors.
    * Interpolates between the vectors lhs and rhs by the interpolant t.
    * The parameter t is clamped to the range [0, 1].
    * This is most commonly used to find a point some fraction of the way along a line between two endpoints
    * (e.g. to move an object gradually between those points).
    */

    T invt = 1.0f - val;
    out->x = lhs.x * invt + rhs.x * val;
    out->y = lhs.y * invt + rhs.y * val;
    out->z = lhs.z * invt + rhs.z * val;
  }

  template <typename T>
  tVec4<T>::tVec4()
    : x(static_cast<T>(0.0f)), y(static_cast<T>(0.0f)), z(static_cast<T>(0.0f)), w(static_cast<T>(0.0f)) { }

  template <typename T>
  tVec4<T>::tVec4(T t_a, T t_b, T t_c, T t_d)
    : x(static_cast<T>(t_a)), y(static_cast<T>(t_b)), z(static_cast<T>(t_c)), w(static_cast<T>(t_d)) { }

  template <typename T>
  tVec4<T>::tVec4(const tVec3<T> &vec, T t_d)
    : x(vec.x), y(vec.y), z(vec.z), w(static_cast<T>(t_d)) { }

  template <typename T>
  tVec4<T>::tVec4(const tVec4<T> &rhs)
    : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) { }

  template <typename T>
  tVec4<T>::tVec4(T scalar)
    : x(scalar), y(scalar), z(scalar), w(scalar) { }

  template <typename T>
  tVec3<T> tVec4<T>::xyz() {

    return tVec3<T>(
      x,
      y,
      z
    );
  }

  template <typename T>
  void tVec4<T>::set(T t_a, T t_b, T t_c, T t_d) {

    this->x = static_cast<T>(t_a);
    this->y = static_cast<T>(t_b);
    this->z = static_cast<T>(t_c);
    this->w = static_cast<T>(t_d);
  }

  template <typename T>
  tVec4<T>::operator tVec3<T>() {

    return tVec3<T>(
      x,
      y,
      z
    );
  }

  template <typename T>
  T tVec4<T>::length() const {

    T _x = this->x;
    T _y = this->y;
    T _z = this->z;
    T _w = this->w;

    return static_cast<float>(sqrt(_x * _x + _y * _y + _z * _z + _w * _w));
  }

  template <typename T>
  T tVec4<T>::sqaredLength() const {

    T _x = this->x;
    T _y = this->y;
    T _z = this->z;
    T _w = this->w;

    return (_x * _x + _y * _y + _z * _z + _w * _w);
  }

  template <typename T>
  void tVec4<T>::normilize() {

    T _x = this->x;
    T _y = this->y;
    T _z = this->z;
    T _w = this->w;
    T inv_len = 1 / static_cast<float>(sqrt(_x * _x + _y * _y + _z * _z + _w * _w));
    _x *= inv_len;
    _y *= inv_len;
    _z *= inv_len;
    this->x = _x;
    this->y = _y;
    this->z = _z;
    this->w = _w;
  }

  template <typename T>
  T &tVec4<T>::operator[](unsigned int index) {

    assert(index < 4);

//        if (index == 0) return x;
//        if (index == 1) return y;
//        if (index == 2) return z;
//        if (index == 3) return w;
    return (&x)[index];

  }

  template <typename T>
  const T &tVec4<T>::operator[](unsigned int index) const {

    assert(index < 4);

//        if (index == 0) return x;
//        if (index == 1) return y;
//        if (index == 2) return z;
//        if (index == 3) return w;
    return (&x)[index];
  }

  template <typename T>
  template <typename U>
  tVec4<T> &tVec4<T>::operator=(const tVec4<U> &rhs) {

    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->w = rhs.w;

    return *this;
  }

  template <typename T>
  template <typename U>
  tVec4<T> &tVec4<T>::operator+=(U scalar) {

    this->x += scalar;
    this->y += scalar;
    this->z += scalar;
    this->w += scalar;

    return *this;
  }

  template <typename T>
  template <typename U>
  tVec4<T> &tVec4<T>::operator+=(const tVec4<U> &rhs) {

    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    this->w += rhs.w;

    return *this;
  }

  template <typename T>
  template <typename U>
  tVec4<T> &tVec4<T>::operator-=(U scalar) {

    this->x -= scalar;
    this->y -= scalar;
    this->z -= scalar;
    this->w -= scalar;

    return *this;
  }

  template <typename T>
  template <typename U>
  tVec4<T> &tVec4<T>::operator-=(const tVec4<U> &rhs) {

    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.w;
    this->w -= rhs.z;

    return *this;
  }

  template <typename T>
  template <typename U>
  tVec4<T> &tVec4<T>::operator*=(U scalar) {

    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    this->w *= scalar;

    return *this;
  }

  template <typename T>
  template <typename U>
  tVec4<T> &tVec4<T>::operator*=(const tVec4<U> &rhs) {

    this->x *= rhs.x;
    this->y *= rhs.y;
    this->z *= rhs.z;
    this->w *= rhs.w;

    return *this;
  }

  template <typename T>
  template <typename U>
  tVec4<T> &tVec4<T>::operator/=(U scalar) {

    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
    this->w /= scalar;

    return *this;
  }


  template <typename T>
  template <typename U>
  tVec4<T> &tVec4<T>::operator/=(const tVec4<U> &rhs) {

    this->x /= rhs.x;
    this->y /= rhs.y;
    this->z /= rhs.z;
    this->w /= rhs.w;

    return *this;
  }

  template <typename T>
  tVec4<T> &tVec4<T>::operator++() {

    ++this->x;
    ++this->y;
    ++this->z;
    ++this->w;
    return *this;

  }

  template <typename T>
  tVec4<T> &tVec4<T>::operator--() {

    --this->x;
    --this->y;
    --this->z;
    --this->w;
    return *this;
  }

  template <typename T>
  const tVec4<T> tVec4<T>::operator++(int) {

    tVec4<T> result(*this);
    ++*this;
    return result;
  }

  template <typename T>
  const tVec4<T> tVec4<T>::operator--(int) {

    tVec4<T> result(*this);
    --*this;
    return result;
  }

  template <typename T>
  tVec4<T> operator+(const tVec4<T> &lhs, T scalar) {

    return tVec4<T>(
      lhs.x + scalar,
      lhs.y + scalar,
      lhs.z + scalar,
      lhs.w + scalar
    );
  }

  template <typename T>
  tVec4<T> operator+(T scalar, const tVec4<T> &rhs) {

    return tVec4<T>(
      scalar + rhs.x,
      scalar + rhs.y,
      scalar + rhs.z,
      scalar + rhs.w
    );
  }

  template <typename T>
  tVec4<T> operator+(const tVec4<T> &lhs, const tVec4<T> &rhs) {

    return tVec4<T>(
      lhs.x + rhs.x,
      lhs.y + rhs.y,
      lhs.z + rhs.z,
      lhs.w + rhs.w
    );
  }

  template <typename T>
  tVec4<T> operator-(const tVec4<T> &lhs, T scalar) {

    return tVec4<T>(
      lhs.x - scalar,
      lhs.y - scalar,
      lhs.z - scalar,
      lhs.w - scalar
    );
  }

  template <typename T>
  tVec4<T> operator-(T scalar, const tVec4<T> &rhs) {

    return tVec4<T>(
      scalar - rhs.x,
      scalar - rhs.y,
      scalar - rhs.z,
      scalar - rhs.w
    );
  }

  template <typename T>
  tVec4<T> operator-(const tVec4<T> &lhs, const tVec4<T> &rhs) {

    return tVec4<T>(
      lhs.x - rhs.x,
      lhs.y - rhs.y,
      lhs.z - rhs.z,
      lhs.w - rhs.w
    );
  }

  template <typename T>
  tVec4<T> operator*(const tVec4<T> &lhs, T scalar) {

    return tVec4<T>(
      lhs.x * scalar,
      lhs.y * scalar,
      lhs.z * scalar,
      lhs.w * scalar
    );
  }

  template <typename T>
  tVec4<T> operator*(T scalar, const tVec4<T> &rhs) {

    return tVec4<T>(
      scalar * rhs.x,
      scalar * rhs.y,
      scalar * rhs.z,
      scalar * rhs.w
    );
  }

  template <typename T>
  tVec4<T> operator*(const tVec4<T> &lhs, const tVec4<T> &rhs) {

    return tVec4<T>(
      lhs.x * rhs.x,
      lhs.y * rhs.y,
      lhs.z * rhs.z,
      lhs.w * rhs.w
    );
  }

  template <typename T>
  tVec4<T> operator/(const tVec4<T> &lhs, T scalar) {

    return tVec4<T>(
      lhs.x / scalar,
      lhs.y / scalar,
      lhs.z / scalar,
      lhs.w / scalar
    );
  }

  template <typename T>
  tVec4<T> operator/(T scalar, const tVec4<T> &rhs) {

    return tVec4<T>(
      scalar / rhs.x,
      scalar / rhs.y,
      scalar / rhs.z,
      scalar / rhs.w
    );
  }

  template <typename T>
  tVec4<T> operator/(const tVec4<T> &lhs, const tVec4<T> &rhs) {

    return tVec4<T>(
      lhs.x / rhs.x,
      lhs.y / rhs.y,
      lhs.z / rhs.z,
      lhs.w / rhs.w
    );
  }

  template <typename T>
  bool operator==(const tVec4<T> &lhs, const tVec4<T> &rhs) {

    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
  }

  template <typename T>
  bool operator!=(const tVec4<T> &lhs, const tVec4<T> &rhs) {

    return (lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z) || (lhs.w != rhs.w);
  }

  template <typename T>
  inline T dotProduct(const tVec4<T> &lhs, const tVec4<T> &rhs) {

    return lhs.x * rhs.x + lhs.y * rhs.y * lhs.z * rhs.z + lhs.w * rhs.w;
  }

  template <typename T>
  inline void lerp(const tVec4<T> &lhs, const tVec4<T> &rhs, tVec4<T> *out, T t) {

    /*
    * Linearly interpolates between two vectors.
    * Interpolates between the vectors lhs and rhs by the interpolant t.
    * The parameter t is clamped to the range [0, 1].
    * This is most commonly used to find a point some fraction of the way along a line between two endpoints
    * (e.g. to move an object gradually between those points).
    */

    T invt = 1.0f - t;
    out->x = lhs.x * invt + rhs.x * t;
    out->y = lhs.y * invt + rhs.y * t;
    out->z = lhs.z * invt + rhs.z * t;
    out->w = lhs.w * invt + rhs.w * t;

  }

}
