#include "StdAfx.h"

#include <list>
using namespace std;

cApplication* cApplication::m_pGlobalApp = NULL;

HINSTANCE g_hInstance;

HINSTANCE AppInstance()
{
	return g_hInstance;
}

/**
 * Se abstrae la creacion de WinMain que se repite en todos
 * los programas Win32
 */
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	cApplication* pApp;

	g_hInstance = hInstance;

	try
	{
		pApp = CreateApplication();

		pApp->Init();
		pApp->SceneInit();
		pApp->Run();
	}
	catch( cError& err )
	{
		/**
		 * Bloquea los graficos antes de intentar leer un dialog
		 */
		if( Graphics() )
		{
			Graphics()->DestroyAll();
		}
		MessageBox( NULL, err.GetText(), L"Error!", MB_OK|MB_ICONEXCLAMATION );

		// Limpia todo
		delete pApp;
		return 0;
	}

	delete pApp;
	return 0;
}



cApplication::cApplication()
{
	char	filepath[_MAX_PATH] = "\0";
	wchar_t tmp[_MAX_PATH] = L"\0";		
	if( m_pGlobalApp )
	{
		throw cError(L"Ya existe una instancia de Applicacion\n");
	}
	m_pGlobalApp = this;
	//Setear variables de la ventana
	m_title = wstring(L"3D Framework");
	m_width = 640;
	m_height = 480;
	m_bpp = 32;
	m_bActive = true;

	//Se crean los Paths absolutos
	_fullpath(filepath, ".\\", _MAX_PATH);
	mbstowcs(tmp, filepath ,_MAX_PATH);
	m_dAppRoot.append(tmp);
	m_dAppRoot.append(L"\\");

	_fullpath(filepath, ".\\Media", _MAX_PATH);
	mbstowcs(tmp, filepath ,_MAX_PATH);
	m_dAppMedia.append(tmp);
	m_dAppMedia.append(L"\\");

	_fullpath(filepath, "..\\", _MAX_PATH);
	mbstowcs(tmp, filepath ,_MAX_PATH);
	m_dRoot.append(tmp);
	m_dRoot.append(L"\\");

	_fullpath(filepath, "..\\Media", _MAX_PATH);
	mbstowcs(tmp, filepath ,_MAX_PATH);
	m_dMedia.append(tmp);
	m_dMedia.append(L"\\");
	
}


cApplication::~cApplication()
{
	delete Graphics();
	delete Sound();
	delete MainWindow();
}

void cApplication::Init()
{
	InitPrimaryWindow();
	InitGraphics();
	InitInput();
	InitSound();
	InitExtraSubsystems();
}


void cApplication::Run()
{
	bool done = false;

	static float lastTime = (float)timeGetTime(); 

	while( !done )
	{
		/**
		 * Does the user want to quit?
		 */
	/*	if( Input()->GetKeyboard() )
		{
			if( Input()->GetKeyboard()->Poll( DIK_ESCAPE ) ||
				Input()->GetKeyboard()->Poll( DIK_Q ) )
			{
				PostMessage( MainWindow()->GetHWnd(), WM_CLOSE, 0, 0 );
			}
		}*/


		/**
		 * pump del mensaje
		 */
		while( !done && MainWindow()->HasMessages() )
		{
			if( resFalse == MainWindow()->Pump() )
				done = true;
		}



		/**
		 * Calcula el delta de tiempo y renderiza el frame.
		 */
		float currTime = (float)timeGetTime();
		float delta = (currTime - lastTime)/1000.f;

		if( m_bActive  )
		{
			Update( delta );
			DoFrame( delta );
		}
		else
		{
			DoIdleFrame( delta );
		}

		lastTime = currTime;
	}
}

void cApplication::Update( float timeDelta )
{
	// Update de los devices de Input
	if( Input() )
		Input()->UpdateDevices();
}

void cApplication::DoFrame( float timeDelta )
{

}

void cApplication::DoIdleFrame( float timeDelta )
{
}


void cApplication::SceneInit()
{
	// por default no hay Scene para iniciar
}


void cApplication::InitPrimaryWindow()
{
	new cWindow( m_width, m_height, m_title.c_str() );

	MainWindow()->RegisterClass();
	MainWindow()->InitInstance();
}


void cApplication::InitGraphics()
{
	cGraphicsLayer::Create( 
		MainWindow()->GetHWnd(), 
		m_width, m_height);
}


void cApplication::InitInput()
{
	cInputLayer::Create( AppInstance(), MainWindow()->GetHWnd(), NULL, true, true );
}


void cApplication::InitSound()
{
	cSoundLayer::Create( MainWindow()->GetHWnd() );
}


void cApplication::InitExtraSubsystems()
{
}


void cApplication::SceneEnd()
{
}
