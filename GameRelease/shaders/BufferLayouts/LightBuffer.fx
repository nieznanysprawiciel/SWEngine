//--------------------------------------------------------------------------------------
// Default Light buffer layout
//--------------------------------------------------------------------------------------

// Declaration should be equal to declaration of ENGINE_MAX_LIGHTS in engine
#define MAX_NUM_LIGHTS 2

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2

struct LightParams
{
	float3		Color;
	float			ClampRadius;
	float3		Position;
	float			SpotAngle;
	float3		Direction;
	uint			LightType;
	float			ConstantAttenuation;
	float			LinearAttenuation;
	float			QuadraticAttenuation;
	float			Intensity;
};


cbuffer LightConstants : register( b1 )
{
	float3		AmbientColor;
	uint			NumLights;
	LightParams	Lights[ MAX_NUM_LIGHTS ];
}

