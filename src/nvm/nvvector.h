//
// Template math library for common 3D functionality
//
// nvVector.h - 2-vector, 3-vector, and 4-vector templates and utilities
//
// This code is in part deriver from glh, a cross platform glut helper library.
// The copyright for glh follows this notice.
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/*
    Copyright (c) 2000 Cass Everitt
	Copyright (c) 2000 NVIDIA Corporation
    All rights reserved.

    Redistribution and use in source and binary forms, with or
	without modification, are permitted provided that the following
	conditions are met:

     * Redistributions of source code must retain the above
	   copyright notice, this list of conditions and the following
	   disclaimer.

     * Redistributions in binary form must reproduce the above
	   copyright notice, this list of conditions and the following
	   disclaimer in the documentation and/or other materials
	   provided with the distribution.

     * The names of contributors to this software may not be used
	   to endorse or promote products derived from this software
	   without specific prior written permission. 

       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	   REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
	   POSSIBILITY OF SUCH DAMAGE. 


    Cass Everitt - cass@r3.nu
*/


#ifndef NV_VECTOR_H
#define NV_VECTOR_H

/*
#ifndef NO_FX_VECTOR
#include "PxPhysicsAPI.h"
#endif
*/
namespace nv {

template <class T> class VEC2;
template <class T> class VEC3;
template <class T> class VEC4;

//////////////////////////////////////////////////////////////////////
//
// VEC2 - template class for 2-tuple vector
//
//////////////////////////////////////////////////////////////////////
template <class T>   
class VEC2 {
public:

    typedef T value_type;
    int size() const { return 2;}

    ////////////////////////////////////////////////////////
    //
    //  Constructors
    //
    ////////////////////////////////////////////////////////

    // Default/scalar constructor
    VEC2(const T & t = T()) {
        for(int i = 0; i < size(); i++) _array[i] = t;
    }

    // Construct from array
    VEC2(const T * tp) {
        for(int i = 0; i < size(); i++) _array[i] = tp[i];
    }

    // Construct from explicit values
    VEC2( const T v0, const T v1) {
        x = v0;
        y = v1;
    }

    explicit VEC2( const VEC3<T> &u) {
        for(int i = 0; i < size(); i++) _array[i] = u._array[i];
    }

    explicit VEC2( const VEC4<T> &u) {
        for(int i = 0; i < size(); i++) _array[i] = u._array[i];
    }
    
    const T * get_value() const {
        return _array;
    }
    
    VEC2<T> & set_value( const T * rhs ) {
        for(int i = 0; i < size(); i++) _array[i] = rhs[i];
        return *this;
    }
    
    // indexing operators
    T & operator [] ( int i ) {
        return _array[i];
    }
    
    const T & operator [] ( int i ) const {
        return _array[i];
    }

    // type-cast operators
    operator T * () {
        return _array;
    }

    operator const T * () const {
        return _array;
    }

    ////////////////////////////////////////////////////////
    //
    //  Math operators
    //
    ////////////////////////////////////////////////////////

    // scalar multiply assign
    friend VEC2<T> & operator *= ( VEC2<T> &lhs, T d ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= d;
        return lhs;
    }

    // component-wise vector multiply assign
    friend VEC2<T> & operator *= ( VEC2<T> &lhs, const VEC2<T> &rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= rhs[i];
        return lhs;
    }

    // scalar divide assign
    friend VEC2<T> & operator /= ( VEC2<T> &lhs, T d ) {
        if(d == 0) return lhs;
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= d;
        return lhs;
    }

    // component-wise vector divide assign
    friend VEC2<T> & operator /= ( VEC2<T> &lhs, const VEC2<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= rhs._array[i];
        return lhs;
    }

    // component-wise vector add assign
    friend VEC2<T> & operator += ( VEC2<T> &lhs, const VEC2<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] += rhs._array[i];
        return lhs;
    }

