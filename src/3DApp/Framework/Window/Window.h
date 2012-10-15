#ifndef _WINDOW_H
#define _WINDOW_H

#include <windows.h>

#include <list>
#include <string>

#include "..\Errors.h"


/**
 * cWindow 
 * Encapsula la ventana de Windows
 */
class cWindow  
{
protected:

	// Ancho y Alto de la ventana del cliente
	int m_Width, m_Height;

	// Handle de la ventana
	HWND m_hWnd;

	// Nombre de la ventana
	std::wstring m_name;

	// Estado de la ventana activo?
	bool m_bActive;

	static cWindow* m_pGlobalWindow;

public:
	cWindow( 
		int width, 
		int height, 
		const TCHAR* name = (TCHAR*)"Ventana de Framework" );
	~cWindow();

	virtual LRESULT WndProc(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam );

	virtual void RegisterClass( WNDCLASSEX* pWc = NULL );
	virtual void InitInstance();

	HWND GetHWnd()
	{
		return m_hWnd;
	}

	bool IsActive()
	{ 
		return m_bActive; 
	}

	// True -> Hay mensajes para procesar
	bool HasMessages();

	// res_AllGood -> Continua
	// res_False -> WM_QUIT.  Para la App.
	eResult Pump();

	static cWindow* GetMainWindow()
	{ 
		return m_pGlobalWindow; 
	}

};

inline cWindow* MainWindow()
{ 
	return cWindow::GetMainWindow(); 
}


#endif //_WINDOW_H
