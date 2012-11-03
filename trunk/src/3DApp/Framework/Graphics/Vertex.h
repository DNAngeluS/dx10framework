#ifndef _VERTEX_H
#define _VERTEX_H
#pragma once

#include <d3d10.h>
#include <d3dx10.h>

#define ERROR_RESOURCE_VALUE 1

template<typename TYPE> BOOL IsErrorResource( TYPE data )
{
    if( ( TYPE )ERROR_RESOURCE_VALUE == data )
        return TRUE;
    return FALSE;
}

// Vertex format
struct sVERTEX
{
	D3DXVECTOR3 vPosition;  
	D3DXVECTOR3 vNormal;
	D3DXCOLOR	cColor;
	D3DXVECTOR2 vTexCoords;
};

const D3D10_INPUT_ELEMENT_DESC const VERTEXLAYOUT[] = 
{
    {"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D10_INPUT_PER_VERTEX_DATA, 0},  
	{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
	{"COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
	{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
	{"TEXCOORD",	1, DXGI_FORMAT_R32G32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
};


// Used for a hashtable vertex cache when creating the mesh from a .obj file
struct sCacheEntry
{
    UINT index;
    sCacheEntry* pNext;
};


// Material properties per mesh subset
struct sMaterial
{
    WCHAR   strName[MAX_PATH];

    D3DXVECTOR3 vAmbient;
    D3DXVECTOR3 vDiffuse;
    D3DXVECTOR3 vSpecular;

    int nShininess;
    float fAlpha;

    bool bSpecular;

    WCHAR   strTexture[MAX_PATH];
    ID3D10ShaderResourceView* pTextureRV10;
    ID3D10EffectTechnique*  pTechnique;
};

class cDefaultVertex
{
public:
	enum Semantic 
	{
		POSITION,
		NORMAL,
		COLOR,
		TEXCOORD
	};

//	D3DXVECTOR3 m_vPosition;  
//	D3DXVECTOR3 m_vNormal;
//	D3DXCOLOR m_vColor;
//	D3DXVECTOR2 m_TexCoords;
//	sVERTEX	m_Vertex;

	static inline LPCSTR Semantics(int i)
	{
		LPCSTR Semantics[] = {
			"POSITION",
			"NOMRAL",
			"COLOR",
			"TEXCOORD"
		};
			
		return Semantics[i];
	}

	static inline D3D10_INPUT_ELEMENT_DESC* GetDefaultVertexDesc(D3D10_INPUT_ELEMENT_DESC** defaultLayout)
	{
		D3D10_INPUT_ELEMENT_DESC dL[] =
		{
			{Semantics(POSITION),	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D10_INPUT_PER_VERTEX_DATA, 0},  
			{Semantics(NORMAL),		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
			{Semantics(COLOR),		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
			{Semantics(TEXCOORD),	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
			{Semantics(TEXCOORD),	1, DXGI_FORMAT_R32G32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
		};
		//ZeroMemory(defaultLayout, sizeof(D3D10_INPUT_ELEMENT_DESC) * 5);
		//CopyMemory(defaultLayout, dL, sizeof(D3D10_INPUT_ELEMENT_DESC) * 5);
		return dL;
	}

	static inline D3D10_INPUT_ELEMENT_DESC* GetDefaultVertexDesc()
	{
		D3D10_INPUT_ELEMENT_DESC dL[] =
		{
			{Semantics(POSITION),	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D10_INPUT_PER_VERTEX_DATA, 0},  
			{Semantics(NORMAL),		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
			{Semantics(COLOR),		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
			{Semantics(TEXCOORD),	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
			{Semantics(TEXCOORD),	1, DXGI_FORMAT_R32G32_FLOAT,		0, D3D10_APPEND_ALIGNED_ELEMENT,	D3D10_INPUT_PER_VERTEX_DATA, 0}, 
		};
		//ZeroMemory(defaultLayout, sizeof(D3D10_INPUT_ELEMENT_DESC) * 5);
		//CopyMemory(defaultLayout, dL, sizeof(D3D10_INPUT_ELEMENT_DESC) * 5);
		return dL;
	}
};

#endif //_VERTEX_H