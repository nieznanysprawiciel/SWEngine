#include "EngineCore/stdafx.h"
#include "Models.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{

/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je�eli zosta� wcze�niej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj�cy model lub nullptr, je�eli model nie zosta� wcze�niej wczytany.*/
ResourcePtr< MeshAsset >	Models::GetSync		( const std::wstring& name )
{
	return Context->modelsManager->GetMesh( name );
}


/**@brief Wczytuje model z podanego pliku.

Model jest �adowany synchronicznie, dzia�anie silnika zawiesza si�, dop�ki wczytywanie nie zako�czy si�.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je�eli wczytywanie nie powiod�o si�.*/
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