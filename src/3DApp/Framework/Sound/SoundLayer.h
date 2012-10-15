#ifndef _SOUNDLAYER_H
#define _SOUNDLAYER_H

#include <dsound.h>
#include "Errors.h"

class cSound;

class cSoundLayer  
{

	LPDIRECTSOUND8			m_pDSound;
	LPDIRECTSOUNDBUFFER8	m_pPrimary; // Mixer principal

	static cSoundLayer* m_pGlobalSLayer;

	cSoundLayer( HWND hWnd );

public:
	virtual ~cSoundLayer();

	static cSoundLayer* GetSound()
	{ 
		return m_pGlobalSLayer; 
	}

	LPDIRECTSOUND8 GetDSound()
	{
		return m_pDSound;
	}

	static void Create( HWND hWnd )
	{
		new cSoundLayer( hWnd );
	}
};

inline cSoundLayer* Sound()
{
	return cSoundLayer::GetSound();
}

#endif //_SOUNDLAYER_H
