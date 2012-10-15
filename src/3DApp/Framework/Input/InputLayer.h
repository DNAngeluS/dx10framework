#ifndef _INPUTLAYER_H
#define _INPUTLAYER_H

#include <dinput.h>
#include "Keyboard.h"
#include "Mouse.h"

class cInputLayer
{
	cKeyboard*		m_pKeyboard;
	cMouse*			m_pMouse;

	// El objeto de DirectInput8
	LPDIRECTINPUT8	m_pDI;
	
	static cInputLayer* m_pGlobalILayer;

	cInputLayer( 
		HINSTANCE hInst, 
		HWND hWnd, 
		bool bExclusive, 
		bool bUseKeyboard = true, 
		bool bUseMouse = true );

public:

	virtual ~cInputLayer();

	cKeyboard* GetKeyboard()
	{
		return m_pKeyboard;
	}

	cMouse* GetMouse()
	{
		return m_pMouse;
	}

	void UpdateDevices();

	static cInputLayer* GetInput()
	{
		return m_pGlobalILayer;
	}

	LPDIRECTINPUT8 GetDInput()
	{
		return m_pDI; 
	}

	void SetFocus(); // Llamada cuando la app gana foco
	void KillFocus(); // Llamada cuando la app pierde foco

	static void Create( 
		HINSTANCE hInst, 
		HWND hWnd, 
		bool bExclusive, 
		bool bUseKeyboard = true, 
		bool bUseMouse = true )
	{
		// El constructor realiza todo el seteo
		new cInputLayer( 
			hInst, 
			hWnd, 
			bExclusive, 
			bUseKeyboard, 
			bUseMouse );
	}
};

inline cInputLayer* Input()
{
	return cInputLayer::GetInput();
}

#endif //_INPUTLAYER_H
