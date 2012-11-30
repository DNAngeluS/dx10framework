#include "StdAfx.h"
#include "TgcLoader.h"
#include "TgcParser.h"
#include "TgcXml.h"


TgcLoader::TgcLoader(void)
{
	m_name = L"";
	m_filename = L"";
}


TgcLoader::~TgcLoader(void)
{
}

void TgcLoader::loadMesh(const WCHAR* filepath, cMesh** mesh)
{
	WCHAR path[MAX_PATH] = {'\0'};

	TgcParser* parser = new TgcParser();


	parser->mesh(m_filename.c_str(), mesh);
	//return new cMesh(m_filename.c_str());
}
