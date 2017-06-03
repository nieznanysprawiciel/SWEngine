// Shadery dla kopuły nieba używającej modelu Hoseka i Wilkiego

// Bufory i tak zostaną przekazane


cbuffer CameraConstants : register( b0 )
{
	matrix			ViewMatrix;					///< View matrix.
	matrix			ProjectionMatrix;			///< Projection matrix.
	matrix			ViewProjectionMatrix;	///< Multiplied view-projection matrix.
	float3			CameraPosition;			///< Position of camera in world space.

	float				Time;							///< Current animation time.
}

// Vertex shader only
cbuffer TransformConstants : register( b1 )
{
	matrix			World;
	float4			MeshScale;
}

// Pixel shader only
cbuffer PhongMaterialBuffer : register( b2 )
{
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
    float3 Color : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Color : COLOR;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
// Potrzebujemy zmodyfikowanej macierzy widoku, bo nie obchodzi nas translacja kamery,
// a jedynie jej obrót. Żeby nie zmieniać macierzy widoku, to podamy ją w macierzy świata,
// która nie jest nam potrzebna, bo i tak wolimy nie ruszać kopuły.
//--------------------------------------------------------------------------------------
PS_INPUT vertex_shader( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = input.Pos * MeshScale;
	output.Pos = mul( output.Pos, World );
	output.Pos = mul( output.Pos, ProjectionMatrix );
	//output.Pos.z = 1.0;//output.Pos.w;		// After division by w this should be 1.0f.
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