#include "stdafx.h"
#include "InputLayer.h"
#include "window.h"

#include <stack>
using namespace std;

#include "Keyboard.h"


cKeyboard::cKeyboard( HWND hWnd )
{
	m_pTarget = NULL;
	
	HRESULT hr;

	/**
	 * Getter del device de DirectInput
	 */
	LPDIRECTINPUT8 pDI = Input()->GetDInput();

	/**
	 * Crea el keyboard device
     */
	hr = Input()->GetDInput()->CreateDevice( GUID_SysKeyboard, &m_pDevice, NULL );
    if( FAILED(hr) )
    { 
        throw cError(L"[cKeyboard::cKeyboard]: Keyboard no pudo ser creado\n");
    } 
 
	/**
	 * Setea el formato del keyboard
	 */
    hr = m_pDevice->SetDataFormat(&c_dfDIKeyboard); 
    if( FAILED(hr) )
    { 
		SafeRelease( m_pDevice );
        throw cError(L"[cKeyboard::cKeyboard]: Keyboard no pudo ser seteado\n");
    } 
 
    /**
	 * Setea el nivel de cooperacion
	 */
    hr = m_pDevice->SetCooperativeLevel(
		hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
    if( FAILED(hr) )
    { 
		SafeRelease( m_pDevice );
        throw cError(L"[cKeyboard::cKeyboard]: No se pudo cambiar el nivel de cooperacion del keyboard\n");
    } 

	memset( m_keyState, 0, 256*sizeof(bool) );	
}

cKeyboard::~cKeyboard()
{
	if( m_pDevice )
	{
		m_pDevice->Unacquire();
		SafeRelease( m_pDevice );
	}
}



void cKeyboard::SetReceiver( iKeyboardReceiver* pTarget )
{
	// Setea el nuevo objetivo.
	m_pTarget = pTarget;
}


bool cKeyboard::Poll( int key )
{
	if( m_keyState[key] & 0x80 )
		return true;
	return false;
}


eResult cKeyboard::Update()
{
	BYTE     newState[256]; 
    HRESULT  hr; 
 
    hr = m_pDevice->Poll(); 
    hr = m_pDevice->GetDeviceState(sizeof(newState),(LPVOID)&newState); 

    if( FAILED(hr) )
    { 
		hr = m_pDevice->Acquire();
		if( FAILED( hr ) )
		{
			return resFailed;
		}

	    hr = m_pDevice->Poll(); 
	    hr = m_pDevice->GetDeviceState(sizeof(newState),(LPVOID)&newState); 
		if( FAILED( hr ) )
		{
			return resFailed;
		}
    } 


	if( m_pTarget )
	{
		int i;
		for( i=0; i< 256; i++ )
		{
			if( m_keyState[i] != newState[i] )
			{
				// Nuevo estado en la tecla
				if( !(newState[i] & 0x80) )
				{
					// Fue liberada
					m_pTarget->KeyUp( i );
				}
				else
				{
					// No hacer nada acaba de ser presionada
				}
			}

			// Copia el estado actual
			m_keyState[i] = newState[i];

			if( Poll( i ) )
			{
				// Fue presionada
				m_pTarget->KeyDown( i );
			}
		}
	}
	else
	{
		// Copia el nuevo estado.
		memcpy( m_keyState, newState, 256 );
	}
 
	return resAllGood;
}


