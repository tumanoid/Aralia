#include "stdAfx.h"

char stb [2048] = {0};
static char log_buff [2048] = {0};


/*
============================================

============================================
*/
void LogOut ( const char* x, ... ) 
{
	va_list args;
    va_start ( args, x );
    vsprintf ( log_buff, x, args );
    printf ("%s\n", log_buff);
    va_end ( args );
}


/*
============================================

============================================
*/
void DG ( const char* x, ... ) 
{
	va_list args;
    va_start ( args, x );
    vsprintf ( log_buff, x, args );
    printf ("%s\n", log_buff);
    va_end ( args );
}


/*
============================================

============================================
*/
void WR ( const char* x, ... ) 
{
	va_list args;
    va_start ( args, x );
    vsprintf ( log_buff, x, args );
    printf ("WARNING* %s\n", log_buff);
    va_end ( args );
}


/*
============================================

============================================
*/
void ER ( const char* x, ... ) 
{
	va_list args;
    va_start ( args, x );
    vsprintf ( log_buff, x, args );
    printf ("ERROR* %s\n", log_buff);
    va_end ( args );
}



/*
============================================

============================================
*/
float rnd () 
{
	return (float)(rand() % 256) / 256.0;
}


/*
============================================
{-1, 1}

============================================
*/
float snrnd () 
{
	float s = rand () / (float)RAND_MAX;
	if (s>0.5)
		return 1;
	
	return -1;
}


/*
============================================
[-1..1]

============================================
*/
float srnd () 
{
	return (float)(rand() % 256) / 128.0 - 1.0;
}


/*
============================================
нормальное, нормализованное

============================================
*/
float srnd2 () 
{
	float u = rand () / (float)RAND_MAX;
	float v = rand () / (float)RAND_MAX;
		
	float x = 2.0 * 3.1415926535 * u;
	float y = sqrt (-2.0 * log(v));

	float r = sin(x) * y / 3.0;

	return r;
}


/*
============================================
Hash

============================================
*/
//http://www.azillionmonkeys.com/qed/hash.html
//#include "cstdint.hpp"

#undef get16bits

#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif


uint32_t SFH (const char * data, int len) 
{

	uint32_t hash = len, tmp;
	int rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
                hash ^= hash << 16;
                hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
                hash += hash >> 11;
                break;
        case 2: hash += get16bits (data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += (signed char)*data;
                hash ^= hash << 10;
                hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}
