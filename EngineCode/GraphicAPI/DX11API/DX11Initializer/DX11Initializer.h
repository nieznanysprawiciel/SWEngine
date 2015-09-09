#pragma once
/**
@file DX11Initializer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "../../IGraphicAPIInitializer.h"
#include "DX11APIObjects.h"

class DX11Initializer : public IGraphicAPIInitializer, public DX11AuxiliaryObjects
{
private:
protected:
public:
	DX11Initializer() = default;
	~DX11Initializer() = default;

	virtual IRenderer*		CreateRenderer			( RendererUsage usage ) override;
	virtual bool			InitAPI					( GraphicAPIInitData& initData ) override;
	virtual void			ReleaseAPI				() override;
};

