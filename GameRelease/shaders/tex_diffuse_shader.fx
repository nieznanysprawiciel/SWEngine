//--------------------------------------------------------------------------------------
// Domyœlne shadery dla meshy z tekstur¹ w kanale Diffuse
//
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D texDiffuse : register( t0 );
SamplerState default_sampler : register( s0 );

cbuffer ConstantPerFrame : register( b0 )
{
	matrix			ViewMatrix;					///< View matrix.
	matrix			ProjectionMatrix;			///< Projection matrix.
	matrix			ViewProjectionMatrix;	///< Multiplied view-projection matrix.
	float3			CameraPosition;			///< Position of camera in world space.

	float				Time;							///< Current animation time.
}


cbuffer ConstantPerMesh : register( b1 )
{
	matrix World;
	float4 MeshScale;
	float4 Diffuse;
	float3 Ambient;
	float3 Specular;
	float3 Emissive;
	float Power;
}

cbuffer ConstantPerFrame : register( b2 )
{
	matrix View;
	matrix Projection;
	float3 LightDir[2];
	float3 LightColor[2];
	float3 AmbientLight;
	float time;
	float time_lag;
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
//--------------------------------------------------------------------------------------
PS_INPUT vertex_shader( VS_INPUT input )
{
   PS_INPUT output = (PS_INPUT)0;
	output.Pos = input.Pos * MeshScale;
	output.Pos = mul( output.Pos, World );
   output.Pos = mul( output.Pos, ViewProjectionMatrix );
   output.Norm = mul( input.Norm, World );
	output.Tex = input.Tex;
    
   return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 pixel_shader( PS_INPUT input) : SV_Target
{
    float4 finalColor;
	finalColor.xyz = Ambient*AmbientLight;
    float dot_product = 0;
	float3 tex_sample = texDiffuse.Sample( default_sampler, input.Tex );
	
	
    //do NdotL lighting for 2 lights
    for(int i=0; i<1; i++)
    {
		dot_product = dot( (float3)LightDir[i], input.Norm);
        finalColor.xyz += dot_product * LightColor[i];
    }
	finalColor.xyz *= tex_sample * (float3)Diffuse;
	finalColor.w = 1.0;
	
    return finalColor;
}