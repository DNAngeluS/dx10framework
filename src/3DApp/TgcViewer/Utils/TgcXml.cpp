#include "StdAfx.h"

#include "TgcXml.h"


TgcXml::TgcXml() :
	m_fp( NULL )
{
	// nada para hacer
}

void TgcXml::Open(LPCTSTR filename)
{
    HRESULT hr = S_OK; 
    m_pFileStream = NULL;
    m_pReader = NULL;     

	SHCreateStreamOnFile(filename, STGM_READ, &m_pFileStream);
	CreateXmlReader(__uuidof(IXmlReader), (void**) &m_pReader, NULL);
	m_pReader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);
	m_pReader->SetInput(m_pFileStream);
}

TgcXml::~TgcXml(void)
{
	SafeRelease(m_pFileStream); 
    SafeRelease(m_pReader); 
}

bool TgcXml::Read()
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
			m_pReader->GetNamespaceUri(&m_ppwszNamespaceUri, NULL);
			m_empty = m_pReader->IsEmptyElement();

			m_tagname.clear();
			m_tagname.append(m_pwszLocalName);
			m_namespace.clear();
			m_namespace.append(m_ppwszNamespaceUri);
            break; 
        case XmlNodeType_EndElement: 
			m_pReader->GetPrefix(&m_pwszPrefix, &m_cwchPrefix);
			m_pReader->GetLocalName(&m_pwszLocalName, NULL);

            break; 
        case XmlNodeType_Text:
			m_pReader->GetValue(&m_pwszValue, NULL);

			break;
        case XmlNodeType_Whitespace: 
			
			Read();
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
			Read();

            break; 
        case XmlNodeType_DocumentType: 
            return false;
            break; 
        } 
    } 
}

void TgcXml::TokenizeValue( queue< wstring >* pList, TCHAR commentChar )
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

wstring TgcXml::GetValue()
{
	wstring value;
	value.append(m_pwszValue);

	return value;
}

wstring TgcXml::GetPrefix()
{
	wstring prefix;
	prefix.append(m_pwszPrefix);

	return prefix;
}