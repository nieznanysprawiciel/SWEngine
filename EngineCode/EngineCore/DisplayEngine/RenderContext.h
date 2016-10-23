#pragma once
/**
@file RenderContext.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

class DisplayEngine;
class LightModule;

/**@brief Context which will be pass to RenderPass::Render function.*/
struct RenderContext
{
	DisplayEngine*		DisplayEngine;
	LightModule*		LightModule;
	// Culling

	std::vector< StaticActor* >&		Meshes;
	std::vector< DirectX::XMFLOAT4X4 >&	Transforms;

	BufferObject*		CameraBuffer;
	BufferObject*		LightBuffer;
	BufferObject*		TransformBuffer;
	BufferObject*		MaterialConstants;		///< @Deprecated Use buffers from MaterialAsset.

	/// @todo In future replace with one time variable.
	float				TimeInterval;
	float				TimeLag;



	RenderContext( std::vector< StaticActor* >& meshes, std::vector< DirectX::XMFLOAT4X4 >& transforms )
		:	Meshes( meshes )
		,	Transforms( transforms )
	{}
};

