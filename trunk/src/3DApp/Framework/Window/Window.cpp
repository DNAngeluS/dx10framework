#include "StdAfx.h"

#include <assert.h>
using namespace std;

cWindow* cWindow::m_pGlobalWindow = NULL;

/**
 * Se catchean todos los errores en wndproc y se elevan a nuestro message pump
 */
cError* g_pThrownError = NULL;


LRESULT CALLBACK GlobalWndProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam )
{
	assert( MainWindow() );

	return MainWindow()->WndProc( hWnd, uMsg, wParam, lParam );
}



cWindow::cWindow( int width, int height, const TCHAR* name ) :
	m_Width( width ),
	m_Height( height ),
	m_hWnd( NULL )
{
	/**
	 * Solo puede haber una ventana.
	 */
	if( m_pGlobalWindow )
	{
		throw cError(L"Ventana ya instancaiada.\n" );
	}

	m_pGlobalWindow = this;
	m_bActive = false;


	// copy the name over
	m_name = wstring( name );
}

cWindow::~cWindow()
{
	DestroyWindow( m_hWnd );
	m_pGlobalWindow = NULL;
}


LRESULT cWindow::WndProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam )
{
	/**
	 * Si tenemos mensajes despues de un error volver inmediatamente.
	 */
	if( g_pThrownError )
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);	
	}


	bool bRunDefault = false;

	try
	{
		switch( uMsg ) 
		{
		case WM_CREATE:
			{
				MainWindow()->m_hWnd = hWnd;
				MainWindow()->m_bActive = true;
				break;
			}
		case WM_PAINT:
			{
				// No hacer nada.
				ValidateRect( hWnd, NULL );
				break;
			}
		case WM_KEYDOWN:
			{
				switch( wParam )
				{
				// Salir si Esc es presionada
				case VK_ESCAPE:
					{
						PostQuitMessage(0);
						break;
					}
				default:
					{
						break;
					}
				}
			}

		case WM_ERASEBKGND:
			{
				break;
			}
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}
		default:
			bRunDefault = true;
			break;
		}	
	}
	catch( cError& err )
	{
		g_pThrownError = new cError( err );
		return 0;
	}

	if( bRunDefault )
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}


void cWindow::RegisterClass( WNDCLASSEX* pWc )
{
	WNDCLASSEX wcex;
	if( pWc )
	{
		wcex = *pWc;
	}
	else
	{
		// Se completan valores por default
		wcex.cbSize = sizeof(WNDCLASSEX); 

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= GlobalWndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= AppInstance();
		wcex.hIcon			= 0;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= 0;
		wcex.lpszClassName	= m_name.c_str();
		wcex.hIconSm		= 0;
	}

	if( 0 == RegisterClassEx(&wcex) )
	{
		throw cError(L"[cWindow::RegisterClass]: No se pudo registrar la clase\n");
	}
}

void cWindow::InitInstance()
{

	HINSTANCE hi = AppInstance();

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = m_Width;
	rc.bottom = m_Height;
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	/**
	 * Capturamos el handle de la ventana cuando WM_CREATE es llamada
	 * en nuestra ventana.
	 */
	CreateWindow( 
	   m_name.c_str(), 
	   m_name.c_str(), 
	   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
	   0, 
	   0, 
	   rc.right - rc.left, 
	   rc.bottom - rc.top, 
	   NULL, 
	   NULL, 
	   AppInstance(), 
	   NULL);

   if ( !m_hWnd )
   {
		throw cError(L"[cWindow::InitInstance]: Fallo la creacion de la ventana\n");
   }

   // Dibujar
   ShowWindow( m_hWnd, SW_SHOW );
   UpdateWindow( m_hWnd );
}


bool cWindow::HasMessages()
{
	MSG msg;
    if( 0 == PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE ) )
	{
		// No habia mensajes
		return false;
	}
	return true;
}

eResult cWindow::Pump()
{
	MSG msg;
	PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

	if( WM_QUIT == msg.message )
		return resFalse;

	TranslateMessage( &msg );
    DispatchMessage( &msg );

	/**
	 * Checkea si hubo errores en la cola de mensajes
	 */
	if( g_pThrownError )
	{
		cError out = *g_pThrownError;
		delete g_pThrownError;
		throw out;
	}


	return resAllGood;
}
