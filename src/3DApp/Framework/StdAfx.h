// stdafx.h : Incluye archivos para sistemas estandar.

#if !defined(AFX_STDAFX_H__9C89AC16_48BC_11D3_B2BB_00C04F685D9A__INCLUDED_)
#define AFX_STDAFX_H__9C89AC16_48BC_11D3_B2BB_00C04F685D9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define DIRECTINPUT_VERSION 0x0800
#define WIN32_LEAN_AND_MEAN		// Excluye del Header compentes raramente usados
#define STRICT

// Deshabilita el warning para caracteres de 255
//#pragma warning( disable : 4786 )

// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <assert.h>

// Local Header Files
#include "Globals.h"
#include "Errors.h"
#include "Types.h"
#include "File.h"

// Framework files
#include "Application.h"
#include "Window.h"
#include "GraphicsLayer.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "InputLayer.h"
#include "SoundLayer.h"
#include "wavread.h"
#include "Sound.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ inserta declaraciones adicionales luego de la linea anterior.

#endif // !defined(AFX_STDAFX_H__9C89AC16_48BC_11D3_B2BB_00C04F685D9A__INCLUDED_)