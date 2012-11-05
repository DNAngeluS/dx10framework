#pragma once

#include <vector>
#include <queue>

#include "..\Math3D\Tri.h"
#include "..\Framework\Application.h"
#include "..\Framework\Graphics\GraphicsLayer.h"
#include "..\Framework\Graphics\Vertex.h"

class TgcParser
{
public:
	TgcParser(void);
	~TgcParser(void);

	void tgcSceneLoader(const LPCTSTR filename);
};

