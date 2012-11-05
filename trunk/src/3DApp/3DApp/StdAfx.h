// stdafx.h : Incluye archivos para sistemas estandar.

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define DIRECTINPUT_VERSION 0x0800
#define WIN32_LEAN_AND_MEAN		// Excluye del Header compentes raramente usados

#include <windows.h>

// Archivos Header Locales
#define D3D_OVERLOADS
#include <d3d10.h>
#include <D3DX10.h>
#include <D3DX10Math.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#include <string>

#include "..\Framework\Graphics\DxHelper.h"

#include "..\Math3D\mathD3D.h"
#include "..\Math3D\Point3.h"
#include "..\Math3D\Matrix4.h"
#include "..\Math3D\Color3.h"

#include "..\Framework\Errors.h"
#include "..\Framework\Types.h"
#include "..\Framework\Globals.h"
#include "..\Framework\File.h"

#include "..\Framework\Application.h"
#include "..\Framework\Graphics\GraphicsLayer.h"
#include "..\Framework\Window\Window.h"
#include "..\Framework\Graphics\Model.h"

#include "..\Framework\Input\InputLayer.h"
#include "..\Framework\Sound\Sound.h"
#include "..\Framework\Sound\SoundLayer.h"
#include "..\Framework\DXUtils\DXUtils.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ inserta declaraciones adicionales luego de la linea anterior.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
