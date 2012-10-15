#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <memory.h>

class cInputLayer;

/**
 * Cualquier objeto que implemente esta interface de poder recibir el input del teclado.
 */
struct iKeyboardReceiver
{
	virtual void KeyUp( int key ){};
	virtual void KeyDown( int key ){};
};


class cKeyboard  
{

	LPDIRECTINPUTDEVICE8	m_pDevice;	// Device de DirectInput

	char					m_keyState[256];

	iKeyboardReceiver*		m_pTarget;

public:

	void ClearTable()
	{
		memset( m_keyState, 0, sizeof(char)*256 );
	}

	cKeyboard( HWND hWnd );
	~cKeyboard();

	// Poll para ver que tecla fue presionada
	bool Poll( int key );

	// Setea el KeyboardReceiver como el input actual
	void SetReceiver( iKeyboardReceiver* pTarget );

	eResult Update();
};

#endif //_KEYBOARD_H
