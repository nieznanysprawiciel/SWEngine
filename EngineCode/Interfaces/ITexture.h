#pragma once

#include <string>

class ITexture
{
private:
protected:
	std::wstring			m_fileName;									///<Nazwa pliku z tekstur�.
public:
	std::wstring&			GetFileName() { return m_fileName; }		///<Zwraca nazw� pliku, kt�ry pos�u�y� do stworzenia obiektu.

	virtual bool			CreateFromFile( const std::wstring& fileName ) = 0;
};

