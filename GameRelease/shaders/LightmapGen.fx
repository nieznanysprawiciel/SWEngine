//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float2 Tex : TEXCOORD;
	float3 Color : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Color : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT vertex_shader( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)1;
	output.Pos.z = 0.6;
	output.Pos.xy = (input.Tex - float2( 0.5, 0.5 ) ) * float2( 2.0, 2.0 );
	output.Color = input.Color;
    
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 pixel_shader( PS_INPUT input) : SV_Target
{
	float4 finalColor;
	finalColor.xyz = input.Color;
	finalColor.w = 1.0;
    return finalColor;
}