//--------------------------------------------------------------------------------------
// Light computing functions
//--------------------------------------------------------------------------------------

// Buffers 
// @todo Try to use #include directive instead of copying declarations.

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

cbuffer ConstantPerFrame : register( b0 )
{
	matrix			ViewMatrix;					///< View matrix.
	matrix			ProjectionMatrix;			///< Projection matrix.
	matrix			ViewProjectionMatrix;	///< Multiplied view-projection matrix.
	float3			CameraPosition;			///< Position of camera in world space.

	float				Time;							///< Current animation time.
}

cbuffer LightConstants : register( b1 )
{
	float3		AmbientColor;
	uint			NumLights;
	LightParams	Lights[ MAX_NUM_LIGHTS ];
}

cbuffer PhongMaterialBuffer : register( b2 )
{
	float4 	Diffuse;
	float3 	Ambient;
	float3 	Specular;
	float3 	Emissive;
	float 	Power;
}

// float3 WorldPosition;		// From Vertex Shader
// float3 Normal;		// From Vertex Shader

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------

struct PhongResult
{
	float4	Diffuse;
	float3	Specular;
}

//--------------------------------------------------------------------------------------
//
float		ComputeLightIntesity( uint lightIdx, float lightDistance )
{
	float intensity = Lights[ lightIdx ].Intensity;
	float attenuation = 	Lights[ lightIdx ].ConstantAttenuation
								+ Lights[ lightIdx ].LinearAttenuation * lightDistance
								+ Lights[ lightIdx ].QuadraticAttenuation * lightDistance * lightDistance;
	
	return intensity / attenuation;
}

//--------------------------------------------------------------------------------------
//
float4	ComputeDiffuse( float3 normal, float3 lightDir )
{
	float lambertFactor = dot( normal, -lightDir );
	return Lights[ lightIdx ].Color * lambertFactor;
}

//--------------------------------------------------------------------------------------
//
float3	ComputeSpecular( float3 normal, float3 lightDir, float3 viewDir, uint lightIdx )
{
	float3 reflectedLightDir = reflect( normal, -lightDir );
	float specularCoeff = dot( reflectedLightDir, viewDir );
	
	return Lights[ lightIdx ].Color * pow( specularCoeff, Power );
}

//--------------------------------------------------------------------------------------
//
PhongResult		ComputePhongColorPointLight( float3 normal, float3 viewDir, float3 lightDir, float lightDistance, uint lightIdx )
{	
	float4 colorDiffuse = ComputeDiffuse( normal, lightDir );
	float3 colorSpecular = ComputeSpecular( normal, lightDir, viewDir, lightIdx );
	float lightIntens = ComputeLightIntesity( lightIdx, lightDistance );
	
	PhongResult resultColor;
	resultColor.Diffuse = colorDiffuse / lightIntens;
	resultColor.Specular = colorSpecular / lightIntens;
	
	return resultColor;
}

float 			ComputeSpotAttenuation( float angle, float3 lightDir, float3 spotDirection )
{
    float minCos = cos( angle );
    float maxCos = ( minCos + 1.0f ) / 2.0f;
    float cosAngle = dot( lightDir, spotDirection );
    return smoothstep( minCos, maxCos, cosAngle ); 
}

//--------------------------------------------------------------------------------------
//
PhongResult		ComputePhongSpotLight( float3 normal, float3 viewDir, float3 lightDir, float lightDistance, uint lightIdx )
{
	float spotAttenuation = ComputeSpotAttenuation( Lights[ lightIdx ].SpotAngle, lightDir, Lights[ LightIdx ].Direction );
	
	if( spotAttenuation > 0 )
	{
		float4 colorDiffuse = ComputeDiffuse( normal, lightDir );
		float3 colorSpecular = ComputeSpecular( normal, lightDir, viewDir, lightIdx );
		float lightIntens = ComputeLightIntesity( lightIdx, lightDistance );
		lightIntens = lightIntens * spotAttenuation;
		
		PhongResult resultColor;
		resultColor.Diffuse = colorDiffuse / lightIntens;
		resultColor.Specular = colorSpecular / lightIntens;
	}
	
	return resultColor;
}

//--------------------------------------------------------------------------------------
//
PhongResult		ComputePhongDirectionalLight( float3 normal, float3 viewDir, uint lightIdx )
{
	PhongResult resultColor;

	resultColor.Diffuse = ComputeDiffuse( normal, Lights[ lightIdx ].Direction );
	resultColor.Specular = ComputeSpecular( normal, Lights[ lightIdx ].Direction, viewDir, lightIdx );
	
	return resultColor;
}

//--------------------------------------------------------------------------------------
//
PhongResult		ComputeLightPhong( float3 normal, float3 viewDir )
{
	PhongResult resultColor;
	
	for( int i = 0; i < NumLights; ++i )
	{
		PhongResult phongColor;
	
		if( Lights[ i ].LightType == DIRECTIONAL_LIGHT )
		{
			phongColor = ComputePhongDirectionalLight( normal, viewDir, i );
		}
		else
		{
			// Computing lights in world space.
			float3 lightDir = normalize( WorldPosition - Lights[ lightIdx ].Position );
			float lightDistance = length( lightDir );
			lightDir = lightDir / lightDistance;
		
			if( Lights[ i ].LightType == POINT_LIGHT )
			{
				phongColor = ComputePhongColorPointLight( normal, viewDir, lightDir, lightDistance, i );
			}
			else if( Lights[ i ].LightType == SPOT_LIGHT )
			{
				phongColor = ComputePhongSpotLight( normal, viewDir, lightDir, lightDistance, i );
			}
		}
		
		resultColor.Diffuse += phongColor.Diffuse;
		resultColor.Specular += phongColor.Specular;
	}
	
	return resultColor;
}

//--------------------------------------------------------------------------------------
// This function computes final color. You can use ComputeLightPhong instead, if you want
// apply texturing or some other tricks.
//
// Remember before calling this function.
//	float3 normal = normalize( Normal );
// float3 viewDir = normalize( WorldPosition - CameraPosition );
float		ComputePhongLightResultColor( float3 normal, float3 viewDir )
{
	float4 resultColor;
	resultColor.w = 0;
	resultColor.xyz = AmbientColor * Ambient;
	resultColor.xyz += Emissive;

	PhongResult phongResult = ComputeLightPhong( normal, view );
	
	resultColor += phongResult.Diffuse * Diffuse;
	resultColor += phongResult.Specular * Specular;
	
	return resultColor;
}




