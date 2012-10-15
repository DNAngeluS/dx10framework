#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include "Globals.h"

//==========--------------------------  

/**
 * El siguiente codigo es lento y usa memoria dinamica, pero solo se usa cuando algo malo realmente pasa.
 */
class cError
{
	std::wstring m_errorText;
public:
	cError( const TCHAR* errorText )
	{
		DP1(L"***\n*** [ERROR] cError thrown! text: [%s]\n***\n", errorText );
		m_errorText = std::wstring( errorText );
	}

	const TCHAR* GetText()
	{
		return m_errorText.c_str();
	}
};

//==========--------------------------  

enum eResult
{
	resAllGood		= 0, // La funcion no tuvo errores
	resFalse		= 1, // La funcion funciono y retorno false
	resFailed		= -1, // La funcion fallo
	resNotImpl		= -2, // La funcion no fue implementada
	resForceDWord = 0x7FFFFFFF
};

inline bool Succeeded( eResult in )
{
	if( in >= 0 )
		return true;
	return false;
}

inline bool Failed( eResult in )
{
	if( in < 0 )
		return true;
	return false;
}

#endif //_ERRORS_H