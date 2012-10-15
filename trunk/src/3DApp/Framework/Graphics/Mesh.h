#ifndef _MESH_H
#define _MESH_H

#include <vector>
#include <string>
#include "..\Framework\Application.h"
#include "..\Math3D\Tri.h"
#include "..\Math3D\mathD3D.h"
#include "..\Framework\Graphics\GraphicsLayer.h"

using namespace std;

class cMesh  
{
	typedef tri<WORD> sTri;

	vector< sTri >		m_tris;
	vector< sTri >		m_colors;
	vector< sTri >		m_textures;
	vector< cGraphicsLayer::cDefaultVertex >	m_verts;

	std::wstring			m_name;
	std::wstring			m_texture;

	ID3D10Buffer*			m_pVertexBuffer;
	ID3D10Buffer*			m_pIndexBuffer;

	ID3D10Resource*				m_pTexture;
	ID3D10ShaderResourceView*	m_pSRView;

private:
	void UpdateTransform();

public:
	D3DXVECTOR3				vTranslation;
	D3DXVECTOR3				vRotation;
	D3DXVECTOR3				vScale;

	D3DXMATRIX				m_mtxTransform;


	vector<D3DXVECTOR3>	vertices, normals;
	vector<D3DXVECTOR2>	textures;
	vector<D3DXCOLOR>	colors;
		
	cMesh( const TCHAR* name,const vector<D3DXVECTOR3>* vertices,const vector<D3DXVECTOR3>* normals,
			const vector<D3DXVECTOR2>* textures,const vector<D3DXCOLOR>* colors);
	cMesh( const LPCTSTR  filename);

	~cMesh();

	float GenRadius();
	void Scale( float amt );
	void Update( float timeDelta );
	void Draw();

	int NumVerts(){ return m_verts.size(); }
	int NumTris(){ return m_tris.size(); }
	const TCHAR* Name(){ return m_name.c_str(); }

	cGraphicsLayer::cDefaultVertex* VertData(){ return &m_verts[0]; }
	sTri* TriData(){ return &m_tris[0]; }

};

#endif // _MESH_H
