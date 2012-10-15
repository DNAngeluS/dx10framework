#ifndef _GLOBALS_H
#define _GLOBALS_H

#define POWER_OF_2(x) (((x) & ((x)-1))? false : true)

#define DP( a )\
{\
	TCHAR buff[1024];\
	swprintf_s( buff, a );\
	OutputDebugString( buff );\
}

#define DP0 DP

#define DP1( a, b )\
{\
	TCHAR buff[1024];\
	swprintf_s( buff, 1024, a, b );\
	OutputDebugString( buff );\
}

#define DP2( a, b, c )\
{\
	TCHAR buff[1024];\
	wmemset( buff, '0', 1024 );\
	swprintf_s( buff, 1024, a, b );\
	OutputDebugString( buff );\
}

#define DP3( a, b, c, d )\
{\
	TCHAR buff[1024];\
	swprintf_s( buff, 1024, a, b );\
	OutputDebugString( buff );\
}

#define DP4( a, b, c, d, e )\
{\
	TCHAR buff[1024];\
	swprintf_s( buff, 1024, a, b );\
	OutputDebugString( buff );\
}

/**
 * Se usa en varios lugares para setear una variables entre un intervalo
 */
template <class T>
static void inline Snap( T &a, T min, T max )
{
	if( a < min ) 
		a = min;
	if( a > max ) 
		a = max;
}

inline float RandFloat( float min = 0.f, float max = 1.f )
{
	return min + (max-min)*((float)rand()/RAND_MAX);
}

#endif //_GLOBALS_H