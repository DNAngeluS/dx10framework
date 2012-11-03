#ifndef _MODEL_H
#define _MODEL_H

#include <vector>
#include <string>
#include "..\Framework\Application.h"
#include "..\Math3D\Tri.h"
#include "..\Math3D\mathD3D.h"
#include "..\Framework\Graphics\GraphicsLayer.h"
#include "..\Framework\Graphics\Vertex.h"

class cModel  
{
	std::vector< sTri >		m_tris;
	std::vector< sTri >		m_colors;
	std::vector< sTri >		m_textures;
	std::vector< sVERTEX >	m_verts;

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

	D3DXMATRIX				mTransform;

	//cModel( const TCHAR* filename );
	cModel( const TCHAR* name, int nVerts, int nTris );
	cModel( const LPCTSTR  filename);

	~cModel();

	float GenRadius();
	void Scale( float amt );
	void Update( float timeDelta );
	void Draw();

	int NumVerts(){ return m_verts.size(); }
	int NumTris(){ return m_tris.size(); }
	const TCHAR* Name(){ return m_name.c_str(); }

	sVERTEX* VertData(){ return &m_verts[0]; }
	sTri* TriData(){ return &m_tris[0]; }

};

#endif // _MODEL_H
