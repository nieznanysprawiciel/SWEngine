#pragma once
/**@file Shaders.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Shaders.*/

class VertexShaderObject;
class PixelShaderObject;
class ShaderInputLayoutObject;
class InputLayoutDescriptor;


#include "EngineCore/MainEngine/UserApi/ApiGroup.h"
#include <string>


namespace Api { namespace Assets {

	/**@brief Zapewnia dostêp do modeli w klasie ModelsManager.*/
	class Shaders : public ApiGroup
	{
	private:
	protected:
	public:
		Shaders() = default;
		~Shaders() = default;

		VertexShaderObject*		GetVertexShaderSync		( const std::wstring& name );
		VertexShaderObject*		LoadVertexShaderSync	( const std::wstring& name );
		VertexShaderObject*		LoadVertexShaderSync	( const std::wstring& name, ShaderInputLayoutObject** layout, InputLayoutDescriptor* layoutDesc );
		PixelShaderObject*		GetPixelShaderSync		( const std::wstring& name );
		PixelShaderObject*		LoadPixelShaderSync		( const std::wstring& name );
	};


}	// Assets
}	// Api
