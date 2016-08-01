#include "EngineCore/stdafx.h"
#include "Shaders.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{

/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je�eli zosta� wcze�niej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj�cy model lub nullptr, je�eli model nie zosta� wcze�niej wczytany.*/
VertexShader* 		Shaders::GetVertexShaderSync( const std::wstring& name )
{
	return Context->modelsManager->GetVertexShader( name );
}

/**@brief Wczytuje shader z podanego pliku.

Model jest �adowany synchronicznie, dzia�anie silnika zawiesza si�, dop�ki wczytywanie nie zako�czy si�.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je�eli wczytywanie nie powiod�o si�.*/
VertexShader* 		Shaders::LoadVertexShaderSync( const std::wstring& name )
{
	return Context->modelsManager->AddVertexShader( name, DEFAULT_VERTEX_SHADER_ENTRY );
}

/**@brief Wczytuje shader z podanego pliku i tworzy layout wierzcho�ka.

Model jest �adowany synchronicznie, dzia�anie silnika zawiesza si�, dop�ki wczytywanie nie zako�czy si�.
@param[in] name Nazwa pliku.
@param[in] layout Wska�nik do kt�rego zostanie zwr�cony layout.
@param[in] layoutDesc Deskryptor layoutu do stworzenia.
@return Zwraca model lub nullptr, je�eli wczytywanie nie powiod�o si�.*/
VertexShader* 		Shaders::LoadVertexShaderSync( const std::wstring& name, ShaderInputLayout** layout, InputLayoutDescriptor* layoutDesc )
{
	return Context->modelsManager->AddVertexShader( name, DEFAULT_VERTEX_SHADER_ENTRY, layout, layoutDesc );
}

/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je�eli zosta� wcze�niej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj�cy model lub nullptr, je�eli model nie zosta� wcze�niej wczytany.*/
PixelShader* 		Shaders::GetPixelShaderSync( const std::wstring& name )
{
	return Context->modelsManager->GetPixelShader( name );
}

/**@brief Wczytuje shader z podanego pliku.

Model jest �adowany synchronicznie, dzia�anie silnika zawiesza si�, dop�ki wczytywanie nie zako�czy si�.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je�eli wczytywanie nie powiod�o si�.*/
PixelShader* 		Shaders::LoadPixelShaderSync( const std::wstring& name )
{
	return Context->modelsManager->AddPixelShader( name, DEFAULT_PIXEL_SHADER_ENTRY );
}


}	// Assets
}	// Api