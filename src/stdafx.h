#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <stdarg.h>

#include "gl/glew.h"
#include "gl/wglew.h"
#include "nvm/nvmath.h"

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

#ifdef WIN32
typedef unsigned char   uint8_t;
typedef signed char     int8_t;
typedef unsigned short  uint16_t;

typedef __int64 int64_t ;
typedef unsigned __int64 uint64_t;

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;

#else
	#include <stdint.h>
	#include <inttypes.h>
#endif

extern char stb [2048];

void LogOut ( const char* x, ... );
void DG ( const char* x, ... );
void WR ( const char* x, ... );
void ER ( const char* x, ... );

uint32_t SFH (const char * data, int len);
inline const char * FNAME (const char * str) { return str;}

float rnd ();
float snrnd ();
float srnd ();
float srnd2 ();

extern int XRES;
extern int YRES;

extern int currFrame;
extern float currTime;
extern float deltaTime;

template <class T>
struct xptr
{
	T * object;
	xptr () : object(0) {}
	xptr (T* obj) : object(obj) {}
	
	xptr (xptr<T> & f) 
	{
		object = f.object;
		f.object = 0;
	}
	
	xptr<T> & operator= (T* obj) 
	{
		object = obj;
		return *this;
	}
	
	xptr<T> & operator= (xptr<T> & f) 
	{
		object = f.object;
		f.object = 0;
		return *this;
	}

	operator T * () {return object;}
	T * operator -> () {return object;}
	~xptr () { 
		if (object)
			delete object;
	}
};