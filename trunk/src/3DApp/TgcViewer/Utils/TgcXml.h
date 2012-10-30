#pragma once
#include <windows.h>
#include <assert.h>

#include <queue>
#include <string>
//XML LIBS
#include <ole2.h> 
#include <xmllite.h> 
#include <stdio.h>
#include <shlwapi.h>

using namespace std;

class TgcXml
{
protected:

	FILE* m_fp;
	wstring m_filename;
	wstring m_tagname;
	wstring m_namespace;

	//XML READER
    IStream *m_pFileStream; 
    IXmlReader *m_pReader; 
    XmlNodeType m_nodeType; 
    const WCHAR* m_pwszPrefix; 
    const WCHAR* m_pwszLocalName;
	const WCHAR* m_ppwszNamespaceUri;
    const WCHAR* m_pwszValue;
    UINT m_cwchPrefix;
	UINT m_pnAttributeCount;
	BOOL m_empty;

public:
	TgcXml(void);
	~TgcXml(void);

	//XML READER
	void Open(LPCTSTR filename);
	bool Read();

	void TokenizeValue( std::queue< std::wstring >* pList, TCHAR commentChar = '#' );

	//Getters
	IXmlReader* GetPointer(){return m_pReader;};
	wstring GetValue();
	wstring GetPrefix();
	wstring GetTagname(){return m_tagname;};
	wstring GetNamespace(){return m_namespace;};
	XmlNodeType GetNodeType(){return m_nodeType;};
};

