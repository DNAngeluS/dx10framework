#ifndef _MESH_H
#define _MESH_H

#include <vector>
#include <string>
#include "..\DX\Include\D3DX10mesh.h"

#include "..\Math3D\Tri.h"
#include "..\Math3D\mathD3D.h"
#include "..\Graphics\Vertex.h"


using namespace std;

class cMesh  
{
protected:
	WCHAR					m_strMediaDir[ MAX_PATH ];
	std::wstring			m_name;
	std::wstring			m_texture;

	vector<DWORD>		m_Attributes;
	vector<WORD>		m_VertexIndex;
	vector<sVERTEX>		m_Vertex;
	vector<sMaterial*>	m_Materials;
	ID3D10Buffer*			m_pVertexBuffer;
	ID3D10Buffer*			m_pIndexBuffer;

	ID3D10Resource*				m_pTexture;
	ID3D10ShaderResourceView*	m_pSRView;

	ID3DX10Mesh*				m_pMesh;

	void UpdateTransform();

public:
	D3DXVECTOR3				vTranslation;
	D3DXVECTOR3				vRotation;
	D3DXVECTOR3				vScale;

	D3DXMATRIX				mtxTransform;


	vector<D3DXVECTOR3>	vertices, normals;
	vector<D3DXVECTOR2>	textures;
	vector<D3DXCOLOR>	colors;
		
	cMesh();
	~cMesh();

	void Create();

	float GenRadius();
	void Scale( float amt );
	void Update( float timeDelta );
	void Draw();

	int NumVerts(){ return m_Vertex.size(); }
	int NumTris(){ return m_VertexIndex.size(); }
	const TCHAR* Name(){ return m_name.c_str(); }

	sVERTEX* VertData(){ return &m_Vertex[0]; }
	//sTri* TriData(){ return &m_tris[0]; }
	WORD* IdxData(){ return &m_VertexIndex[0]; }

};

#endif // _MESH_H
