#pragma once

#include <string>

class ITexture
{
private:
protected:
public:
	virtual bool			CreateFromFile( const std::wstring& fileName ) = 0;
	virtual bool			CreateFromMemmory() = 0;
};

