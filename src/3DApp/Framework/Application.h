#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <windows.h>

#include "Errors.h"
#include "Types.h"
#include "Graphics\GraphicsLayer.h"
#include "Input\InputLayer.h"

class cWindow;

class cApplication 
{
protected:

	//==========--------------------------  Variables de Clase
	
	std::wstring	m_title;		// Nombre de la ventana

	int		m_width; // Ancho de la ventana
	int		m_height; // Alto de la ventana
	int		m_bpp; // Profundidad del color

	bool	m_bActive; // Estado activo de la app

	UINT	m_glFlags; // Flags para GraphicsLayer

	static cApplication*	m_pGlobalApp;

	virtual void InitPrimaryWindow();
	virtual void InitGraphics();
	virtual void InitInput();
	virtual void InitSound();
	virtual void InitExtraSubsystems();

public:

	std::wstring	m_dRoot;		// Directorio de Raiz
	std::wstring	m_dMedia;		// Directorio de Media Framework
	std::wstring	m_dAppMedia;	// Directorio Media Aplicacion
	std::wstring	m_dAppRoot;		// Directorio Raiz Apliacion

	cApplication();
	virtual ~cApplication();

	/**
	*/
	std::wstring MediaDir();

	/**
	 * Inicializacion de la App
	 */
	virtual void Init(); 

	/**
	 * Corre la App
	 */
	virtual void Run();

	/**
	 * Update de cada cuadro
	 */
	virtual void Update( float timeDelta );

	/**
	 * Renderiza cada cuadro
	 * timeDelta == amount of time (in seconds) since last call
	 */
	virtual void DoFrame( float timeDelta );
	virtual void DoIdleFrame( float timeDelta );


	/**
	 * SceneInit se llama en Init. el usuario no debe settear D3D
	 * u obtener mensajes de error.
	 */
	virtual void SceneInit();
	virtual void SceneEnd();

	void Pause() { m_bActive = false; }
	void UnPause() { m_bActive = true; }

	static cApplication* GetApplication() { return m_pGlobalApp; }

	static void KillApplication() 
	{
		delete m_pGlobalApp;
		m_pGlobalApp = NULL;
	}
};

inline cApplication* Application()
{
	return cApplication::GetApplication();
}

HINSTANCE AppInstance();

/**
 * Funcion implementada por cualquier cliente del framework
 */
cApplication*	CreateApplication(); // Devuelve un puntero valido a un obejto aplicacion




#endif // _APPLICATION_H
