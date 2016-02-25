#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

class DynamicMeshObject;
class Object;
class StaticObject;

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"
#include "EngineCore/MainEngine/UserApi/Actors/Assets.h"
#include "EngineCore/Actors/ActorInfo.h"

#include <vector>

namespace Api
{

/**@defgroup ActorsAPI Zarz¹dzanie aktorami.
@ingroup UserAPI
@brief Funkcje do zarz¹dzania aktorami.


	/**@brief Zawiera funkcje do obs³ugi aktorów.
	@ingroup UserAPI
	@ingroup ActorsAPI*/
	class ActorsApi : public ApiGroup
	{
	public:
		Actors::Assets		assets;			///< Pozwala na ustawianie assetów aktorom.

	private:
	protected:
	public:
		ActorsApi() = default;
		~ActorsApi() = default;
	public:
		std::vector<DynamicMeshObject*>		GetSceneObjects();	///<@todo Zlikwidowaæ GetSceneObjects. Stworzone tylko dla LightmapsTool.

		void			AddDynamicMesh	( DynamicMeshObject* object );
		void			CleanScene		();

		template< typename Type = Object >	Type*		CreateActor				( const std::string& name, ActorInfo actorModules );
		template< typename Type = Object >	Type*		CreateActor				( ActorType id, ActorInfo actorModules );

	private:
		void			AddToModules	( Object* newActor, ActorInfo actorModules );
	};


	/**@brief Tworzy aktora na podstawie nazwy.
	
	@param[in] name Nazwa aktora.
	@param[in] actorModules Struktura ActorInfo opisuj¹ca, do których modu³ów nale¿y dodaæ aktora.
	@return Zwraca wskaŸnik na aktora lub nullptr, je¿eli podany typ aktora nie zosta³ zarejestrowany
	lub ¿¹dany typ jest niezgodny z zarejestrowanym typem. W trybie debug takie sytuacje powoduj¹ zatrzymanie aplikacji na assercie.*/
	template<typename Type>
	inline Type* ActorsApi::CreateActor( const std::string& name, ActorInfo actorModules )
	{
		Type* newActor = Context->actorsManager->CreateActor< Type >( name, actorModules );

		if( newActor == nullptr )
			return nullptr;

		AddToModules( newActor, actorModules );
		return newActor;
	}

	/**@brief Tworzy aktora na podstawie identyfikatora.
	
	@param[in] id Identyfikator aktora. Jest to liczba zwrócona w momencie rejestracji typu aktora.
	@param[in] actorModules Struktura ActorInfo opisuj¹ca, do których modu³ów nale¿y dodaæ aktora.
	@return Zwraca wskaŸnik na aktora lub nullptr, je¿eli podany typ aktora nie zosta³ zarejestrowany
	lub ¿¹dany typ jest niezgodny z zarejestrowanym typem. W trybie debug takie sytuacje powoduj¹ zatrzymanie aplikacji na assercie.*/
	template<typename Type>
	inline Type* ActorsApi::CreateActor( ActorType id, ActorInfo actorModules )
	{
		Type* newActor = Context->actorsManager->CreateActor< Type >( id, actorModules );

		if( newActor == nullptr )
			return nullptr;

		AddToModules( newActor, actorModules );
		return newActor;
	}

}	// Api
