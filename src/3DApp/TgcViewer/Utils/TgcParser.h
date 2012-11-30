#pragma once

#include <vector>
#include <queue>

#include "..\Math3D\Tri.h"
#include "..\Framework\Application.h"
#include "..\Framework\Graphics\GraphicsLayer.h"
#include "..\Framework\Graphics\Vertex.h"
#include "..\Framework\Graphics\Mesh.h"

class TgcParser
{
public:
	TgcParser(void);
	~TgcParser(void);

	void mesh(const LPCTSTR filename, cMesh** mesh);
};

