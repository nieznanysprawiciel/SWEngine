#pragma once
/**@file Shaders.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Shaders.*/

class VertexShader;
class PixelShader;
class ShaderInputLayout;
class InputLayoutDescriptor;


#include "EngineCore/UserApi/ApiGroup.h"
#include <string>


namespace Api { namespace Assets {

	/**@brief Zapewnia dostêp do modeli w klasie ModelsManager.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class Shaders : public ApiGroup
	{
	private:
	protected:
	public:
		Shaders() = default;
		~Shaders() = default;

		VertexShader*		GetVertexShaderSync		( const std::wstring& name );
		VertexShader*		LoadVertexShaderSync	( const std::wstring& name );
		VertexShader*		LoadVertexShaderSync	( const std::wstring& name, ShaderInputLayout** layout, InputLayoutDescriptor* layoutDesc );
		PixelShader*		GetPixelShaderSync		( const std::wstring& name );
		PixelShader*		LoadPixelShaderSync		( const std::wstring& name );
	};


}	// Assets
}	// Api