    // component-wise vector subtract assign
    friend VEC2<T> & operator -= ( VEC2<T> &lhs, const VEC2<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] -= rhs._array[i];
        return lhs;
    }

    // unary negate
    friend VEC2<T> operator - ( const VEC2<T> &rhs) {
        VEC2<T> rv;
        for(int i = 0; i < rhs.size(); i++) rv._array[i] = -rhs._array[i];
        return rv;
    }

    // vector add
    friend VEC2<T> operator + ( const VEC2<T> & lhs, const VEC2<T> & rhs) {
        VEC2<T> rt(lhs);
        return rt += rhs;
    }

    // vector subtract 
    friend VEC2<T> operator - ( const VEC2<T> & lhs, const VEC2<T> & rhs) {
        VEC2<T> rt(lhs);
        return rt -= rhs;
    }

	//	ну я понимаю конечно они на неявное приведение через конструктор 
	//	надеялись, но гуля их обломал
	// scalar сумма,, Сука, бля НВИДИА небыло. хорошо компилятор от NDK помог
    friend VEC2<T> operator + ( const VEC2<T> & lhs, T rhs) {
		VEC2<T> rt(lhs);
		rt.x += rhs;
		rt.y += rhs;
        return rt;
    }
    
    // scalar сумма,, Сука, бля НВИДИА небыло. хорошо компилятор от NDK помог
    friend VEC2<T> operator - ( const VEC2<T> & lhs, T rhs) {
		VEC2<T> rt(lhs);
		rt.x -= rhs;
		rt.y -= rhs;
        return rt;
    }
    
    // scalar multiply
    friend VEC2<T> operator * ( const VEC2<T> & lhs, T rhs) {
        VEC2<T> rt(lhs);
        return rt *= rhs;
    }

    // scalar multiply
    friend VEC2<T> operator * ( T lhs, const VEC2<T> & rhs) {
        VEC2<T> rt(lhs);
        return rt *= rhs;
    }

    // vector component-wise multiply
    friend VEC2<T> operator * ( const VEC2<T> & lhs, const VEC2<T> & rhs){
        VEC2<T> rt(lhs);
        return rt *= rhs;
    }

    // scalar multiply
    friend VEC2<T> operator / ( const VEC2<T> & lhs, T rhs) {
        VEC2<T> rt(lhs);
        return rt /= rhs;
    }

    // vector component-wise multiply
    friend VEC2<T> operator / ( const VEC2<T> & lhs, const VEC2<T> & rhs){
        VEC2<T> rt(lhs);
        return rt /= rhs;
    }
	
	// cross product
    friend float cross( const VEC2<T> & lhs, const VEC2<T> & rhs) {
        return lhs.x * rhs.y - lhs.y * rhs.x;
    }

    ////////////////////////////////////////////////////////
    //
    //  Comparison operators
    //
    ////////////////////////////////////////////////////////

    // equality
    friend bool operator == ( const VEC2<T> &lhs, const VEC2<T> &rhs ) {
        bool r = true;
        for (int i = 0; i < lhs.size(); i++)
            r &= lhs._array[i] == rhs._array[i];
        return r;
    }

    // inequality
    friend bool operator != ( const VEC2<T> &lhs, const VEC2<T> &rhs ) {
        bool r = false;	//	Вот уроды, а здесь был true/ т.е. просто скопипастили из ==
		
		//	вот здесь было &= бакланы, сука. нвидиа
        for (int i = 0; i < lhs.size(); i++)
            r |= lhs._array[i] != rhs._array[i];
			
		//return (lhs._array[0] != rhs._array[0]) || (lhs._array[1] != rhs._array[1]);
        return r;
    }
   
    //data intentionally left public to allow VEC2.x
    union {
        struct {
            T x,y;          // standard names for components
        };
        struct {
            T s,t;          // standard names for components
        };
        T _array[2];     // array access
    };
};

//////////////////////////////////////////////////////////////////////
//
// VEC3 - template class for 3-tuple vector
//
//////////////////////////////////////////////////////////////////////
template <class T>
class VEC3 {
public:
	
	static VEC3 deep;
	static VEC3 back;
	static VEC3 up;
	static VEC3 down;
	static VEC3 left;
	static VEC3 right;
	static VEC3 zero;
	static VEC3 one;
	
    typedef T value_type;
    int size() const { return 3;}

    ////////////////////////////////////////////////////////
    //
    //  Constructors
    //
    ////////////////////////////////////////////////////////

    // Default/scalar constructor
    VEC3(const T & t = T()) {
        for(int i = 0; i < size(); i++) _array[i] = t;
    }

