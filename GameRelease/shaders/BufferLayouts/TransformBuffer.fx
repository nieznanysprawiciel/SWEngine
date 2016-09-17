//--------------------------------------------------------------------------------------
// Default Transform buffer layout
//--------------------------------------------------------------------------------------

cbuffer TransformConstants : register( b1 )
{
	matrix			World;
	float3			MeshScale;
}