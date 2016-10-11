#pragma once
/**
@file FileName
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "ResourceObject.h"

#include <DirectXMath.h>


/**@brief Blending operation.
@ingroup PipelineState*/
enum class BlendOperation : uint8
{
	Add,
	Subtract,
	ReverseSubtract,
	Min,
	Max
};


/**@brief Blend factor for blending equation.
@ingroup PipelineState*/
enum class BlendFactor : uint8
{
	Zero,
	One,
	SrcColor,
	DstColor,
	SrcAlpha,
	DstAlpha,
	BlendFactor,			///< Custom blend factor. @see BlendingInfo.
	InverseSrcColor,		///< One minus SrcColor
	InverseDstColor,		///< One minus DstColor
	InverseSrcAlpha,		///< One minus SrcAlpha
	InverseDstAlpha,		///< One minus DstAlpha
	InverseBlendFactor		///< One minus custom blend factor. @see BlendingInfo.
};


/**@brief Initialization of BlendingState.

@ingroup PipelineState*/
struct BlendingInfo
{
	DirectX::XMFLOAT4	CustomBlendFactor;		///< Custom blend factors.
	bool				EnableBlending;
	BlendOperation		ColorOperation;
	BlendOperation		AlphaOperation;
	BlendFactor			SrcColorBlend;
	BlendFactor			DstColorBlend;
	BlendFactor			SrcAlphaBlend;
	BlendFactor			DstAlphaBlend;
};


/**@brief 

@ingroup PipelineState*/
class BlendingState : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
private:
protected:

	explicit		BlendingState()
		:	ResourceObject( 0 )
	{}

	virtual	~BlendingState() = default;

public:

	virtual const BlendingInfo&		GetDescriptor	() = 0;
};

