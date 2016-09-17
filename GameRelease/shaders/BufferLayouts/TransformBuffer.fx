//--------------------------------------------------------------------------------------
// Default Transform buffer layout
//--------------------------------------------------------------------------------------

cbuffer ConstantPerFrame : register( b0 )
{
	matrix			World;
	float3			MeshScale;
}