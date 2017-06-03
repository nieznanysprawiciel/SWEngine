#include "EngineCore/stdafx.h"
#include "Textures.h"

#include "EngineCore/MainEngine/EngineContext.h"



namespace sw {
namespace Api {
namespace Assets
{

/**@brief Pobiera tekstur� o podanej nazwie.

Tekxtura jest pobierana tylko, je�eli zosta�a wcze�niej wczytana.
@param[in] name Nazwa pliku z tekstur� lub po prostu nazwa tekstury, je�eli zosta�a wygenerowana.
@return Zwraca obiekt zawieraj�cy tekstur� lub nullptr, je�eli tekstura nie zosta�a wcze�niej wczytana.*/
TextureObject*		Textures::GetTexture		( const std::wstring& name )
{
	return Context->modelsManager->GetTexture( name );
}

/**@brief Listuje wszystkie wczytane aktualnie tekstury.*/
std::vector< ResourcePtr< TextureObject > >		Textures::List()
{
	return Context->modelsManager->ListTextures();
}

}	// Assets
}	// Api
}	// sw