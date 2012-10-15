#include "StdAfx.h"

using std::map;

map< CWaveSoundRead*, int > cSound::m_waveMap;

cSound::cSound( TCHAR* filename )
{
	m_pWaveSoundRead = NULL;
	m_pBuffer = NULL;

    // Crea un nuevo wave file
    m_pWaveSoundRead = new CWaveSoundRead();
	m_waveMap[ m_pWaveSoundRead ] = 1;

    // Abre el wave
    if( FAILED( m_pWaveSoundRead->Open( filename ) ) )
    {
        throw cError(L"No se pudo abrir el archivo de audio.");
    }

	Init();
	Fill();
}

cSound::cSound( cSound& in )
{
	m_pWaveSoundRead = in.m_pWaveSoundRead;
	m_waveMap[ m_pWaveSoundRead ]++;
	Init();
	Fill();
}

cSound& cSound::operator=( const cSound &in )
{
	/**
	 * Elimina el objeto antiguo
	 */
	int count = --m_waveMap[ m_pWaveSoundRead ];
	if( !count )
	{
		delete m_pWaveSoundRead;
	}
	SafeRelease( m_pBuffer );

	/**
	 * Clone the incoming one
	 */
	m_pWaveSoundRead = in.m_pWaveSoundRead;
	m_waveMap[ m_pWaveSoundRead ]++;

	Init();
	Fill();
	
	return *this;
}


cSound::~cSound()
{
	int count = m_waveMap[ m_pWaveSoundRead ];
	if( count == 1 )
	{
		delete m_pWaveSoundRead;
	}
	else
	{
		m_waveMap[ m_pWaveSoundRead ] = count - 1;
	}
		
	SafeRelease( m_pBuffer );
}

void cSound::Init()
{
    /**
	 * Setea DirectSound. El tamaño de archivo y el formato puede ser obtenido desde el objeto.
	 * Se setea el flag STATIC, para que el driver no quede restringido al buffer.
	 */
	sAutoZero<DSBUFFERDESC> dsbd;
    dsbd.dwFlags       = DSBCAPS_STATIC;
    dsbd.dwBufferBytes = m_pWaveSoundRead->m_ckIn.cksize;
    dsbd.lpwfxFormat   = m_pWaveSoundRead->m_pwfx;

    HRESULT hr; 

	// Puntero temporal a la antigua inteface de DirectSound
	LPDIRECTSOUNDBUFFER pTempBuffer = 0;

	// Crea el buffer de sonido
	hr = Sound()->GetDSound()->CreateSoundBuffer( &dsbd, &pTempBuffer, NULL );
    if( FAILED( hr ) )
    {
        throw cError(L"CreateSoundBuffer fallo!");
    }

	// Actualiza el buffer de sonido a la version 8
	pTempBuffer->QueryInterface( IID_IDirectSoundBuffer8, (void**)&m_pBuffer );
	if( FAILED( hr ) )
    {
        throw cError(L"Fallo la conversion del SoundBuffer a la version 8!");
    }
	
	// Libera la buffer temporal
	pTempBuffer->Release();

    /**
	 * Setea el tamaño del buffer
	 */
    m_bufferSize = dsbd.dwBufferBytes;
}


void cSound::Restore()
{
    HRESULT hr;

    if( NULL == m_pBuffer )
	{
		return;
	}

    DWORD dwStatus;
    if( FAILED( hr = m_pBuffer->GetStatus( &dwStatus ) ) )
	{
		throw cError( L"No se pudo obtener el estado del buffer" );
	}

    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
		/**
		 * Si llegamos aca es porque la aplicacion acaba de empezar y DirectSound no se inicializo.
		 * Loopeamos hasta poder tener el control.
		 */
        do 
        {
            hr = m_pBuffer->Restore();
            if( hr == DSERR_BUFFERLOST )
                Sleep( 10 );
        }
        while( hr = m_pBuffer->Restore() );

		/**
		 * El buffer fue restaurado.
		 */
        Fill();
    }
}


void cSound::Fill()
{
    HRESULT hr; 
    BYTE*   pbWavData; // Puntero al wav actual
    UINT    cbWavSize; // Tamaño del wav
    void*   pbData  = NULL;
    void*   pbData2 = NULL;
    ULONG   dwLength;
    ULONG   dwLength2;

	/**
	 * Guarda el tamaño del wav
	 */
    UINT nWaveFileSize = m_pWaveSoundRead->m_ckIn.cksize;

    /**
	 * Alloc de memoria para guardar el wav
	 */
    pbWavData = new BYTE[ nWaveFileSize ];
    if( NULL == pbWavData )
	{
	    delete [] pbWavData;
        throw cError(L"Sin memoria para el audio!");
	}

	hr = m_pWaveSoundRead->Read( 
		nWaveFileSize, 
        pbWavData, 
        &cbWavSize );
    if( FAILED( hr ) )           
	{
	    delete [] pbWavData;
        throw cError(L"m_pWaveSoundRead->Read fallo!");
	}

    /**
	 * Resetea el archivo al principio
	 */
    m_pWaveSoundRead->Reset();

    /**
	 * Lock al buffer para poder copiar los datos
	 */
	hr = m_pBuffer->Lock( 
		0, m_bufferSize, &pbData, &dwLength, 
        &pbData2, &dwLength2, 0L );
    if( FAILED( hr ) )
	{
	    delete [] pbWavData;
        throw cError(L"m_pBuffer->Lock fallo!");
	}

    /**
	 * Copia los datos y hace un Unlock
	 */
    memcpy( pbData, pbWavData, m_bufferSize );
    m_pBuffer->Unlock( pbData, m_bufferSize, NULL, 0 );

    /**
	 * Borramos los datos del wav
	 */
    delete [] pbWavData;
}


bool cSound::IsPlaying()
{
    DWORD dwStatus = 0;

    m_pBuffer->GetStatus( &dwStatus );

    if( dwStatus & DSBSTATUS_PLAYING )
        return true;
    else 
        return false;
}


void cSound::Play( bool bLoop )
{
	HRESULT hr;
    if( NULL == m_pBuffer )
        return;

    // Recupera el buffer si se perdio
    Restore();

    // Play buffer 
    DWORD dwLooped = bLoop ? DSBPLAY_LOOPING : 0L;
    if( FAILED( hr = m_pBuffer->Play( 0, 0, dwLooped ) ) )
	{
		throw cError(L"m_pBuffer->Play fallo!");
	}
}
