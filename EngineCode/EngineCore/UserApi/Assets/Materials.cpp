#include "EngineCore/stdafx.h"
#include "Materials.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{

/**@brief Pobiera materia³ o podanej nazwie.*/
ResourcePtr< MaterialAsset >		Materials::GetSync		( const std::wstring& name )
{
	return Context->modelsManager->GetMaterial( name );
}

/**@copydoc AssetsManager::ListMaterials.*/
std::vector< ResourcePtr< MaterialAsset > > Materials::List()
{
	return Context->modelsManager->ListMaterials();
}



}	// Assets
}	// Api