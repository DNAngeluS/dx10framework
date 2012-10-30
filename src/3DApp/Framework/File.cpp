#include "StdAfx.h"

#include "File.h"
using namespace std;

cFile::cFile() :
	m_fp( NULL )
{
	// nada para hacer
}

cFile::~cFile()
{
	// Chequear que el usuario halla cerrado el archivo
	if( m_fp != NULL )
	{
		Close();
	}
}

void cFile::Create( const TCHAR* filename )
{
	// Crea un handle de archivo
	m_fp = 0;
	_wfopen_s( &m_fp, filename, L"w+, ccs=UTF-8" );

	// Chequeo que todo haya salido bien
	if( m_fp == NULL )
	{
		throw cFileCannotCreate( filename );
	}

	m_filename = wstring( filename );
}

void cFile::Append( const TCHAR* filename )
{
	// Crea el handle de archivo
	m_fp = 0;
	_wfopen_s( &m_fp, filename, L"ab" );

	if( m_fp == NULL )
	{
		throw cFileCannotCreate( filename );
	}

	m_filename = wstring( filename );
}

void cFile::Open( const TCHAR* filename )
{
	m_fp = 0;
	m_fp = _wfopen( filename, L"r+b" );
	
	if( m_fp == NULL )
	{
		//throw cFileCannotCreate( filename );
		throw cError(L"No se pudo abrir el archivo" );
	}

	m_filename = wstring( filename );

}


void cFile::Close()
{
	if( m_fp == NULL )
		return; // El archivo ya fue cerrado

	if( 0 != fclose( m_fp ) )
	{
		DP(L"Error en cFile::Close\n");
	}

	m_fp = NULL;
}



bool cFile::Exists( const TCHAR* filename )
{
	// Intenta abrir el archivo. Si no puede, no existe
	cFile temp;

	temp.m_fp = 0;
	_wfopen_s( &temp.m_fp, filename, L"r" );

	// Chequeo
	if( temp.m_fp == NULL )
	{
		return false;
	}
	temp.Close();
	return true;
}


void cFile::ReadBuff( void* pBuffer, int buffSize )
{
	if( m_fp == NULL )
		return; // error

	//int numRead = fread( pBuffer, buffSize, 1, m_fp );
	int numRead = fread( pBuffer, 1, buffSize, m_fp );

	if( numRead != buffSize )
	{
		if( 0 != feof( m_fp ) )
		{
			throw cFileEOF();
		}
		else if( ferror( m_fp ) )
		{
			throw cFileReadError();
		}
		else
		{
			int foo = 0;
		}
	}
}


void cFile::WriteBuff( void* pBuffer, int buffSize )
{
	if( m_fp == NULL )
		return; // error

	int numWritten = fwrite( pBuffer, buffSize, 1, m_fp );

	if( numWritten != buffSize )
	{
		throw cFileWriteError();
	}
}


/**
 * Lectura no segura ante Overrun
 */
void cFile::ReadLine( TCHAR* pBuffer )
{
	TCHAR currChar;
	bool done = false;

	int nRead = 0;
	while( !done )
	{
		try
		{
			ReadBuff( &currChar, 1 );
			nRead++;
		}
		catch( cFileEOF )
		{
			// break loop, al final de archivo
			if( nRead == 0 )
			{
				/**
				 * Es el final del archivo y no se puede leer.
				 */
				throw;
			}
			break;
		}

		
		if( currChar == '\0' || currChar == '\n' )
		{
			break;
		}
		*pBuffer++ = currChar;
	}
	// Fin del string con \n\0
	*pBuffer++ = '\n';
	*pBuffer++ = '\0';
}

/**
 * Lectura segura ante Overrun
 */
wstring cFile::ReadLine()
{
	TCHAR currChar;
	bool done = false;

	int nRead = 0;

	wstring out;

	while( !done )
	{
		try
		{
			currChar = 0;
			ReadBuff( &currChar, 1 );
			nRead++;
		}
		catch( cFileEOF )
		{
			// break loop, al final de archivo
			if( nRead == 0 )
			{
				/**
				 * EOF no se puede leer.
				 */
				throw;
			}
			break;
		}

		if( currChar == '\0' || currChar == '\n' )
		{
			break;
		}

		out += currChar;
	}
	return out;
}


void cFile::ReadNonCommentedLine( TCHAR* pBuffer, TCHAR commentChar )
{
	TCHAR buff[1024];
	buff[0] = 0;
	while( 1 )
	{
		ReadLine( buff );
		if( buff[0] != commentChar )
			break;
	}
	wcscpy_s( pBuffer, 1024, buff );
}


