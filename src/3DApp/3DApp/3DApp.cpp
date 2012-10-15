#include "StdAfx.h"
using namespace std;

class c3DApp : public cApplication, public iKeyboardReceiver
{
	cModel *m_pModel;
	wstring m_filename;
	wstring m_file;
	cFile  *m_pFile;
	void InitLights();

public:

	void PlaySound( int num );

	virtual void c3DApp::Update(float timeDelta);
	virtual void DoFrame( float timeDelta );
	virtual void SceneInit();

	virtual void SceneEnd()
	{
		delete m_pModel;
	}

	c3DApp() :
		cApplication()
	{
		m_title = wstring( L"Ejemplo DirecX" );
		m_width = 1024;
		m_height = 768;
		m_pModel = NULL;
//		m_filename = L"D:\\Projects\\Investigacion\\DirectX10Framework\\src\\3DApp\\3DApp\\Media\\RABBIT.O3D";
		m_filename = L"..\\Media\\RABBIT.O3D";
		m_file = Application()->m_dAppMedia + L"Box-TgcScene.xml";
		m_pVertexBuffer = NULL;
	}

	~c3DApp()
	{

	}

protected:

	ID3D10Buffer* m_pVertexBuffer;

};

cApplication* CreateApplication()
{
	return new c3DApp();
}

void DestroyApplication( cApplication* pApp )
{
	delete pApp;
}

void c3DApp::SceneInit()
{
	m_pModel = new cModel( m_file.c_str() );
	m_pModel->Scale(1.0f / m_pModel->GenRadius() );
	//m_pFile->OpenXML(m_file);
	InitLights();
}

void c3DApp::InitLights()
{
	Graphics()->AddLight(D3DXCOLOR(0.8f, 0.2f, 0.2f, 0.4f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

void c3DApp::Update(float timeDelta)
{
	m_pModel->vRotation.y = m_pModel->vRotation.y + 0.001f;
	//m_pModel->vRotation.x = m_pModel->vRotation.x + 0.0001f;
	m_pModel->Update(timeDelta);

	//Graphics()->m_vCamPos.x = Graphics()->m_vCamPos.x + 0.2f;
	//Graphics()->m_vCamPos.y = Graphics()->m_vCamPos.y + 1.0f;
	Graphics()->UpdateCam();
}

void c3DApp::DoFrame( float timeDelta )
{
	if(!Graphics()) 
		return;

	float colClear[4] = {0.1f, 0.1f, 1.0f, 1.0f};
	Graphics()->Clear(colClear);
	Graphics()->ClearDepthStencil(1.0f, 0);
	
	//Draw de los objetos
	m_pModel->Draw();

	Graphics()->Present();
}
