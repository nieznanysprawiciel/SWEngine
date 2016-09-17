//--------------------------------------------------------------------------------------
// Default Camera buffer layout
//--------------------------------------------------------------------------------------


cbuffer ConstantPerFrame : register( b0 )
{
	matrix			ViewMatrix;					///< View matrix.
	matrix			ProjectionMatrix;			///< Projection matrix.
	matrix			ViewProjectionMatrix;	///< Multiplied view-projection matrix.
	float3			CameraPosition;			///< Position of camera in world space.

	float				Time;							///< Current animation time.
}

