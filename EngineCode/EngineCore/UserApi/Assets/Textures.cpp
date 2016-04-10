#include "EngineCore/stdafx.h"
#include "Textures.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api { namespace Assets
{

/**@brief Pobiera teksturê o podanej nazwie.

Tekxtura jest pobierana tylko, je¿eli zosta³a wczeœniej wczytana.
@param[in] name Nazwa pliku z tekstur¹ lub po prostu nazwa tekstury, je¿eli zosta³a wygenerowana.
@return Zwraca obiekt zawieraj¹cy teksturê lub nullptr, je¿eli tekstura nie zosta³a wczeœniej wczytana.*/
TextureObject*		Textures::GetTexture		( const std::wstring& name )
{
	return Context->modelsManager->GetTexture( name );
}

}	// Assets
}	// Api