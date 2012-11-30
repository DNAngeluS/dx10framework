#pragma once
#include "..\Framework\Graphics\Mesh.h"

class TgcLoader
{
private:
	wstring m_name;
	wstring m_filename;
	wstring m_path;

public:

	TgcLoader(void);
	~TgcLoader(void);

	void loadMesh(const WCHAR* filepath, cMesh** mesh);
};
