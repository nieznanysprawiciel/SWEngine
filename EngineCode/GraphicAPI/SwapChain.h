#pragma once

#include "GraphicAPI/MeshResources.h"


class SwapChain
{
	RTTR_ENABLE()
protected:

	RenderTargetObject*		m_renderTarget;

protected:
	SwapChain( RenderTargetObject* windowRT )
		:	m_renderTarget( windowRT )
	{
		assert( windowRT );
		m_renderTarget->AddAssetReference();
	}

public:
	virtual ~SwapChain()
	{
		m_renderTarget->DeleteAssetReference();
		m_renderTarget = nullptr;
	}


	virtual void			Present			( int syncInterval )					= 0;
	virtual void			Resize			( uint16 newWidth, uint16 newHeight )	= 0;


	RenderTargetObject*		GetRenderTarget	()		{ return m_renderTarget; }
};

