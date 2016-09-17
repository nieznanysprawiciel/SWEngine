//--------------------------------------------------------------------------------------
// Default Phong materials buffer layout
//--------------------------------------------------------------------------------------



cbuffer PhongMaterialBuffer : register( b2 )
{
	float4 Diffuse;
	float3 Ambient;
	float3 Specular;
	float3 Emissive;
	float Power;
}
