#include "EngineCore/stdafx.h"
#include "Materials.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{

/**@brief Pobiera materia� o podanej nazwie.*/
MaterialObject*		Materials::GetSync		( const std::wstring& name )
{
	return Context->modelsManager->GetMaterialObject( name );
}

/**@copydoc AssetsManager::ListMaterials.*/
std::vector< ResourcePtr< MaterialObject > > Materials::List()
{
	return Context->modelsManager->ListMaterials();
}



}	// Assets
}	// Api