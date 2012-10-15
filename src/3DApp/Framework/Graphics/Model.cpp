#include "StdAfx.h"

#include "Model.h"
#include "File.h"

using namespace std;

cModel::cModel(const TCHAR* name, int nVerts, int nTris) :
	m_name(name)
{
	int i;

	m_verts.reserve(nVerts);
	cGraphicsLayer::cDefaultVertex vert;
	for(i=0; i<nVerts; i++)
	{
		m_verts.push_back(vert);
	}

	m_tris.reserve(nTris);
	sTri tri;
	for(i=0; i<nTris; i++)
	{
		m_tris.push_back(tri);
	}

	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
}

cModel::~cModel()
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}

cModel::cModel(const LPCTSTR filename)
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

	file.ReadXML();
	file.GetXmlPrefix(&tag);
	file.GetXmlValue(&value);

	if(value != L"tgcScene")
	{
		//Break formato incorrecto
	}
	

	file.ReadXML(); //<name>
	file.ReadXML(); //Text_Node

	file.GetXmlValue(&value);
	

	//Guardar name de la scene

	file.ReadXML(); //</name>

	file.ReadXML(); //<texturesExport />
	pReader->GetLocalName(&value, NULL);
	pReader->GetAttributeCount(&iAtributeCount);

	file.ReadXML(); //<lightmapExport />
	pReader->GetAttributeCount(&iAtributeCount);

	file.ReadXML(); //<sceneBoundingBox />
	pReader->GetAttributeCount(&iAtributeCount);

	file.ReadXML(); //<materials>
	pReader->GetAttributeCount(&iAtributeCount);

	pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
	pReader->GetValue(&value, NULL);

	t1 = _wtoi(value);	
	for(int i = 0; i < t1; i++)
	{
		file.ReadXML(); //<m>		

		file.ReadXML(); //<ambient>
		file.ReadXML(); //Text_node
		file.ReadXML(); //</ambient>

		file.ReadXML(); //<diffuse>
		file.ReadXML(); //Text_node
		file.ReadXML(); //</diffuse>

		file.ReadXML(); //<specular>
		file.ReadXML(); //Text_node
		file.ReadXML(); //</specular>		

		file.ReadXML(); //<opacity>
		file.ReadXML(); //Text_node
		file.ReadXML(); //</opacity>
		
		file.ReadXML(); //<bitmap>
		file.ReadXML(); //Text_node
		pReader->GetValue(&value, NULL);
		m_texture.append(Application()->m_dAppMedia+L"Textures\\");
		m_texture.append(value);
		file.ReadXML(); //</bitmap>
		
		file.ReadXML(); //</m>		
	}
		
	file.ReadXML(); //</Materials>

	file.ReadXML(); //<Meshes>	
	pReader->GetAttributeCount(&iAtributeCount);

	pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
	pReader->GetValue(&value, NULL);

	t1 = _wtoi(value);	
	for(int i = 0; i < t1; i++)
	{
		file.ReadXML(); //<mesh>		

		pReader->MoveToNextAttribute();//name

		pReader->MoveToNextAttribute();//pos

		pReader->MoveToNextAttribute();//matId

		pReader->MoveToNextAttribute();//color

		pReader->MoveToNextAttribute();//visibility

		pReader->MoveToNextAttribute();//lightmap

		file.ReadXML(); //<boundingBox />
		pReader->GetAttributeCount(&iAtributeCount);

		file.ReadXML(); //<coordinatesIdx>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);
		//Se guarda la cantidad de triangulos
		nTris = _wtoi(value) / 3;
		file.ReadXML();	//Text_Node
		pReader->GetValue(&value, NULL);	//String

		file.TokenizeXmlValue(&m_tokens, ' ');

		m_tris.reserve(nTris);
		for(int j = 0; j < nTris; j++)
		{
			sTri tri;
						
			tri.v[0] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[1] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[2] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();

			m_tris.push_back(tri);
		}

		file.ReadXML(); //</coordinatesIdx>

		file.ReadXML(); //<textCoordsIdx>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);

		nTex = _wtoi(value) / 3;

		file.ReadXML(); //Text_Node
		pReader->GetValue(&value, NULL);	//String
		file.TokenizeXmlValue(&m_tokens, ' ');
		
		m_textures.reserve(nTex);
		for(int j = 0; j < nTex; j++)
		{
			sTri tri;
						
			tri.v[0] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[1] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[2] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();

			m_textures.push_back(tri);
		}

		file.ReadXML(); //</textCoordsIdx>

		file.ReadXML(); //<colorsIdx>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);

		nColors = _wtoi(value) / 3;

		file.ReadXML(); //Text_Node
		pReader->GetValue(&value, NULL);	//String
		file.TokenizeXmlValue(&m_tokens, ' ');
		
		m_colors.reserve(nColors);
		for(int j = 0; j < nColors; j++)
		{
			sTri tri;
						
			tri.v[0] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[1] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[2] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();

			m_colors.push_back(tri);
		}		

		file.ReadXML(); //</colorsIdx>

		file.ReadXML(); //<matIds>
		file.ReadXML(); //Text_Node
		file.ReadXML(); //</matIds>

		file.ReadXML(); //<vertices>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);
		//Se guarda la cantidad de vertices
		nVerts = _wtoi(value) / 3;

		file.ReadXML(); //Text_Node
		pReader->GetValue(&value, NULL);	//String


		file.TokenizeXmlValue(&m_tokens, ' ');
		vertices.reserve(nVerts);
		for(int j = 0; j < nVerts; j++)
		{
			D3DXVECTOR3 v;
			v.x = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();
			v.y = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();
			v.z = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();

			vertices.push_back(v);
		}

		
		file.ReadXML(); //</vertices>

		file.ReadXML(); //<normals>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);
		//Se guarda la cantidad de normales
		normals.reserve(_wtoi(value) / 3);

		file.ReadXML(); //Text_Node
		file.TokenizeXmlValue(&m_tokens, ' ');

		for(int j = 0; j < normals.capacity(); j++)
		{
			D3DXVECTOR3 v;
			v.x = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();
			v.y = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();
			v.z = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();

			normals.push_back(v);
		}

		file.ReadXML(); //</normals>

		file.ReadXML(); //<texCoords>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);
		//Se guarda la cantidad de normales
		textures.reserve(_wtoi(value) / 2);

		file.ReadXML(); //Text_Node
		file.TokenizeXmlValue(&m_tokens, ' ');

		for(int j = 0; j < textures.capacity(); j++)
		{
			D3DXVECTOR2 v;
			v.x = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();
			v.y = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();

			textures.push_back(v);
		}
		file.ReadXML(); //</texCoords>

		file.ReadXML(); //<colors>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);
		colors.reserve(_wtoi(value) / 3);

		file.ReadXML(); //Text_Node
		file.TokenizeXmlValue(&m_tokens, ' ');

		for(int j = 0; j < colors.capacity();j++)
		{
			D3DXCOLOR c;
			c.r = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();
			c.g = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();
			c.b = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();
			c.a = 1.0f;

			colors.push_back(c);
		}

		file.ReadXML(); //</colors>

		file.ReadXML(); //</mesh>

		nVerts = nTris * 3;

		m_verts.reserve(nVerts);
		for(int j = 0; j < nTris; j++)
		{
			cGraphicsLayer::cDefaultVertex v1, v2, v3;

			v1.m_vPosition	=	vertices[m_tris[j].v[0]];
			v1.m_vColor		=	colors[m_colors[j].v[0]];
			v1.m_vNormal	=	normals[m_tris[j].v[0]];
			v1.m_TexCoords	=	textures[m_textures[j].v[0]];
			m_verts.push_back(v1);
			tris.push_back(m_verts.size() - 1);

			v2.m_vPosition	=	vertices[m_tris[j].v[1]];
			v2.m_vColor		=	colors[m_colors[j].v[1]];
			v2.m_vNormal	=	normals[m_tris[j].v[1]];
			v2.m_TexCoords	=	textures[m_textures[j].v[1]];
			m_verts.push_back(v2);
			tris.push_back(m_verts.size() - 1);

			v3.m_vPosition	=	vertices[m_tris[j].v[2]];
			v3.m_vColor		=	colors[m_colors[j].v[2]];
			v3.m_vNormal	=	normals[m_tris[j].v[2]];
			v3.m_TexCoords	=	textures[m_textures[j].v[2]];
			m_verts.push_back(v3);
			tris.push_back(m_verts.size() - 1);
		}
	}

	file.ReadXML(); //</meshes>

	file.ReadXML(); //</tgcScene>

	// Setup vertex and index buffers
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;

	D3D10_BUFFER_DESC descBuffer; //Se crea la estructura que describe el vertexBuffer
	memset(&descBuffer, 0, sizeof(descBuffer));
    descBuffer.Usage = D3D10_USAGE_DEFAULT;
	descBuffer.ByteWidth = sizeof(cGraphicsLayer::cDefaultVertex) * NumVerts();
    descBuffer.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    descBuffer.CPUAccessFlags = 0;
    descBuffer.MiscFlags = 0;
   
	D3D10_SUBRESOURCE_DATA resData;
	memset(&resData, 0, sizeof(resData));
    resData.pSysMem = &m_verts[0];
	Graphics()->GetDevice()->CreateBuffer(&descBuffer, &resData, &m_pVertexBuffer); //Creacion del VertexBuffer

	descBuffer.Usage = D3D10_USAGE_DEFAULT; //Se crea la estructura que describe el IndexBuffer
    descBuffer.ByteWidth = sizeof(WORD) * NumTris() * 3;        
    descBuffer.BindFlags = D3D10_BIND_INDEX_BUFFER;
    descBuffer.CPUAccessFlags = 0;
    descBuffer.MiscFlags = 0;
    resData.pSysMem = &tris[0];
    Graphics()->GetDevice()->CreateBuffer(&descBuffer, &resData, &m_pIndexBuffer); //Creacion del IndexBuffer

	D3DX10_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory( &loadInfo, sizeof(D3DX10_IMAGE_LOAD_INFO) );
	loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	loadInfo.Format = DXGI_FORMAT_BC1_UNORM;

	ID3D10ShaderResourceView *pSRView = NULL;
	D3DX10CreateShaderResourceViewFromFile(Graphics()->GetDevice(), m_texture.c_str(), &loadInfo, NULL, &m_pSRView, NULL );

	vTranslation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXMatrixIdentity(&mTransform);
}

