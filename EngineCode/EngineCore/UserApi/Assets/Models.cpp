#include "EngineCore/stdafx.h"
#include "Models.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{

/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je¿eli zosta³ wczeœniej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj¹cy model lub nullptr, je¿eli model nie zosta³ wczeœniej wczytany.*/
ResourcePtr< MeshAsset >	Models::GetSync		( const std::wstring& name )
{
	return Context->modelsManager->GetMesh( name );
}


/**@brief Wczytuje model z podanego pliku.

Model jest ³adowany synchronicznie, dzia³anie silnika zawiesza siê, dopóki wczytywanie nie zakoñczy siê.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je¿eli wczytywanie nie powiod³o siê.*/
ResourcePtr< MeshAsset >	Models::LoadSync	( const std::wstring& name )
{
	return Context->modelsManager->LoadMesh( name );
}

/**@copydoc AssetsManager::ListMeshes.*/
std::vector< ResourcePtr< MeshAsset > >		Models::List()
{
	return Context->modelsManager->ListMeshes();
}

}	// Assets
}	// Api