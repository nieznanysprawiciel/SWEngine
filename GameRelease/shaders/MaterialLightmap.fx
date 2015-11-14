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


struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

Texture2D texLightmap : register( t0 );
SamplerState defaultSampler : register( s0 );


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 pixel_shader( PS_INPUT input) : SV_Target
{
    float4 finalColor;
	finalColor.xyz = Ambient*AmbientLight;
	
	float3 lightmapSample = texLightmap.Sample( defaultSampler, input.Tex );
	
	finalColor.xyz += lightmapSample * (float3)Diffuse;
	finalColor.w = 1.0;

    return finalColor;
}
