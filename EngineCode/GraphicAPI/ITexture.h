#pragma once
/**
@file ITexture.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/

#include <string>

#include "GraphicAPI/ResourceObject.h"

class ITexture	:	public ResourceObject
{
private:
protected:
	ITexture() : ResourceObject( 0 ) {}
	virtual ~ITexture() = default;
public:

	virtual bool		SaveToFile		( const std::string& filePath ) = 0;
};

