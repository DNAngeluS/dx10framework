#include "StdAfx.h"

#include "TgcParser.h"
#include "TgcXml.h"

using namespace std;

TgcParser::TgcParser(void)
{
}

void TgcParser::mesh(const LPCTSTR filename, cMesh** mesh)
{
	TgcXml file;	

	const WCHAR*	tag;
	const WCHAR*	value;
	WCHAR			previous_directory[MAX_PATH] = {'\0'};
	IXmlReader*		pReader;
	UINT			iAtributeCount;

	int t1, t2;

	int nVertexFields, nTris, nVerts, nColors, nTex;
	queue<wstring>		m_tokens;
	vector<D3DXVECTOR3>	vertices, normals;
	vector<D3DXVECTOR2>	textures;
	vector<D3DXCOLOR>	colors;
	vector<WORD>		tris;

	//Model Variables
	std::vector< sTri >		modelTris;
	std::vector< sTri >		modelColors;
	std::vector< sTri >		modelTextures;
	std::vector< sVERTEX >	modelVerts;
	std::wstring			modelName;
	std::wstring			modelTexture;

	std::wstring			working_directory, path;

	file.Open(filename);
	pReader = file.GetPointer();
	
	path.append(filename);
	working_directory = path.substr(0, path.find_last_of('\\', path.size()) ) + L"\\";

	GetCurrentDirectory(MAX_PATH, previous_directory);
	SetCurrentDirectory(path.c_str());

	file.Read();

	if(value != L"tgcScene")
	{
		//Break formato incorrecto
	}
	

	file.Read(); //<name>
	file.Read(); //Text_Node
	

	//Guardar name de la scene

	file.Read(); //</name>

	file.Read(); //<texturesExport />
	pReader->GetLocalName(&value, NULL);
	pReader->GetAttributeCount(&iAtributeCount);

	file.Read(); //<lightmapExport />
	pReader->GetAttributeCount(&iAtributeCount);

	file.Read(); //<sceneBoundingBox />
	pReader->GetAttributeCount(&iAtributeCount);

	file.Read(); //<materials>
	pReader->GetAttributeCount(&iAtributeCount);

	pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
	pReader->GetValue(&value, NULL);

	t1 = _wtoi(value);	
	for(int i = 0; i < t1; i++)
	{
		file.Read(); //<m>		
		
		file.Read(); //<ambient> or <SubM>
		pReader->GetLocalName(&value, NULL);
		if(StrCmpCW(value, L"ambient"))
		{
			file.Read(); //Text_node
			file.Read(); //</ambient>

			file.Read(); //<diffuse>
			file.Read(); //Text_node
			file.Read(); //</diffuse>

			file.Read(); //<specular>
			file.Read(); //Text_node
			file.Read(); //</specular>		

			file.Read(); //<opacity>
			file.Read(); //Text_node
			file.Read(); //</opacity>
		
			file.Read(); //<bitmap>
			file.Read(); //Text_node
			pReader->GetValue(&value, NULL);

			//GetCurrentDirectory(MAX_PATH, path);
			modelTexture.append(working_directory);
			modelTexture.append(L"Textures\\");
			modelTexture.append(value);
			file.Read(); //</bitmap>

			file.Read(); //</m>
		}
		if(StrCmpCW(value, L"subM"))
		{
			while(StrCmpCW(value, L"m"))
			{
				file.Read(); //Text_node
				file.Read(); //</ambient>

				file.Read(); //<diffuse>
				file.Read(); //Text_node
				file.Read(); //</diffuse>

				file.Read(); //<specular>
				file.Read(); //Text_node
				file.Read(); //</specular>		

				file.Read(); //<opacity>
				file.Read(); //Text_node
				file.Read(); //</opacity>
		
				file.Read(); //<bitmap>
				file.Read(); //Text_node
				pReader->GetValue(&value, NULL);
				modelTexture.append(Application()->m_dAppMedia+L"Textures\\");
				modelTexture.append(value);
				file.Read(); //</bitmap>

				file.Read(); //</m>
			}
		}

	}
		
	file.Read(); //</Materials>

	file.Read(); //<Meshes>	
	pReader->GetAttributeCount(&iAtributeCount);

	pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
	pReader->GetValue(&value, NULL);

	t1 = _wtoi(value);	
	for(int i = 0; i < t1; i++)
	{
		file.Read(); //<mesh>		

		pReader->MoveToNextAttribute();//name

		pReader->MoveToNextAttribute();//pos

		pReader->MoveToNextAttribute();//matId

		pReader->MoveToNextAttribute();//color

		pReader->MoveToNextAttribute();//visibility

		pReader->MoveToNextAttribute();//lightmap

		file.Read(); //<boundingBox />
		pReader->GetAttributeCount(&iAtributeCount);

		file.Read(); //<coordinatesIdx>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);
		//Se guarda la cantidad de triangulos
		nTris = _wtoi(value) / 3;
		file.Read();	//Text_Node
		pReader->GetValue(&value, NULL);	//String

		file.TokenizeValue(&m_tokens, ' ');

		modelTris.reserve(nTris);
		for(int j = 0; j < nTris; j++)
		{
			sTri tri;
						
			tri.v[0] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[1] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[2] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();

			modelTris.push_back(tri);
		}

		file.Read(); //</coordinatesIdx>

		file.Read(); //<textCoordsIdx>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);

		nTex = _wtoi(value) / 3;

		file.Read(); //Text_Node
		pReader->GetValue(&value, NULL);	//String
		file.TokenizeValue(&m_tokens, ' ');
		
		modelTextures.reserve(nTex);
		for(int j = 0; j < nTex; j++)
		{
			sTri tri;
						
			tri.v[0] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[1] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[2] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();

			modelTextures.push_back(tri);
		}

		file.Read(); //</textCoordsIdx>

		file.Read(); //<colorsIdx>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);

		nColors = _wtoi(value) / 3;

		file.Read(); //Text_Node
		pReader->GetValue(&value, NULL);	//String
		file.TokenizeValue(&m_tokens, ' ');
		
		modelColors.reserve(nColors);
		for(int j = 0; j < nColors; j++)
		{
			sTri tri;
						
			tri.v[0] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[1] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();
			tri.v[2] = _wtoi(m_tokens.front().c_str());
			m_tokens.pop();

			modelColors.push_back(tri);
		}		

		file.Read(); //</colorsIdx>

		file.Read(); //<matIds>
		file.Read(); //Text_Node
		file.Read(); //</matIds>

		file.Read(); //<vertices>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);
		//Se guarda la cantidad de vertices
		nVerts = _wtoi(value) / 3;

		file.Read(); //Text_Node
		pReader->GetValue(&value, NULL);	//String


		file.TokenizeValue(&m_tokens, ' ');
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

		
		file.Read(); //</vertices>

		file.Read(); //<normals>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);
		//Se guarda la cantidad de normales
		normals.reserve(_wtoi(value) / 3);

		file.Read(); //Text_Node
		file.TokenizeValue(&m_tokens, ' ');

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

		file.Read(); //</normals>

		file.Read(); //<texCoords>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);
		//Se guarda la cantidad de normales
		textures.reserve(_wtoi(value) / 2);

		file.Read(); //Text_Node
		file.TokenizeValue(&m_tokens, ' ');

		for(int j = 0; j < textures.capacity(); j++)
		{
			D3DXVECTOR2 v;
			v.x = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();
			v.y = (float)_wtof(m_tokens.front().c_str());
			m_tokens.pop();

			textures.push_back(v);
		}
		file.Read(); //</texCoords>

		file.Read(); //<colors>
		pReader->MoveToAttributeByName(L"count", NULL); //se busca el count
		pReader->GetValue(&value, NULL);
		colors.reserve(_wtoi(value) / 3);

		file.Read(); //Text_Node
		file.TokenizeValue(&m_tokens, ' ');

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

		file.Read(); //</colors>

		file.Read(); //</mesh>

		nVerts = nTris * 3;

		modelVerts.reserve(nVerts);
		for(int j = 0; j < nTris; j++)
		{
			sVERTEX v1, v2, v3;

			v1.vPosition	=	vertices[modelTris[j].v[0]];
			v1.cColor		=	colors[modelColors[j].v[0]];
			v1.vNormal	=	normals[modelTris[j].v[0]];
			v1.vTexCoords	=	textures[modelTextures[j].v[0]];
			modelVerts.push_back(v1);
			tris.push_back(modelVerts.size() - 1);

			v2.vPosition	=	vertices[modelTris[j].v[1]];
			v2.cColor		=	colors[modelColors[j].v[1]];
			v2.vNormal	=	normals[modelTris[j].v[1]];
			v2.vTexCoords	=	textures[modelTextures[j].v[1]];
			modelVerts.push_back(v2);
			tris.push_back(modelVerts.size() - 1);

			v3.vPosition	=	vertices[modelTris[j].v[2]];
			v3.cColor		=	colors[modelColors[j].v[2]];
			v3.vNormal	=	normals[modelTris[j].v[2]];
			v3.vTexCoords	=	textures[modelTextures[j].v[2]];
			modelVerts.push_back(v3);
			tris.push_back(modelVerts.size() - 1);
		}
	}

	file.Read(); //</meshes>

	file.Read(); //</tgcScene>

	SetCurrentDirectory(previous_directory);

}

TgcParser::~TgcParser(void)
{
}