    // Construct from array
    VEC3(const T * tp) {
        for(int i = 0; i < size(); i++) _array[i] = tp[i];
    }

    // Construct from explicit values
    VEC3( const T v0, const T v1, const T v2) {
        x = v0;
        y = v1;
        z = v2;
    }

    explicit VEC3( const VEC4<T> &u) {
        for(int i = 0; i < size(); i++) _array[i] = u._array[i];
    }

    explicit VEC3( const VEC2<T> &u, T v0) {
        x = u.x;
        y = u.y;
        z = v0;
    }
    
	const T * get_value() const {
        return _array;
    }
    
    VEC3<T> & set_value( const T * rhs ) {
        for(int i = 0; i < size(); i++) _array[i] = rhs[i];
        return *this;
    }
    
    VEC3<T> & set_value( const float _x, const float _y, const float _z ) {      
        _array[0] = _x;
        _array[1] = _y;
		_array[2] = _z;
		//_array[3] = _z; //??????????????????????? NVidia лохи
        return *this;
    }
    
    // indexing operators
    T & operator [] ( int i ) {
        return _array[i];
    }
    
    const T & operator [] ( int i ) const {
        return _array[i];
    }

    // type-cast operators
    operator T * () {
        return _array;
    }

    operator const T * () const {
        return _array;
    }


    ////////////////////////////////////////////////////////
    //
    //  Math operators
    //
    ////////////////////////////////////////////////////////

    // scalar multiply assign
    friend VEC3<T> & operator *= ( VEC3<T> &lhs, T d ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= d;
        return lhs;
    }

    // component-wise vector multiply assign
    friend VEC3<T> & operator *= ( VEC3<T> &lhs, const VEC3<T> &rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= rhs[i];
        return lhs;
    }

    // scalar divide assign
    friend VEC3<T> & operator /= ( VEC3<T> &lhs, T d ) {
        if(d == 0) return lhs;
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= d;
        return lhs;
    }

    // component-wise vector divide assign
    friend VEC3<T> & operator /= ( VEC3<T> &lhs, const VEC3<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= rhs._array[i];
        return lhs;
    }

    // component-wise vector add assign
    friend VEC3<T> & operator += ( VEC3<T> &lhs, const VEC3<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] += rhs._array[i];
        return lhs;
    }

    // component-wise vector subtract assign
    friend VEC3<T> & operator -= ( VEC3<T> &lhs, const VEC3<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] -= rhs._array[i];
        return lhs;
    }

    // unary negate
    friend VEC3<T> operator - ( const VEC3<T> &rhs) {
        VEC3<T> rv;
        for(int i = 0; i < rhs.size(); i++) rv._array[i] = -rhs._array[i];
        return rv;
    }

    // vector add
    friend VEC3<T> operator + ( const VEC3<T> & lhs, const VEC3<T> & rhs) {
        VEC3<T> rt(lhs);
        return rt += rhs;
    }

    // vector subtract 
    friend VEC3<T> operator - ( const VEC3<T> & lhs, const VEC3<T> & rhs) {
        VEC3<T> rt(lhs);
        return rt -= rhs;
    }

    // scalar multiply
    friend VEC3<T> operator * ( const VEC3<T> & lhs, T rhs) {
        VEC3<T> rt(lhs);
        return rt *= rhs;
    }

    // scalar multiply
    friend VEC3<T> operator * ( T lhs, const VEC3<T> & rhs) {
        VEC3<T> rt(lhs);
        return rt *= rhs;
    }

    // vector component-wise multiply
    friend VEC3<T> operator * ( const VEC3<T> & lhs, const VEC3<T> & rhs){
        VEC3<T> rt(lhs);
        return rt *= rhs;
    }

    // scalar multiply
    friend VEC3<T> operator / ( const VEC3<T> & lhs, T rhs) {
        VEC3<T> rt(lhs);
        return rt /= rhs;
    }

    // vector component-wise multiply
    friend VEC3<T> operator / ( const VEC3<T> & lhs, const VEC3<T> & rhs){
        VEC3<T> rt(lhs);
        return rt /= rhs;
    }

    ////////////////////////////////////////////////////////
    //
    //  Comparison operators
    //
    ////////////////////////////////////////////////////////

    // equality
    friend bool operator == ( const VEC3<T> &lhs, const VEC3<T> &rhs ) {
        bool r = true;
        for (int i = 0; i < lhs.size(); i++)
            r &= lhs._array[i] == rhs._array[i];
        return r;
    }

    // inequality
    friend bool operator != ( const VEC3<T> &lhs, const VEC3<T> &rhs ) {
        bool r = true;
        for (int i = 0; i < lhs.size(); i++)
            r |= lhs._array[i] != rhs._array[i];
        return r;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //
    // dimension specific operations
    //
    ////////////////////////////////////////////////////////////////////////////////

    // cross product
    friend VEC3<T> cross( const VEC3<T> & lhs, const VEC3<T> & rhs) {
        VEC3<T> r;

        r.x = lhs.y * rhs.z - lhs.z * rhs.y;
        r.y = lhs.z * rhs.x - lhs.x * rhs.z;
        r.z = lhs.x * rhs.y - lhs.y * rhs.x;

        return r;
    }
   
    //data intentionally left public to allow VEC2.x
    union {
        struct {
            T x, y, z;          // standard names for components
        };
        struct {
            T s, t, r;          // standard names for components
        };
        T _array[3];     // array access
    };
};


