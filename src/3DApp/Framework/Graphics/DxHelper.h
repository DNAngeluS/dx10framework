#ifndef _D3DHELPER_H
#define _D3DHELPER_H

#include <memory.h>

/**
 * Se encagar de cargar las estructuras de DirectX y Windows
 */
template <class T>
struct sAutoZero : public T
{
	sAutoZero()
	{
		memset( this, 0, sizeof(T) );
		dwSize = sizeof(T);
	}
};


/**
 * Libera de manera correcta la interfaces COM.
 * Si todavia estan activas, las suelta y las invalida.
 */
template <class T>
inline void SafeRelease( T& iface )
{
	if( iface )
	{
		iface->Release();
		iface = NULL;
	}
}

#endif // _D3DHELPER_H