void cFile::TokenizeNextNCLine( queue< wstring >* pList, TCHAR commentChar )
{
	wstring str;
	while(1)
	{
		str = ReadLine();
		if( str[0] != commentChar )
			break;
	}

	// Elimina comentarios al final
	DP1(L"parsing [%s]\n", str.c_str() );
	unsigned int commentLoc = str.find( commentChar, 0);
	if( commentLoc != -1 )
	{
		//str = str.erase( commentLoc );
	}

	TCHAR sep[] = L" \t\n\r";

	unsigned int tokStart = str.find_first_not_of( (TCHAR*)sep, 0 );
	unsigned int tokEnd;
	while( tokStart != -1 )
	{
		tokEnd = str.find_first_of( (TCHAR*)sep, tokStart );
		wstring token = str.substr( tokStart, tokEnd-tokStart );
		pList->push( token );

		tokStart = str.find_first_not_of( (TCHAR*)sep, tokEnd );
	}
}

void cFile::OpenXML(LPCTSTR filename)
{ 
    HRESULT hr = S_OK; 
    m_pFileStream = NULL;
    m_pReader = NULL;     

	SHCreateStreamOnFile(filename, STGM_READ, &m_pFileStream);
	CreateXmlReader(__uuidof(IXmlReader), (void**) &m_pReader, NULL);
	m_pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);
	m_pReader->SetInput(m_pFileStream);
 }
/**
*
*
*
*/
bool cFile::ReadXML()
{
	HRESULT hr = S_OK; 
    if(S_OK == (hr = m_pReader->Read(&m_nodeType))) 
    { 
        switch (m_nodeType) 
        { 
        case XmlNodeType_XmlDeclaration: 

            break; 
        case XmlNodeType_Element: 
			hr = m_pReader->GetPrefix(&m_pwszPrefix, &m_cwchPrefix);
			m_pReader->GetLocalName(&m_pwszLocalName, NULL);
			m_empty = m_pReader->IsEmptyElement();
 
            break; 
        case XmlNodeType_EndElement: 
			m_pReader->GetPrefix(&m_pwszPrefix, &m_cwchPrefix);
			m_pReader->GetLocalName(&m_pwszLocalName, NULL);

            break; 
        case XmlNodeType_Text:
			m_pReader->GetValue(&m_pwszValue, NULL);

			break;
        case XmlNodeType_Whitespace: 
			
			ReadXML();
            break; 
        case XmlNodeType_CDATA: 
			m_pReader->GetValue(&m_pwszValue, NULL);

            break; 
        case XmlNodeType_ProcessingInstruction: 
			m_pReader->GetLocalName(&m_pwszLocalName, NULL);
			m_pReader->GetValue(&m_pwszValue, NULL);

            break; 
        case XmlNodeType_Comment: 
			//m_pReader->GetValue(&m_pwszValue, NULL);
			ReadXML();

            break; 
        case XmlNodeType_DocumentType: 
            return false;
            break; 
        } 
    } 
}

void cFile::TokenizeXmlValue( queue< wstring >* pList, TCHAR commentChar )
{
	wstring str = m_pwszValue;
	while(1)
	{		
		if( str[0] != commentChar )
			break;
	}

	// Elimina comentarios al final
	//DP1(L"parsing [%s]\n", str.c_str() );
	//unsigned int commentLoc = str.find( commentChar, 0);
	//if( commentLoc != -1 )
	//{
		//str = str.erase( commentLoc );
	//}

	TCHAR sep[] = L" \t\n\r";

	unsigned int tokStart = str.find_first_not_of( (TCHAR*)sep, 0 );
	unsigned int tokEnd;
	while( tokStart != -1 )
	{
		tokEnd = str.find_first_of( (TCHAR*)sep, tokStart );
		wstring token = str.substr( tokStart, tokEnd-tokStart );
		pList->push( token );

		tokStart = str.find_first_not_of( (TCHAR*)sep, tokEnd );
	}
}

void cFile::GetXmlValue(__out LPCWSTR *value)
{
	*value = m_pwszValue;
}

void cFile::GetXmlPrefix(__out LPCWSTR *prefix)
{
	*prefix = m_pwszPrefix;
}

XmlNodeType cFile::GetXmlNodeType()
{
	return m_nodeType;
}
IXmlReader* cFile::GetXml()
{
	return m_pReader;
}