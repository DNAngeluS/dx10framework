#ifndef _GRAPHICSLAYER_H
#define _GRAPHICSLAYER_H

#include <d3d10.h>
#include <d3dx10.h>

#include <list>
#include <string>
using std::wstring;
using std::list;

#include "DxHelper.h"

//--------------------------------------------------------------------------------------
// Maneja el punto de insercion para escribir texto
//--------------------------------------------------------------------------------------

//class cApplication;

#define MAX_LIGHTS 10	// Se debe asegurar que sea el mismo que en los shaders

class cGraphicsLayer
{
public:

	class cLight
	{
	public:
		cLight()
		{
		}

		D3DXCOLOR m_vColor;
		D3DXVECTOR3 m_vDirection;
	};

protected:

	HWND					m_hWnd;			// Handdle the la ventana

	ID3D10Device*			m_pDevice;		// Interface de IDirect3DDevice10

	ID3D10Texture2D*		m_pBackBuffer;	// Puntero al back buffer

	ID3D10Texture2D*		m_pDepthStencilBuffer;			// Puntero al depth/stencial buffer
	ID3D10DepthStencilState*m_pDepthStencilState;			// Estado del depth/stencil buffer
	ID3D10DepthStencilView* m_pDepthStencilView;			// Vista del  depth/stencil buffer

	ID3D10RenderTargetView* m_pRenderTargetView; // Puntero al render target view

	IDXGISwapChain*         m_pSwapChain;	// Puntero a la cadena de swap

	RECT	m_rcScreenRect;					// Dimensiones de la ventana

	cGraphicsLayer(HWND hWnd);				// Constructor
	static cGraphicsLayer*	m_pGlobalGLayer;// Puntero al Objeto Global GFX

	ID3DX10Font*			m_pFont;		// La fuente para los textos
	ID3DX10Sprite*			m_pFontSprite;	// Sprites de la fuente

	ID3D10InfoQueue*		m_pMessageQueue;// Queue para los mensajes de D3D

	static const UINT		m_uiMAX_CHARS_PER_FRAME = 512;

	ID3D10Effect*			m_pDefaultEffect;				// Default rendering shader
	ID3D10EffectTechnique*	m_pDefaultTechnique;			// Default rendering technique
	ID3D10InputLayout*		m_pDefaultInputLayout;			// Default vertex layout
	ID3D10RasterizerState*	m_pDefaultRasterizerState;		// Default raterizer state

	ID3D10EffectMatrixVariable* m_pmtxWorldVar;		// Puntero a la variable de la matriz world en el shader
	ID3D10EffectMatrixVariable* m_pmtxViewVar;		// Puntero a la variable de la matriz view en el shader
	ID3D10EffectMatrixVariable* m_pmtxProjVar;		// Puntero a la variable de la matriz de projeccion en el shader

	ID3D10EffectVectorVariable* m_pLightDirVar;		// Puntero al array de direccion de luz en el shader
	ID3D10EffectVectorVariable* m_pLightColorVar;	// Puntero al array de direccion de color en el shader
	ID3D10EffectVectorVariable* m_pNumLightsVar;	// Puntero a la variable que tiene el numero de luces

	ID3D10EffectVectorVariable* m_pTextureDataVar;	// Pointer to the variable holding the number of lights 
	ID3D10EffectShaderResourceVariable* m_pTexturesVar;	// Pointer to the texture variable

	D3DXMATRIX m_mtxWorld;					// Matriz World
	D3DXMATRIX m_mtxView;					// Matriz View
	D3DXMATRIX m_mtxProj;					// Matriz de Projeccion

	cLight m_aLights[MAX_LIGHTS];			// Array de luces
	int m_iNumLights;						// Numero de luces activas

public:

	D3DXVECTOR3 m_vCamPos;					// Vector Posicion de la Camara
    D3DXVECTOR3 m_vCamAt;					// Vector LookAt de la Camara
    D3DXVECTOR3 m_vCamUp;					// Vector Up de la Camara

