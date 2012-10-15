#ifndef _FILE_H
#define _FILE_H

#include <windows.h>
#include <assert.h>

#include <queue>
#include <string>
//XML LIBS
#include <ole2.h> 
#include <xmllite.h> 
#include <stdio.h>
#include <shlwapi.h>

#include "Globals.h"
//==========--------------------------  Excepciones tiradas por cFile class

class cFileError
{
protected:
	std::wstring m_filename;
	std::wstring m_errorText;
public:
	cFileError( const TCHAR* filename, const TCHAR* errorText = L"\n" )
	{
		DP1(L"***\n*** [ERROR] cFileError thrown! text: [%s]\n***\n", errorText );
		m_filename = std::wstring( filename );
		m_errorText = std::wstring( errorText );
	}
	const TCHAR* Name()
	{
		return m_filename.c_str();
	}

	const TCHAR* GetText()
	{
		return m_errorText.c_str();
	}
};

// Thrown al abrir.
class cFileNotFound : public cFileError
{
public:
	cFileNotFound( const TCHAR* filename ) : 
	  cFileError( filename, L"No se encontro el archivo" )
	{	
	}
};

class cFileCannotCreate : public cFileError
{
public:
	cFileCannotCreate( const TCHAR* filename ) : 
	  cFileError( filename, L"No se pudo crear el archivo" )
	{	
	}
};

class cFileEOF { };

class cFileReadError { };

class cFileWriteError { };

//==========--------------------------  cFile class

class cFile  
{
protected:

	FILE* m_fp;
	std::wstring m_filename;

	//XML READER
    IStream *m_pFileStream; 
    IXmlReader *m_pReader; 
    XmlNodeType m_nodeType; 
    const WCHAR* m_pwszPrefix; 
    const WCHAR* m_pwszLocalName; 
    const WCHAR* m_pwszValue;
    UINT m_cwchPrefix;
	UINT m_pnAttributeCount;
	BOOL m_empty;

public:

	cFile();
	~cFile();

	// Descarga si existe
	void Create( const TCHAR* filename );

	// Agrega si existe, falle si no
	void Append( const TCHAR* filename );

	// Abre si existe, falla si no
	void Open( const TCHAR* filename );

	// Cierra
	void Close();

	// Chequeo de existencia
	static bool Exists( const TCHAR* filename );

	// Lee del buffer del archivo
	void ReadBuff( void* pBuffer, int buffSize );

	// Escribe en el buffer del archivo
	void WriteBuff( void* pBuffer, int buffSize );

	// Lee una linea del buffer. El tamaño debe ser >= 255 para estar seguros
	void ReadLine( TCHAR* pBuffer );
	std::wstring ReadLine();

	// Lee lineas no comentadas
	void ReadNonCommentedLine( TCHAR* pBuffer, TCHAR commentChar );

	// Parsea la lineas no comentadas con \t\n
	void TokenizeNextNCLine( std::queue< std::wstring >* pList, TCHAR commentChar = '#' );

	// Estructuras basicas pueden ser leias y escritas automaticamente
	template <class type>
	void Read( type* data )
	{
		ReadBuff( data, sizeof( type ) );
	}

	template <class type>
	void Write( type& data )
	{
		WriteBuff( &data, sizeof( type ) );
	}

	//XML READER
	void OpenXML(LPCTSTR filename);
	void ReadXML();

	void TokenizeXmlValue( std::queue< std::wstring >* pList, TCHAR commentChar = '#' );

	//Getters
	IXmlReader* GetXml();
	void GetXmlValue(LPCWSTR *);
	void GetXmlPrefix(LPCWSTR *);
	XmlNodeType GetXmlNodeType();
};

#endif //_FILE_H
