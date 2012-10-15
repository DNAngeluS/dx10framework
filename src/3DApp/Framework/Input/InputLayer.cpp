#include "stdafx.h"
#include "InputLayer.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Application.h"
#include "Window.h"

cInputLayer* cInputLayer::m_pGlobalILayer = NULL;

cInputLayer::cInputLayer( 
	HINSTANCE hInst, 
	HWND hWnd, 
	bool bExclusive, 
	bool bUseKeyboard, 
	bool bUseMouse )
{


	m_pKeyboard = NULL;
	m_pMouse = NULL;

	if( m_pGlobalILayer )
	{
		throw cError(L"cInputLayer ya fue inicializado!\n");
	}
	m_pGlobalILayer = this;

	HRESULT hr;

	/**
	 * Crea el objeto DI8
	 */

	hr = DirectInput8Create( hInst, DIRECTINPUT_VERSION, 
						IID_IDirectInput8, (void**)&m_pDI, NULL );
    if( FAILED(hr) )
	{
		throw cError(L"DirectInput8 no pudo ser creado\n"); 
	}

	try 
	{
		if( bUseKeyboard )
		{
			m_pKeyboard = new cKeyboard( hWnd );
		}
		if( bUseMouse )
		{
			m_pMouse = new cMouse( hWnd, bExclusive );
		}
	}
	catch( ... )
	{
		SafeRelease( m_pDI );
		throw;
	}

}


cInputLayer::~cInputLayer()
{
	if( m_pDI )
	{
		if( m_pKeyboard )
		{
			delete m_pKeyboard; // Elimina el teclado.
		}

		if( m_pMouse )
		{
			delete m_pMouse; // Elimina el mouse.
		}
		SafeRelease( m_pDI );
	}
	m_pGlobalILayer = NULL;
}

void cInputLayer::UpdateDevices()
{
	if( m_pKeyboard )
	{
		m_pKeyboard->Update();
	}
	if( m_pMouse )
	{
		m_pMouse->Update();
	}
}


void cInputLayer::SetFocus()
{
	if( m_pKeyboard )
	{
		m_pKeyboard->ClearTable();
	}
	if( m_pMouse )
	{
		m_pMouse->Acquire();
	}
}


void cInputLayer::KillFocus()
{
	if( m_pKeyboard )
	{
		m_pKeyboard->ClearTable();
	}
	if( m_pMouse )
	{
		m_pMouse->UnAcquire();
	}
}