	// Agrega luces a la array si hay espacio.
	void AddLight(const D3DXCOLOR& vColor, const D3DXVECTOR3 vDir)
	{
		if(m_iNumLights < MAX_LIGHTS)
		{		
			m_aLights[m_iNumLights].m_vColor = vColor;
			m_aLights[m_iNumLights].m_vDirection = vDir;
			m_iNumLights++;
		}
	}

	// Manda la luces al shader
	void UpdateLights();

	// Actualiza las matrices de transformacion
	void SetWorldMtx(const D3DXMATRIX& mtxWorld){m_mtxWorld = mtxWorld;}
	void SetViewMtx(const D3DXMATRIX& mtxView){m_mtxView = mtxView;}
	D3DXMATRIX GetViewMtx(){return m_mtxView;}

	void SetProjMtx(const D3DXMATRIX& mtxProj){m_mtxProj = mtxProj;}

	// Getters de las matrices de transformacion
	const D3DXMATRIX& GetWorldMtx() const {return m_mtxWorld;}
	const D3DXMATRIX& GetViewMtx() const {return m_mtxView;}
	const D3DXMATRIX& GetProjdMtx() const {return m_mtxProj;}

	void SetTexture(int iIdx, ID3D10ShaderResourceView* pTexShaderView);
	void SetNumActiveTexture(int texCount, int iCurrent);

	// Cambia los vectores de la camara
	void UpdateCam();

	// Envia las matrices de transformacion al shader
	void UpdateMatrices();

	void DestroyAll();
	~cGraphicsLayer();
	
	/**
	 * <summary> Inicializaciones. </summary>
	 */
	void InitD3D(int width, int height);
	void CreateDeviceAndSwapChain();
	void CreateViewport();
	void CreateDebugText();
	/**
	 * <summary> Crea el Depth/Stencil Buffer. </summary>
	 */
	void CreateDepthStencilBuffer();
	/**
	 * <summary> Setea y Crea el Rasterizer. 
	 *	Por defecto se configura en Clockwise, Cullmode none, Fillmode full.</summary>
	 */
	void CreateRasterizerState();
	/**
	 * <summary> Crea el Shader.
	 *	Por defecto solo realiza la transformacion y devuelve el color de textura.</summary>
	 */
	void CreateDefaultShader();

	/**
	 * <sumary>
	 * Escribe texto al back buffer.
	 * Es mas rapido que usar GDI
	 * </sumary>
	 */
	void DrawTextString(int x, int y, D3DXCOLOR color, const TCHAR* str);

	ID3D10EffectTechnique* GetDefaultTechnique() const {return m_pDefaultTechnique;}
	ID3D10Effect* GetDefaultEffect() const {return m_pDefaultEffect;}

	HWND GetHWnd(){return m_hWnd;}

	void DumpMessages();
	//==========--------------------------  Getters

	/** 
	* <summary>Puntero al Device de D3</summary>
	*/
	ID3D10Device* GetDevice()
	{
		return m_pDevice;
	}

	/** 
	* <summary>Puntero al back buffer</summary>
	*/
	ID3D10Texture2D* GetBackBuffer()
	{
		return m_pBackBuffer;
	}

	/** 
	* <summary>Ancho de pantala</summary>
	*/
	int Width() const
	{
		return m_rcScreenRect.right;
	}

	/** 
	* <summary>Alto de pantala</summary>
	*/
	int Height() const
	{
		return m_rcScreenRect.bottom;
	}

	/** 
	* <summary>Presenta el back buffer</summary>
	*/
	void Present();

	/** 
	* <summary>Limpia el back bugger</summary>
	* <param name="colClear">Color para pintar la pantalla</param>
	*/
	void Clear(const float (&colClear)[4]);

	// Limpia el depth/stencil buffer
	void ClearDepthStencil(const float fDepth, const UINT8 uiStencil);

	/** 
	* <summary>Puntero al Objeto GFX principal</summary>
	*/
	static cGraphicsLayer* GetGraphics()
	{
		return m_pGlobalGLayer;
	}

	/** 
	* <summary>Inicializa el objeto</summary>
	*/
	static void Create(
		HWND hWnd, // Handle a la ventana
		short width, short height); // guid del device
};

inline cGraphicsLayer* Graphics()
{
	return cGraphicsLayer::GetGraphics();
}


#endif //_GRAPHICSLAYER_H
