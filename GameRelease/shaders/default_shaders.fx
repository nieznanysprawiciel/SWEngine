//--------------------------------------------------------------------------------------
// Domyślne shadery obsługujące kanały Ambient i Diffuse raz światła kierunkowe.
// Nie używają tekstur.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantPerFrame : register( b0 )
{
	matrix View;
	matrix Projection;
	float3 LightDir[2];
	float3 LightColor[2];
	float3 AmbientLight;
	float time;
	float time_lag;
}

cbuffer ConstantPerMesh : register( b1 )
{
	matrix World;
	float4 mesh_scale;
	float4 Diffuse;
	float3 Ambient;
	float3 Specular;
	float3 Emissive;
	float Power;
}


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
// Ten shader jest taki jaki domyślny dla aplikacji więc nie jest wczytywany
//--------------------------------------------------------------------------------------
PS_INPUT vertex_shader( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	output.Pos = input.Pos * mesh_scale;
    output.Pos = mul( output.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Norm = mul( input.Norm, World );
    output.Tex = input.Tex;
	
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float3 pixel_shader( PS_INPUT input) : SV_Target
{
    float3 finalColor = Ambient*AmbientLight;
    float dot_product = 0;
	
    //do NdotL lighting for 2 lights
    for(int i=0; i<1; i++)
    {
		dot_product = dot( (float3)LightDir[i], input.Norm);
        finalColor += dot_product * LightColor[i];
    }
	finalColor *= (float3)Diffuse;

    return finalColor;
}

/*
//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid( PS_INPUT input) : SV_Target
{
    return vOutputColor;
}
*/