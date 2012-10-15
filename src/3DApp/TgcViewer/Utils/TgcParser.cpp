#include "StdAfx.h"

#include <vector>
#include <queue>

#include "TgcParser.h"
#include "..\Framework\File.h"

using namespace std;

TgcParser::TgcParser(void)
{
}

void TgcParser::tgcSceneLoader(const LPCTSTR filename)
{
	cFile file;	

	const WCHAR*	tag;
	const WCHAR*	value;
	IXmlReader*		pReader;
	UINT			iAtributeCount;

	int t1, t2;

	int nVertexFields, nTris, nVerts, nColors, nTex;
	queue<wstring>		m_tokens;
	vector<D3DXVECTOR3>	vertices, normals;
	vector<D3DXVECTOR2>	textures;
	vector<D3DXCOLOR>	colors;
	vector<WORD>		tris;

	file.OpenXML(filename);
	pReader = file.GetXml();

}

TgcParser::~TgcParser(void)
{
}