//////////////////////////////////////////////////////////////////////
//
// VEC4 - template class for 4-tuple vector
//
//////////////////////////////////////////////////////////////////////
template <class T>
class VEC4 {
public:

    typedef T value_type;
    int size() const { return 4;}

    ////////////////////////////////////////////////////////
    //
    //  Constructors
    //
    ////////////////////////////////////////////////////////

    // Default/scalar constructor
    VEC4(const T & t = T()) {
        for(int i = 0; i < size(); i++) _array[i] = t;
    }

    // Construct from array
    VEC4(const T * tp) {
        for(int i = 0; i < size(); i++) _array[i] = tp[i];
    }

    // Construct from explicit values
    VEC4( const T v0, const T v1, const T v2, const T v3) {
        x = v0;
        y = v1;
        z = v2;
        w = v3;
    }

    explicit VEC4( const VEC3<T> &u, T v0) {
        x = u.x;
        y = u.y;
        z = u.z;
        w = v0;
    }

    explicit VEC4( const VEC2<T> &u, T v0, T v1) {
        x = u.x;
        y = u.y;
        z = v0;
        w = v1;
    }
    
    const T * get_value() const {
        return _array;
    }
    
    VEC4<T> & set_value( const T * rhs ) {
        for(int i = 0; i < size(); i++) _array[i] = rhs[i];
        return *this;
    }

	VEC4<T> & set_value(const T& _x, const T& _y, const T& _z, const T& _w) {
        x = _x;
		y = _y;
		z = _z;
		w = _w;
		return *this;
    }
    
    // indexing operators
    T & operator [] ( int i ) {
        return _array[i];
    }
    
    const T & operator [] ( int i ) const {
        return _array[i];
    }

    // type-cast operators
    operator T * () {
        return _array;
    }

    operator const T * () const {
        return _array;
    }

    ////////////////////////////////////////////////////////
    //
    //  Math operators
    //
    ////////////////////////////////////////////////////////

    // scalar multiply assign
    friend VEC4<T> & operator *= ( VEC4<T> &lhs, T d ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= d;
        return lhs;
    }

    // component-wise vector multiply assign
    friend VEC4<T> & operator *= ( VEC4<T> &lhs, const VEC4<T> &rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= rhs[i];
        return lhs;
    }

    // scalar divide assign
    friend VEC4<T> & operator /= ( VEC4<T> &lhs, T d ) {
        if(d == 0) return lhs;
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= d;
        return lhs;
    }

    // component-wise vector divide assign
    friend VEC4<T> & operator /= ( VEC4<T> &lhs, const VEC4<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= rhs._array[i];
        return lhs;
    }

