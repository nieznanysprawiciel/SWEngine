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

Model jest pobierany tylko, je¿eli zosta³ wczeœniej wczytany.
@param[in] name Nazwa pliku z modelem.
@return Zwraca obiekt zawieraj¹cy model lub nullptr, je¿eli model nie zosta³ wczeœniej wczytany.*/
ResourcePtr< MeshAsset >			Meshes::GetSync			( const std::wstring& name )
{
	return Context->modelsManager->GetMesh( name );
}


/**@brief Wczytuje model z podanego pliku.

Model jest ³adowany synchronicznie, dzia³anie silnika zawiesza siê, dopóki wczytywanie nie zakoñczy siê.
@param[in] name Nazwa pliku.
@return Zwraca model lub nullptr, je¿eli wczytywanie nie powiod³o siê.*/
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