/* =======================================================================
   $File: sfs.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#ifndef PROJECT_DEF_VECTOR_HPP
#define PROJECT_DEF_VECTOR_HPP

#include <array>

namespace ost {

    //We adopt glm's union style inside vectors to represent
    //the same value with diferent names. e.x xyzw for space cordinates
    //rgba for color and stpq for texture cordinates

    template <typename T>
    struct tVec2 {

        tVec2<T>();
        tVec2<T>(T t_a, T t_b);
        tVec2<T>(const tVec2<T>& rhs);
        explicit tVec2<T>(T scalar);

        T& operator [](unsigned int index);
        const T& operator [](unsigned int index) const;

        /* get_index and set_index static methods used for lua-bindings */
        static T get_index(tVec2<T>& v, unsigned int i) {
            return v[i];
        }

        static void set_index(tVec2<T>& v, unsigned int i, T value) {
            v[i] = value;
        }

        void set(T a, T b);

        /* Unary arithmetic operators */

        template <typename U>
        tVec2<T>& operator=(const tVec2<U>& rhs);
        template <typename U>
        tVec2<T>& operator+=(U scalar);
        template <typename U>
        tVec2<T>& operator+=(const tVec2<U>& rhs);
        template <typename U>
        tVec2<T>& operator-=(U scalar);
        template <typename U>
        tVec2<T>& operator-=(const tVec2<U>& rhs);
        template <typename U>
        tVec2<T>& operator*=(U scalar);
        template <typename U>
        tVec2<T>& operator*=(const tVec2<U>& rhs);
        template <typename U>
        tVec2<T>& operator/=(U scalar);
        template <typename U>
        tVec2<T>& operator/=(const tVec2<U>& rhs);

        tVec2<T>& operator ++();
        tVec2<T>& operator --();
        const tVec2<T> operator ++(int);
        const tVec2<T> operator --(int);

        T               length() const;
        T               squareLength() const;

        union { T x, r, s; };
        union { T y, g, t; };
    };

    /* tVec2 Binary operators */

    template <typename T>
    tVec2<T> operator+(const tVec2<T>& lhs, T scalar);

    template <typename T>
    tVec2<T> operator+(T scalar, const tVec2<T>& rhs);

    template <typename T>
    tVec2<T> operator+(const tVec2<T>& lhs, const tVec2<T>& rhs);

    template <typename T>
    tVec2<T> operator-(const tVec2<T>& rhs, T scalar);

    template <typename T>
    tVec2<T> operator-(T scalar, const tVec2<T>& rhs);

    template <typename T>
    tVec2<T> operator-(const tVec2<T>& lhs, const tVec2<T>& rhs);

    template <typename T>
    tVec2<T> operator*(const tVec2<T>& lhs, T scalar);

    template <typename T>
    tVec2<T> operator*(T scalar, const tVec2<T>& rhs);

    template <typename T>
    tVec2<T> operator*(const tVec2<T>& lhs, const tVec2<T>& rhs);

    template <typename T>
    tVec2<T> operator/(const tVec2<T>& lhs, T scalar);

    template <typename T>
    tVec2<T> operator/(T scalar, const tVec2<T>& rhs);

    template <typename T>
    tVec2<T> operator/(const tVec2<T>& lhs, const tVec2<T>& rhs);

    template <typename T>
    bool operator==(const tVec2<T>& lhs, const tVec2<T>& rhs);

    template <typename T>
    bool operator!=(const tVec2<T>& lhs, const tVec2<T>& rhs);


    template <typename T>
    T dotProduct(const tVec2<T>& lhs, const tVec2<T>& rhs);

    template <typename T>
    struct tVec3 {

        union { T x, r, s; };
        union { T y, g, t; };
        union { T z, b, p; };

        tVec3<T>();
        tVec3<T>(T a,T b,T c);
        tVec3<T>(const tVec2<T>& lhs, T c);
        tVec3<T>(const tVec3<T>& rhs);
        explicit tVec3<T>(T scalar);
        explicit tVec3<T>(const std::array<T, 3>& array);

        void set(T a, T b, T c);
        void set(const tVec2<T>& lhs, T c);
        tVec2<T> xy() const;

        explicit operator std::array<T, 3>();

        tVec3<T>& operator =(const std::array<T, 3>& array);

        T &operator [](unsigned int index);
        const T &operator [](unsigned int index) const;
        
        /* get_index and set_index static methods used for lua-bindings */
        static T get_index(tVec3<T>& v, unsigned int i) {
            return v[i];
        }

        static void set_index(tVec3<T>& v, unsigned int i, T value) {
            v[i] = value;
        }

        /* Unary arithmetic operators */

        template <typename U>
        tVec3<T>& operator=(const tVec3<U>& rhs);
        template <typename U>
        tVec3<T>& operator+=(U scalar);
        template <typename U>
        tVec3<T>& operator+=(const tVec3<U>& rhs);
        template <typename U>
        tVec3<T>& operator-=(U scalar);
        template <typename U>
        tVec3<T>& operator-=(const tVec3<U>& rhs);
        template <typename U>
        tVec3<T>& operator*=(U scalar);
        template <typename U>
        tVec3<T>& operator*=(const tVec3<U>& rhs);
        template <typename U>
        tVec3<T>& operator/=(U scalar);
        template <typename U>
        tVec3<T>& operator/=(const tVec3<U>& rhs);

        tVec3<T>& operator -();

        /* Increment and decrement operators */

        tVec3<T>& operator++();
        tVec3<T>& operator--();
        const tVec3<T> operator++(int);
        const tVec3<T> operator--(int);


        void normalize();
        tVec3<T> normalized() const;
        T length() const;
        T squaredLength() const;

    };

    template <typename T>
    tVec3<T> operator+(const tVec3<T>& lhs, T scalar);

    template <typename T>
    tVec3<T> operator+(T scalar, const tVec3<T>& lhs);

    template <typename T>
    tVec3<T> operator+(const tVec3<T>& lhs, const tVec3<T>& rhs);

    template <typename T>
    tVec3<T> operator-(const tVec3<T>& lhs, T scalar);

    template <typename T>
    tVec3<T> operator-(T scalar, const tVec3<T>& rhs);

    template <typename T>
    tVec3<T> operator-(const tVec3<T>& lhs, const tVec3<T>& rhs);

    template <typename T>
    tVec3<T> operator*(const tVec3<T>& lhs, T scalar);

    template <typename T>
    tVec3<T> operator*(T scalar, const tVec3<T>& rhs);

    template <typename T>
    tVec3<T> operator*(const tVec3<T>& lhs, const tVec3<T>& rhs);

    template <typename T>
    tVec3<T> operator/(const tVec3<T>& lhs, T scalar);

    template <typename T>
    tVec3<T> operator/(T scalar, const tVec3<T>& rhs);

    template <typename T>
    tVec3<T> operator/(const tVec3<T>& lhs, const tVec3<T>& rhs);

    template <typename T>
    bool operator==(const tVec3<T>& lhs, const tVec3<T>& rhs);

    template <typename T>
    bool operator!=(const tVec3<T>& lhs, const tVec3<T>& rhs);

    template <typename T>
    T dotProduct(const tVec3<T>& lhs, const tVec3<T>& rhs);

    template <typename T>
    tVec3<T> crossProduct(const tVec3<T>& lhs, const tVec3<T>& rhs);

    template <typename T>
    void lerp(const tVec3<T>& lhs, const tVec3<T>& rhs, tVec3<T>* out, T val);

    template <typename T>
    struct tVec4 {

        //The idea with union come up from
        //Glew library, so credits goes to Glew library
        union { T x, r, s; };
        union { T y, g, t; };
        union { T z, b, p; };
        union { T w, a, q; };

        tVec4<T>();
        tVec4<T>(T a, T b, T c, T d);
        tVec4<T>(const tVec3<T>& vec, T d);
        tVec4<T>(const tVec4<T>& rhs);
        tVec4<T>(T scalar);

        tVec3<T> xyz();
        void set(T a, T b, T c, T d);
        operator tVec3<T>();

        T& operator [](unsigned int index);
        const T& operator [](unsigned int index) const;

        /* get_index and set_index static methods used for lua-bindings */
        static T get_index(tVec4<T>& v, unsigned int i) {
            return v[i];
        }

        static void set_index(tVec4<T>& v, unsigned int i, T value) {
            v[i] = value;
        }

        /* Unary arithmetic operators */

        template <typename U>
        tVec4<T>& operator=(const tVec4<U>& rhs);
        template <typename U>
        tVec4<T>& operator+=(U scalar);
        template <typename U>
        tVec4<T>& operator+=(const tVec4<U>& rhs);
        template <typename U>
        tVec4<T>& operator-=(U scalar);
        template <typename U>
        tVec4<T>& operator-=(const tVec4<U>& rhs);
        template <typename U>
        tVec4<T>& operator*=(U scalar);
        template <typename U>
        tVec4<T>& operator*=(const tVec4<U>& rhs);
        template <typename U>
        tVec4<T>& operator/=(U scalar);
        template <typename U>
        tVec4<T>& operator/=(const tVec4<U>& rhs);

        /* Increment and decrement operators */

        tVec4<T>& operator++();
        tVec4<T>& operator--();
        const tVec4<T> operator++(int);
        const tVec4<T> operator--(int);

        T length() const;
        T sqaredLength() const;
        void normilize();

    };

    /* tVec4 arithmetic binary operators */

    template <typename T>
    tVec4<T> operator+(const tVec4<T>& lhs, T scalar);

    template <typename T>
    tVec4<T> operator+(T scalar, const tVec4<T>& rhs);

    template <typename T>
    tVec4<T> operator+(const tVec4<T>& lhs, const tVec4<T>& rhs);

    template <typename T>
    tVec4<T> operator-(const tVec4<T>& lhs, T scalar);

    template <typename T>
    tVec4<T> operator-(T scalar, const tVec4<T>& rhs);

    template <typename T>
    tVec4<T> operator-(const tVec4<T>& lhs, const tVec4<T>& rhs);

    template <typename T>
    tVec4<T> operator*(const tVec4<T>& lhs, T scalar);

    template <typename T>
    tVec4<T> operator*(T scalar, const tVec4<T>& rhs);

    template <typename T>
    tVec4<T> operator*(const tVec4<T>& lhs, const tVec4<T>& rhs);

    template <typename T>
    tVec4<T> operator/(const tVec4<T>& lhs, T scalar);

    template <typename T>
    tVec4<T> operator/(T scalar, const tVec4<T>& rhs);

    template <typename T>
    tVec4<T> operator/(const tVec4<T>& lhs, const tVec4<T>& rhs);

    template <typename T>
    bool operator==(const tVec4<T>& lhs, const tVec4<T>& rhs);

    template <typename T>
    bool operator!=(const tVec4<T>& lhs, const tVec4<T>& rhs);


    template <typename T>
    T dotProduct(const tVec4<T>& lhs, const tVec4<T>& rhs);

    template <typename T>
    void lerp(const tVec4<T>& lhs, const tVec4<T>& rhs, tVec4<T>* out, T t);
}

#include "def_vector.inl"
#endif //PROJECT_DEF_VECTOR_HPP
