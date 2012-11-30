#include "StdAfx.h"
#include "Mesh.h"


cMesh::cMesh()
{
    m_pMesh = NULL;

    //m_NumAttribTableEntries = 0;
    //m_pAttribTable = NULL;

    ZeroMemory( m_strMediaDir, sizeof( m_strMediaDir ) );
	// Setup vertex and index buffers
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
}


cMesh::~cMesh()
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}

void cMesh::Create()
{
	std::vector< sTri >		tris;
	std::vector< sTri >		colors;
	std::vector< sTri >		textures;


	D3D10_BUFFER_DESC descBuffer; //Se crea la estructura que describe el vertexBuffer
	memset(&descBuffer, 0, sizeof(descBuffer));
    descBuffer.Usage = D3D10_USAGE_DEFAULT;
	descBuffer.ByteWidth = sizeof(sVERTEX) * NumVerts();
    descBuffer.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    descBuffer.CPUAccessFlags = 0;
    descBuffer.MiscFlags = 0;
   
	D3D10_SUBRESOURCE_DATA resData;
	memset(&resData, 0, sizeof(resData));
	resData.pSysMem = VertData();
	Graphics()->GetDevice()->CreateBuffer(&descBuffer, &resData, &m_pVertexBuffer); //Creacion del VertexBuffer

	descBuffer.Usage = D3D10_USAGE_DEFAULT; //Se crea la estructura que describe el IndexBuffer
    descBuffer.ByteWidth = sizeof(WORD) * NumTris() * 3;        
    descBuffer.BindFlags = D3D10_BIND_INDEX_BUFFER;
    descBuffer.CPUAccessFlags = 0;
    descBuffer.MiscFlags = 0;
	resData.pSysMem = IdxData();
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

	D3DXMatrixIdentity(&mtxTransform);

	D3DX10CreateMesh(Graphics()->GetDevice(), VERTEXLAYOUT , sizeof(VERTEXLAYOUT),
		cDefaultVertex::Semantics(cDefaultVertex::POSITION), NumVerts(), NumTris(), D3DX10_MESH_32_BIT, &m_pMesh);

	m_pMesh->SetVertexData(0, (void *)VertData());

	m_pMesh->SetIndexData((void *)IdxData(), m_VertexIndex.size());

    //pMesh->SetAttributeData( (UINT*)m_Attributes.GetData() );
    //m_Attributes.RemoveAll();
}

void cMesh::Scale(float amt)
{
	int size = m_Vertex.size();
	for(int i=0; i<size; i++)
	{
		D3DXVec3Scale(&m_Vertex[i].vPosition, &m_Vertex[i].vPosition, amt);
	}
}

void cMesh::Update(float timeDelta)
{
	UpdateTransform();

	//seteo la matriz de transfromacion
	Graphics()->SetWorldMtx(mtxTransform);
	//update de las variables de Matrices y luces
	Graphics()->UpdateMatrices();
	Graphics()->UpdateLights();
}

void cMesh::Draw()
{
	UINT uiStride = sizeof(sVERTEX);
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
		Graphics()->SetTexture(0, m_pSRView);
        Graphics()->GetDefaultTechnique()->GetPassByIndex(uiCurPass)->Apply(0);
		Graphics()->GetDevice()->DrawIndexed(m_VertexIndex.size() * 3, 0, 0);
    }	
}

void cMesh::UpdateTransform()
{
	//Se calcula la matriz de transformacion
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&mtxTransform);
	D3DXMatrixScaling(&matrix, vScale.x, vScale.y, vScale.z);
	D3DXMatrixMultiply(&mtxTransform, &mtxTransform, &matrix);
	D3DXMatrixRotationYawPitchRoll(&matrix, vRotation.y, vRotation.x, vRotation.z);
	D3DXMatrixMultiply(&mtxTransform, &mtxTransform, &matrix);
	D3DXMatrixTranslation(&matrix, vTranslation.x, vTranslation.y, vTranslation.z);
	D3DXMatrixMultiply(&mtxTransform, &mtxTransform, &matrix);	
}

float cMesh::GenRadius()
{
	float best = 0.f;
	int size = m_Vertex.size();
	for(int i=0; i<size; i++)
	{
		float curr = D3DXVec3Length(&m_Vertex[i].vPosition);
		if(curr > best)
			best = curr;
	}
	return best;
}