#include "EngineCore/stdafx.h"
#include "Shaders.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{

/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je¿eli zosta³ wczeœniej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj¹cy model lub nullptr, je¿eli model nie zosta³ wczeœniej wczytany.*/
VertexShaderObject* 		Shaders::GetVertexShaderSync( const std::wstring& name )
{
	return Context->modelsManager->GetVertexShader( name );
}

/**@brief Wczytuje shader z podanego pliku.

Model jest ³adowany synchronicznie, dzia³anie silnika zawiesza siê, dopóki wczytywanie nie zakoñczy siê.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je¿eli wczytywanie nie powiod³o siê.*/
VertexShaderObject* 		Shaders::LoadVertexShaderSync( const std::wstring& name )
{
	return Context->modelsManager->AddVertexShader( name, DEFAULT_VERTEX_SHADER_ENTRY );
}

/**@brief Wczytuje shader z podanego pliku i tworzy layout wierzcho³ka.

Model jest ³adowany synchronicznie, dzia³anie silnika zawiesza siê, dopóki wczytywanie nie zakoñczy siê.
@param[in] name Nazwa pliku.
@param[in] layout WskaŸnik do którego zostanie zwrócony layout.
@param[in] layoutDesc Deskryptor layoutu do stworzenia.
@return Zwraca model lub nullptr, je¿eli wczytywanie nie powiod³o siê.*/
VertexShaderObject* 		Shaders::LoadVertexShaderSync( const std::wstring& name, ShaderInputLayoutObject** layout, InputLayoutDescriptor* layoutDesc )
{
	return Context->modelsManager->AddVertexShader( name, DEFAULT_VERTEX_SHADER_ENTRY, layout, layoutDesc );
}

/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je¿eli zosta³ wczeœniej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj¹cy model lub nullptr, je¿eli model nie zosta³ wczeœniej wczytany.*/
PixelShaderObject* 		Shaders::GetPixelShaderSync( const std::wstring& name )
{
	return Context->modelsManager->GetPixelShader( name );
}

/**@brief Wczytuje shader z podanego pliku.

Model jest ³adowany synchronicznie, dzia³anie silnika zawiesza siê, dopóki wczytywanie nie zakoñczy siê.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je¿eli wczytywanie nie powiod³o siê.*/
PixelShaderObject* 		Shaders::LoadPixelShaderSync( const std::wstring& name )
{
	return Context->modelsManager->AddPixelShader( name, DEFAULT_PIXEL_SHADER_ENTRY );
}


}	// Assets
}	// Api