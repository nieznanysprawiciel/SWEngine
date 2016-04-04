#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

class DynamicMeshActor;
class Object;
class StaticActor;

#include "EngineCore/MainEngine/UserApi/ApiGroup.h"
#include "EngineCore/MainEngine/UserApi/Actors/Assets.h"
#include "EngineCore/Actors/ActorInfo.h"

#include <vector>
#include <map>

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
		std::vector<DynamicMeshActor*>		GetSceneObjects();	///<@todo Zlikwidowaæ GetSceneObjects. Stworzone tylko dla LightmapsTool.

		void			AddDynamicMesh	( DynamicMeshActor* object );
		void			CleanScene		();

		template< typename Type = Object >	Type*		CreateActor				( const std::string& name, ActorInfo actorModules );
		template< typename Type = Object >	Type*		CreateActor				( ActorType id, ActorInfo actorModules );
		template< typename Type = Object >	Type*		CreateActor				( const std::string& name );
		template< typename Type = Object >	Type*		CreateActor				( ActorType id );

		const std::map< std::string, ActorType >&		GetRegisteredClasses	();

		//void			DeleteActor				( Object* actor );
		//void			RemoveFromModules		( Object* newActor, ActorInfo actorModules );
		void			AddToModules			( Object* newActor, ActorInfo actorModules );

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

	/**@brief Tworzy aktora na podstawie nazwy.
	
	@param[in] name Nazwa aktora.
	@return Zwraca wskaŸnik na aktora lub nullptr, je¿eli podany typ aktora nie zosta³ zarejestrowany
	lub ¿¹dany typ jest niezgodny z zarejestrowanym typem. W trybie debug takie sytuacje powoduj¹ zatrzymanie aplikacji na assercie.*/
	template<typename Type>
	inline Type* ActorsApi::CreateActor( const std::string& name )
	{
		Type* newActor = Context->actorsManager->CreateActor< Type >( name, actorModules );

		if( newActor == nullptr )
			return nullptr;

		return newActor;
	}

	/**@brief Tworzy aktora na podstawie identyfikatora.
	
	@param[in] id Identyfikator aktora. Jest to liczba zwrócona w momencie rejestracji typu aktora.
	@return Zwraca wskaŸnik na aktora lub nullptr, je¿eli podany typ aktora nie zosta³ zarejestrowany
	lub ¿¹dany typ jest niezgodny z zarejestrowanym typem. W trybie debug takie sytuacje powoduj¹ zatrzymanie aplikacji na assercie.*/
	template<typename Type>
	inline Type* ActorsApi::CreateActor( ActorType id )
	{
		Type* newActor = Context->actorsManager->CreateActor< Type >( id, actorModules );

		if( newActor == nullptr )
			return nullptr;

		return newActor;
	}

}	// Api
