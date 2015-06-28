#pragma once

#include <string>

class IShader	:	public referenced_object
{
private:
protected:
	std::string				m_shaderEntry;			///< Nazwa g³ownej funkcji shadera.
	std::wstring			m_shaderFile;			///< Œcie¿ka do pliku z shaderem.
public:
	virtual ~IShader();

	
	std::wstring&			GetShaderFile()		{ return m_shaderFile; }
	std::string&			GetShaderEntry()	{ return m_shaderEntry; }


	virtual bool			CreateFromFile			( const std::wstring& file_name, const std::string& shader_name, const char* shader_model ) = 0;
	virtual bool			CreateFromBinFile		( const std::wstring& file_name ) = 0;
	virtual void			SaveShaderBinFile		( const std::wstring& file_name ) = 0;
};

