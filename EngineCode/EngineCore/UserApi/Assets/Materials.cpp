#include "EngineCore/stdafx.h"
#include "Materials.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace sw {
namespace Api {
namespace Assets
{

/**@brief Pobiera materia³ o podanej nazwie.*/
ResourcePtr< MaterialAsset >		Materials::GetSync				( const std::wstring& name )
{
	return Context->modelsManager->GetMaterial( name );
}

// ================================ //
//
ResourcePtr< MaterialAsset >		Materials::CreateMaterialSync	( const std::wstring& name, MaterialInitData&& initData )
{
	return Context->modelsManager->CreateMaterial( name, std::move( initData ) );
}

// ================================ //
//
ResourcePtr< MaterialAsset >		Materials::CreateMaterialSync	( const std::wstring& name, MaterialCreateData&& initData )
{
	return Context->modelsManager->CreateMaterial( name, std::move( initData ) );
}

/**@copydoc AssetsManager::ListMaterials.*/
std::vector< ResourcePtr< MaterialAsset > > Materials::List()
{
	return Context->modelsManager->ListMaterials();
}



}	// Assets
}	// Api
}	// sw