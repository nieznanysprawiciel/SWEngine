#pragma once

#include <string>

class ITexture
{
private:
protected:
	std::wstring			m_fileName;									///<Nazwa pliku z tekstur¹.
public:
	std::wstring&			GetFileName() { return m_fileName; }		///<Zwraca nazwê pliku, który pos³u¿y³ do stworzenia obiektu.

	virtual bool			CreateFromFile( const std::wstring& fileName ) = 0;
};

