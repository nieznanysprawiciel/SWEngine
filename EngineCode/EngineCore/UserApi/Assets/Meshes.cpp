/**
@file Meshes.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "Meshes.h"

#include "EngineCore/MainEngine/EngineContext.h"



namespace sw {
namespace Api {
namespace Assets
{

/**@brief Pobiera model o podanej nazwie.

Model jest pobierany tylko, je�eli zosta� wcze�niej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj�cy model lub nullptr, je�eli model nie zosta� wcze�niej wczytany.*/
ResourcePtr< MeshAsset >			Meshes::GetSync			( const std::wstring& name )
{
	return Context->modelsManager->GetMesh( name );
}


/**@brief Wczytuje model z podanego pliku.

Model jest �adowany synchronicznie, dzia�anie silnika zawiesza si�, dop�ki wczytywanie nie zako�czy si�.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je�eli wczytywanie nie powiod�o si�.*/
ResourcePtr< MeshAsset >			Meshes::LoadSync		( const std::wstring& name )
{
	return Context->modelsManager->LoadMesh( name );
}

// ================================ //
//
ResourcePtr< MeshAsset >			Meshes::CreateMeshSync	( const std::wstring& name, MeshInitData&& initData )
{
	return Context->modelsManager->CreateMesh( name, std::move( initData ) );
}

// ================================ //
//
ResourcePtr< MeshAsset >			Meshes::CreateMeshSync	( const std::wstring& name, MeshCreateData&& initData )
{
	return Context->modelsManager->CreateMesh( name, std::move( initData ) );
}

/**@copydoc AssetsManager::ListMeshes.*/
std::vector< ResourcePtr< MeshAsset > >		Meshes::List()
{
	return Context->modelsManager->ListMeshes();
}

}	// Assets
}	// Api
}	// sw