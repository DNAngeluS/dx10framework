matrix g_mtxWorld;
matrix g_mtxView;
matrix g_mtxProj;

#define MAX_LIGHTS 10	// Asegurar que este seteado el mismo valor en el codigo

int4  g_viLightStatus;
float4 g_vLightColors[MAX_LIGHTS];
float4 g_vLightDirections[MAX_LIGHTS];

Texture2D g_Texture;
////////////////////////////////////////////////////////////////////////////
// Default Vertex Shader
struct VS_INPUT
{
    float3 vPosition	: POSITION;
    float3 vNormal		: NORMAL;
    float4 vColor		: COLOR0;
    float2 vTexCoords	: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 vPosition	: SV_POSITION;
	float3 vNormal		: NORMAL;
    float4 vColor		: COLOR0;
    float2 vTexCoords	: TEXCOORD;
};

VS_OUTPUT DefaultVS(VS_INPUT dataIn) 
{
    VS_OUTPUT result;
    
    float4 vPos = float4(dataIn.vPosition, 1.0f);
    
    float4 vFinalPos = mul(vPos, g_mtxWorld);
    vFinalPos = mul(vFinalPos, g_mtxView);
    vFinalPos = mul(vFinalPos, g_mtxProj);
    
    result.vPosition = vFinalPos;
    result.vNormal = dataIn.vNormal;
    result.vColor = dataIn.vColor;    
    result.vTexCoords = dataIn.vTexCoords;   
    
    return result;
}

////////////////////////////////////////////////////////////////////////////
// Default Pixel Shader 

SamplerState SamplerStateWrap
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 DefaultPS(VS_OUTPUT dataIn) : SV_Target
{
	//return float4(1,1,1,1);
	float4 texColor = g_Texture.Sample(SamplerStateWrap, dataIn.vTexCoords);

	return texColor;
	/*
	if(g_viLightStatus.x > 0)
	{
		float4 finalColor = 0;        
		for(int iCurLight = 0 ; iCurLight < g_viLightStatus.x ; iCurLight++)
		{
			finalColor += saturate(dot(g_vLightDirections[iCurLight], dataIn.vNormal) * g_vLightColors[iCurLight]);
		}
		return finalColor * texColor;
	}
	else
	{        
		return dataIn.vColor * texColor;
	}
	*/
}

////////////////////////////////////////////////////////////////////////////
// Default Technique
technique10 DefaultTechnique
{
    pass Pass0
    {

		SetGeometryShader(NULL);
        SetVertexShader(CompileShader(vs_4_0, DefaultVS()));
        SetPixelShader(CompileShader(ps_4_0, DefaultPS()));
    }
    
    
}