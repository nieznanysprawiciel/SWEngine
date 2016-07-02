#pragma once
/**@file RenderingApi.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy RenderingApi.*/

class DynamicMeshActor;
class ActorBase;
class StaticActor;

#include "EngineCore/UserApi/ApiGroup.h"
#include "EngineCore/UserApi/Actors/Assets.h"
#include "EngineCore/UserApi/Actors/Communication.h"
#include "EngineCore/Actors/ActorInfo.h"

#include <vector>
#include <map>

namespace Api
{

/**@defgroup ActorsAPI Zarz¹dzanie aktorami.
@ingroup UserAPI
@brief Funkcje do zarz¹dzania aktorami.*/


	/**@brief Zawiera funkcje do obs³ugi aktorów.
	@ingroup UserAPI
	@ingroup ActorsAPI*/
	class ActorsApi : public ApiGroup
	{
	public:
		Actors::Assets			Assets;			///< Pozwala na ustawianie assetów aktorom.
		Actors::Communication	Communication;	///< Pozwala na rejestrowanie funkcji obs³ugi eventów i wysy³anie sygna³ów.

	private:
	protected:
	public:
		ActorsApi() = default;
		~ActorsApi() = default;
	public:
		std::vector<DynamicMeshActor*>		GetSceneObjects();	///<@todo Zlikwidowaæ GetSceneObjects. Stworzone tylko dla LightmapsTool.

		void			AddDynamicMesh	( DynamicMeshActor* object );
		void			CleanScene		();

		template< typename Type = ActorBase >	Type*		CreateActor				( const std::string& name, ActorInfo actorModules );
		template< typename Type = ActorBase >	Type*		CreateActor				( ActorType id, ActorInfo actorModules );
		template< typename Type = ActorBase >	Type*		CreateActor				( const std::string& name );
		template< typename Type = ActorBase >	Type*		CreateActor				( ActorType id );
		

		//void			DeleteActor				( ActorBase* actor );
		//void			RemoveFromModules		( ActorBase* newActor, ActorInfo actorModules );
		void			AddToModules			( ActorBase* newActor, ActorInfo actorModules );
		
		const ActorData*								FindActor				( ActorBase* actor );
		ActorData*										FindActorByName			( const std::string& name );
		ActorBase*										GetActorByName			( const std::string& name );

		const std::map< std::string, ActorType >&		GetRegisteredClasses	();
		const std::vector< ActorData >&					GetAllActors			();
		const std::map< std::string, ActorBase* >		GetActorsNames			();
	};

	template<>							ActorBase*		ActorsApi::CreateActor				( const std::string& name );
	template<>							ActorBase*		ActorsApi::CreateActor				( ActorType id );

	/**@brief Tworzy aktora na podstawie nazwy.
	
	@param[in] name Nazwa aktora.
	@param[in] actorModules Struktura ActorInfo opisuj¹ca, do których modu³ów nale¿y dodaæ aktora.
	@return Zwraca wskaŸnik na aktora lub nullptr, je¿eli podany typ aktora nie zosta³ zarejestrowany
	lub ¿¹dany typ jest niezgodny z zarejestrowanym typem. W trybie debug takie sytuacje powoduj¹ zatrzymanie aplikacji na assercie.*/
	template<typename Type>
	inline Type* ActorsApi::CreateActor( const std::string& name, ActorInfo actorModules )
	{
		Type* newActor = Context->actorsManager->CreateActor< Type >( name );

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
		Type* newActor = Context->actorsManager->CreateActor< Type >( id );

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
		Type* newActor = Context->actorsManager->CreateActor< Type >( name );

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
		Type* newActor = Context->actorsManager->CreateActor< Type >( id );

		if( newActor == nullptr )
			return nullptr;

		return newActor;
	}

}	// Api
