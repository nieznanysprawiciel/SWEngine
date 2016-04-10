#include "EngineCore/stdafx.h"
#include "Materials.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{

/**@brief Pobiera materia³ o podanej nazwie.*/
MaterialObject*		Materials::GetSync		( const std::wstring& name )
{
	return Context->modelsManager->GetMaterial( name );
}



}	// Assets
}	// Api