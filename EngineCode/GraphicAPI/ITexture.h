#pragma once

#include "ReferencedObject.h"
#include <string>

class ITexture	:	public referenced_object
{
private:
protected:
	ITexture() : referenced_object( 0 ) {}
public:
	virtual bool			CreateFromFile( const std::wstring& fileName ) = 0;
	virtual bool			CreateFromMemmory() = 0;
};