    // component-wise vector add assign
    friend VEC4<T> & operator += ( VEC4<T> &lhs, const VEC4<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] += rhs._array[i];
        return lhs;
    }

    // component-wise vector subtract assign
    friend VEC4<T> & operator -= ( VEC4<T> &lhs, const VEC4<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] -= rhs._array[i];
        return lhs;
    }

    // unary negate
    friend VEC4<T> operator - ( const VEC4<T> &rhs) {
        VEC4<T> rv;
        for(int i = 0; i < rhs.size(); i++) rv._array[i] = -rhs._array[i];
        return rv;
    }

    // vector add
    friend VEC4<T> operator + ( const VEC4<T> & lhs, const VEC4<T> & rhs) {
        VEC4<T> rt(lhs);
        return rt += rhs;
    }

    // vector subtract 
    friend VEC4<T> operator - ( const VEC4<T> & lhs, const VEC4<T> & rhs) {
        VEC4<T> rt(lhs);
        return rt -= rhs;
    }

    // scalar multiply
    friend VEC4<T> operator * ( const VEC4<T> & lhs, T rhs) {
        VEC4<T> rt(lhs);
        return rt *= rhs;
    }

    // scalar multiply
    friend VEC4<T> operator * ( T lhs, const VEC4<T> & rhs) {
        VEC4<T> rt(lhs);
        return rt *= rhs;
    }

    // vector component-wise multiply
    friend VEC4<T> operator * ( const VEC4<T> & lhs, const VEC4<T> & rhs){
        VEC4<T> rt(lhs);
        return rt *= rhs;
    }

    // scalar multiply
    friend VEC4<T> operator / ( const VEC4<T> & lhs, T rhs) {
        VEC4<T> rt(lhs);
        return rt /= rhs;
    }

    // vector component-wise multiply
    friend VEC4<T> operator / ( const VEC4<T> & lhs, const VEC4<T> & rhs){
        VEC4<T> rt(lhs);
        return rt /= rhs;
    }

    ////////////////////////////////////////////////////////
    //
    //  Comparison operators
    //
    ////////////////////////////////////////////////////////

    // equality
    friend bool operator == ( const VEC4<T> &lhs, const VEC4<T> &rhs ) {
        bool r = true;
        for (int i = 0; i < lhs.size(); i++)
            r &= lhs._array[i] == rhs._array[i];
        return r;
    }

    // inequality
    friend bool operator != ( const VEC4<T> &lhs, const VEC4<T> &rhs ) {
        bool r = true;
        for (int i = 0; i < lhs.size(); i++)
			r |= lhs._array[i] != rhs._array[i];
        return r;
    }
   
    //data intentionally left public to allow VEC2.x
    union {
        struct {
            T x, y, z, w;          // standard names for components
        };
        struct {
            T s, t, r, q;          // standard names for components
        };
        T _array[4];     // array access
    };
};

////////////////////////////////////////////////////////////////////////////////
//
// Generic vector operations
//
////////////////////////////////////////////////////////////////////////////////

// compute the dot product of two vectors
template<class T>
inline typename T::value_type dot( const T & lhs, const T & rhs ) {
	typename T::value_type r = 0;
    for(int i = 0; i < lhs.size(); i++) r += lhs._array[i] * rhs._array[i];
    return r;
}

// return the length of the provided vector
template< class T>
inline float length( const T & vec) {
	float r = 0;
    for(int i = 0; i < vec.size(); i++) r += (float) (vec._array[i]*vec._array[i]); 
    return sqrt(r);
}

// return the squared norm
template< class T>
inline typename T::value_type square_norm( const T & vec) {
	typename T::value_type r = 0;
    for(int i = 0; i < vec.size(); i++) r += vec._array[i]*vec._array[i]; 
    return r;
}

// return the normalized version of the vector
template< class T>
inline T normalize( const T & vec) { 
	typename T::value_type sum(0);
    T r;
    for(int i = 0; i < vec.size(); i++) 
        sum += vec._array[i] * vec._array[i];
    sum = typename T::value_type(sqrt(sum));
    if (sum > 0)
        for(int i = 0; i < vec.size(); i++) 
            r._array[i] = vec._array[i] / sum;
    return r;
}

// In VC8 : min and max are already defined by a #define...
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

//componentwise min
template< class T>
inline T min( const T & lhs, const T & rhs ) {
    T rt;
    for (int i = 0; i < lhs.size(); i++) rt._array[i] = MIN( lhs._array[i], rhs._array[i]);
    return rt;
}

// componentwise max
template< class T>
inline T max( const T & lhs, const T & rhs ) {
    T rt;
    for (int i = 0; i < lhs.size(); i++) rt._array[i] = MAX( lhs._array[i], rhs._array[i]);
    return rt;
}


};

#endif
