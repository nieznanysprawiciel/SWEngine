#pragma once
/**
@file RasterizerStateObject.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "ResourceObject.h"


/**@defgroup PipelineState
@ingroup Resources
*/


/**@brief Polygon culling mode.*/
enum class CullMode : uint8
{
	Front,
	Back,
	None
};

/**@brief Polygon fill mode.*/
enum class FillMode : uint8
{
	Solid,
	Wireframe
};

/**@brief 
@ingroup PipelineState*/
struct RasterizerStateInfo
{
	CullMode		CullMode;
	FillMode		FillMode;
	int				DepthBias;
	bool			IsClockwise;
	bool			EnableScissor;
	bool			EnableZClipping;			///< Clipping of far plane.
	bool			ConservativeRaserizer;		///< Since in DirectX 11.2
	bool			EnableDepthTest;
	bool			EnableStencilTest;


	RasterizerStateInfo()
		:	CullMode( CullMode::Back )
		,	FillMode( FillMode::Solid )
		,	DepthBias( 0 )
		,	IsClockwise( true )
		,	EnableScissor( false )
		,	EnableZClipping( true )
		,	ConservativeRaserizer( false )
		,	EnableDepthTest( true )
		,	EnableStencilTest( false )
	{}
};


/**@brief Rasterizer state and depth stencil state.

@ingroup PipelineState*/
class RasterizerState : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );

private:
protected:
	
	explicit		RasterizerState()
		:	ResourceObject( 0 )
	{}

	virtual			~RasterizerState() = default;

public:

	virtual const RasterizerStateInfo&		GetDescriptor	() = 0;
};

