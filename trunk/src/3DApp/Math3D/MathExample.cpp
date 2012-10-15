#include "stdafx.h"

#include <vector>
#include <string>
using namespace std;

#include <mmsystem.h> // timeGetTime

class MathExample : public cApplication, public iKeyboardReceiver
{

public:

	float m_delta1, m_delta2, m_delta3, m_delta4, m_delta5;

	//==========--------------------------  cApplication

	//virtual void Init();
	virtual void SceneInit();
	//virtual void Update(float timeDelta);
	virtual void DoFrame( float timeDelta );

	MathExample() :
		cApplication()
	{
		m_title = wstring( L"Ejemplo DirecX" );
		m_width = 1024;
		m_height = 768;
	}

	~MathExample()
	{

	}

};

cApplication* CreateApplication()
{
	return new MathExample();
}

void DestroyApplication( cApplication* pApp )
{
	delete pApp;
}

void MathExample::SceneInit()
{
	float last = (float)timeGetTime();
	float curr = (float)timeGetTime();
	point3 v1 = point3(2.0f, 2.0f, 1.0f);	
	D3DXVECTOR3 u1;	u1.x = 2.0f; u1.y = 2.0f; u1.z = 1.0f;
	
	Vector3 y1(2.0f, 2.0f, 2.0f);
	
	last = (float)timeGetTime();
	for(int i = 0; i < 10000000; i++)
	{
		Vector3 y2 = Vector3(float(i), float(i), 1.0f); 
		//y1 * y2;		
	}
	curr = (float)timeGetTime();
	m_delta3 = (curr - last)/1000.f;
	
	last = (float)timeGetTime();
	for(int i = 0; i < 10000000; i++)
	{
		point3 v2 = point3(float(i), float(i), 1.0f);
		//v1 * v2;
	}
	curr = (float)timeGetTime();
	m_delta1 = (curr - last)/1000.f;

	last = (float)timeGetTime();
	for(int i = 0; i < 10000000; i++)
	{		
		D3DXVECTOR3 u2; u2.x = float(i); u2.y = float(i); u2.z = 1.0f; 
		//D3DXVec3Dot(&u1, &u2);
	}
	curr = (float)timeGetTime();
	m_delta2 = (curr - last)/1000.f;
/*
	last = (float)timeGetTime();
	for(int i = 0; i < 100000000; i++)
	{		
		int a = i; int b = i;
		a + b;
	}
	curr = (float)timeGetTime();
	m_delta4 = (curr - last)/1000.f;

	last = (float)timeGetTime();
	for(int j = 0; j < 100000000; j++)
	{		
		float a, b; a = (float)j; b = (float)j;
		a + b;
	}
	curr = (float)timeGetTime();
	m_delta5 = (curr - last)/1000.f;
*/
}

void MathExample::DoFrame( float timeDelta )
{	
	TCHAR time1[256]; 
	float colClear[4] = {0.0f, 0.0f, 0.0f, 1.0f};

	// Clear
	Graphics()->Clear(colClear);

	swprintf(time1, L"Math  =   %f\n", m_delta1);
	
	Graphics()->DrawTextString( 1, 1, D3DXCOLOR(0.8f, 0.2f, 0.0f, 1.0f), time1);

	swprintf(time1, L"D3D  =   %f\n", m_delta2);
	
	Graphics()->DrawTextString( 1, 30, D3DXCOLOR(0.9f, 0.8f, 0.0f, 1.0f), time1);
	
	swprintf(time1, L"Vector3  =   %f\n", m_delta3);
	
	Graphics()->DrawTextString( 1, 60, D3DXCOLOR(0.0f, 0.8f, 0.4f, 1.0f), time1);

	swprintf(time1, L"int  =   %f\n", m_delta4);
	
	Graphics()->DrawTextString( 1, 90, D3DXCOLOR(0.0f, 0.4f, 0.4f, 1.0f), time1);

	swprintf(time1, L"float  =   %f\n", m_delta5);
	
	Graphics()->DrawTextString( 1, 120, D3DXCOLOR(0.0f, 0.4f, 0.4f, 1.0f), time1);
	

	// Trae el backbuffer
	Graphics()->Present();
}
