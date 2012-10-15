#include "StdAfx.h"

cSoundLayer* cSoundLayer::m_pGlobalSLayer = NULL;

cSoundLayer::cSoundLayer( HWND hWnd )
{
	m_pDSound = NULL;
	m_pPrimary = NULL;

	if( m_pGlobalSLayer )
	{
		throw cError(L"cSoundLayer ya fue inicializada" );
	}
	m_pGlobalSLayer = this;

    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    // Crea una interface de IDirectSound usando el adaptador primario de sonido
	hr = DirectSoundCreate8( NULL, &m_pDSound, NULL );
    if( FAILED( hr ) )
	{
        throw cError(L"DirectSoundCreate fallo!" );
	}

    // Setea el nivel de cooperacion a DSSCL_PRIORITY
	hr = m_pDSound->SetCooperativeLevel( hWnd, DSSCL_PRIORITY );
    if( FAILED( hr ) )
	{
        throw cError(L"SetCooperativeLevel (DS) fallo!" );
	}

    // Obtiene el buffer primario
	sAutoZero<DSBUFFERDESC> dsbd;
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
       
	hr = m_pDSound->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL );
    if( FAILED( hr ) )
	{
        throw cError(L"CreateSoundBuffer (DS) fallo!" );
	}

    // Setea el formato del buffer a 22kHz y 16-bit.
    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = 2; 
    wfx.nSamplesPerSec  = 4410; 
    wfx.wBitsPerSample  = 32; 
    wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    if( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) )
	{
       throw cError(L"SetFormat (DS) fallo!" );
	}

    SafeRelease( pDSBPrimary );
}

cSoundLayer::~cSoundLayer()
{
	SafeRelease( m_pPrimary );
	SafeRelease( m_pDSound );
	m_pGlobalSLayer = NULL;
}
