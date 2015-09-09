#pragma once
/**
@file IGraphicAPIInitializer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja interfejsu IGraphicAPIInitializer.
*/

#include "IRenderer.h"
#include "Common/types_definitions.h"

#include <string>

struct GraphicAPIInitData
{
	uint16 windowWidth;
	uint16 windowHeight;
	uint32 windowHandle;
	bool singleThreaded;
	bool fullScreen;

	void DefaultSettings()
	{
		windowWidth = 1024;
		windowHeight = 768;
		singleThreaded = true;
		fullScreen = false;
	}
};

/**@brief Interfejs klasy do inicjowania API graficznego.*/
class IGraphicAPIInitializer
{
private:
protected:
public:
	virtual ~IGraphicAPIInitializer() = 0;

	virtual IRenderer*		CreateRenderer			( RendererUsage usage ) = 0;
	virtual bool			InitAPI					( GraphicAPIInitData& initData ) = 0;
	virtual void			ReleaseAPI				() = 0;

	// Future
	// virtual std::wstring	GetErrorString() = 0;
	// virtual void			Resize( uint16 newWidth, uint16 newHeight ) = 0;
};