void cModel::Scale(float amt)
{
	int size = m_verts.size();
	for(int i=0; i<size; i++)
	{
		D3DXVec3Scale(&m_verts[i].m_vPosition, &m_verts[i].m_vPosition, amt);
	}
}

void cModel::Update(float timeDelta)
{
	UpdateTransform();

	//seteo la matriz de transfromacion
	Graphics()->SetWorldMtx(mTransform);
	//update de las variables de Matrices y luces
	Graphics()->UpdateMatrices();
	Graphics()->UpdateLights();
}

void cModel::Draw()
{
	UINT uiStride = sizeof(cGraphicsLayer::cDefaultVertex);
	UINT uiOffset = 0;

	//Se setea el buffer para renderizar
    Graphics()->GetDevice()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &uiStride, &uiOffset);
    Graphics()->GetDevice()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);	
    Graphics()->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//se obtiene el shader y se realizan las pasadas necesarias
	D3D10_TECHNIQUE_DESC descTechnique;
    Graphics()->GetDefaultTechnique()->GetDesc(&descTechnique);
    for(UINT uiCurPass = 0; uiCurPass < descTechnique.Passes; uiCurPass++)
    {
		//Graphics()->GetDevice()->PSSetShaderResources(1, 1, &m_pSRView); //Pasar la view
		Graphics()->SetTexture(0, m_pSRView);
        Graphics()->GetDefaultTechnique()->GetPassByIndex(uiCurPass)->Apply(0);
		Graphics()->GetDevice()->DrawIndexed(m_tris.size() * 3, 0, 0);
    }	
}

void cModel::UpdateTransform()
{
	//Se calcula la matriz de transformacion
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&mTransform);
	D3DXMatrixScaling(&matrix, vScale.x, vScale.y, vScale.z);
	D3DXMatrixMultiply(&mTransform, &mTransform, &matrix);
	D3DXMatrixRotationYawPitchRoll(&matrix, vRotation.y, vRotation.x, vRotation.z);
	D3DXMatrixMultiply(&mTransform, &mTransform, &matrix);
	D3DXMatrixTranslation(&matrix, vTranslation.x, vTranslation.y, vTranslation.z);
	D3DXMatrixMultiply(&mTransform, &mTransform, &matrix);	
}

float cModel::GenRadius()
{
	float best = 0.f;
	int size = m_verts.size();
	for(int i=0; i<size; i++)
	{
		float curr = D3DXVec3Length(&m_verts[i].m_vPosition);
		if(curr > best)
			best = curr;
	}
	return best;
}
