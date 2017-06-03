#include "EngineCore/stdafx.h"
#include "ActorsApi.h"

#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/Actors/ActorInfo.h"

namespace sw {
namespace Api
{

/**@brief Pobiera aktor�w z modu�y @ref DisplayEngine.
@deprecated Pozostawione dla zgodno�ci z LightmapTools. Zamiast tego nale�y u�ywa� funkcji
@ref ActorsApi::GetAllActors, kt�ra zwaraca wszystkicj aktor�w w silniku.*/
std::vector<StaticActor*> ActorsApi::GetSceneObjects()
{
	return Context->displayEngine->GetSceneObjects();
}


/**@brief
@deprecated Pozostawione dla zgodno�ci z LightmapTools. Zamiast tego nale�y u�ywa� funkcji
@ref ActorsApi::AddToModules*/
void ActorsApi::AddDynamicMesh( StaticActor* object )
{
	Context->displayEngine->AddMeshObject( object );
	Context->objectList.push_back( object );
}

/**@brief */
void ActorsApi::CleanScene()
{
	Context->displayEngine->DeleteAllMeshes();
}


/**@brief Zwraca map� nazw i ich typ�w.*/
const std::map<std::string, ActorType>& ActorsApi::GetRegisteredClasses()
{
	return Context->actorsManager->GetActorFactory().GetRegisteredClasses();
}

/**@brief Usuwa z silnika wszystkie odwo�ania do podanego aktora.

@todo Funkcja powinna usuwa� aktora dopiero po zako�czeniu klatki.*/
void ActorsApi::DeleteActor( ActorBase* actor )
{
	Context->collisionEngine->RemoveActor( actor );
	Context->controllersEngine->RemoveActor( actor );
	Context->displayEngine->RemoveActor( actor );
	Context->movementEngine->RemoveActor( actor );
	Context->physicEngine->RemoveActor( actor );
	Context->eventsManager->RemoveReferences( actor );

	// Odwo�anie w ActorsManager kasujemy jako ostatnie.
	Context->actorsManager->RemoveActor( actor );
}

/**@brief Usuwa wszystkich aktor�w.*/
void ActorsApi::DeleteAllActors()
{
	Context->collisionEngine->RemoveAllActors();
	Context->controllersEngine->RemoveAllActors();
	Context->displayEngine->RemoveAllActors();
	Context->movementEngine->RemoveAllActors();
	Context->physicEngine->RemoveAllActors();
	Context->eventsManager->RemoveAllActors();

	// Odwo�anie w ActorsManager kasujemy jako ostatnie.
	Context->actorsManager->RemoveAllActors();
}

/**@brief Dodaje aktora do wszystkich modu��w zgodnie z zawarto�ci� struktury
ActorInfo.*/
void ActorsApi::AddToModules( ActorBase* newActor, ActorInfo actorModules )
{
	assert( newActor != nullptr );		// W trybie release nie powinno si� zdarzy�.

	if( actorModules.EnableDisplay() )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast<StaticActor*>( newActor ) );
		Context->displayEngine->AddMeshObject( static_cast<StaticActor*>( newActor ) );
	}

	if( actorModules.EnableMovement() )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast<DynamicActor*>( newActor ) );
		Context->movementEngine->AddMoveableObject( static_cast<DynamicActor*>( newActor ) );
	}

	if( actorModules.EnablePreController() )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast<DynamicActor*>( newActor ) );
		Context->controllersEngine->AddPreControlled( static_cast<DynamicActor*>( newActor ) );
	}

	if( actorModules.EnablePostController() )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast<DynamicActor*>( newActor ) );
		Context->controllersEngine->AddPostControlled( static_cast<DynamicActor*>( newActor ) );
	}

	if( actorModules.IsCamera() )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast<CameraActor*>( newActor ) );
		Context->displayEngine->AddCamera( static_cast<CameraActor*>( newActor ) );
	}

	if( actorModules.IsLight() )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast<LightBase*>( newActor ) );
		Context->displayEngine->GetLightModule()->AddLightActor( static_cast<LightBase*>( newActor ) );
	}

	// @todo Pomy�le� co zrobi� w trybie release, je�eli actor nie przeszed�by przez asserty.
	Context->actorsManager->UpdateActor( newActor, actorModules );

	//@todo Doda� do pozosta�ych modu��w, kiedy b�dzie to mo�liwe.
}

/**@brief Zwraca dane o podanym aktorze.*/
const ActorData*					ActorsApi::FindActor				( ActorBase* actor )
{
	return Context->actorsManager->FindActor( actor );
}

/**@copydoc ActorsManager::FindActorByName*/
ActorData*							ActorsApi::FindActorByName			( const std::string& name )
{
	return Context->actorsManager->FindActorByName( name );
}

/**@copydoc ActorsManager::GetActorByName*/
ActorBase*							ActorsApi::GetActorByName			( const std::string& name )
{
	return Context->actorsManager->GetActorByName( name );
}

/**@brief Pobiera dane o wszystkich aktorach.

@return Zwraca referencj� na wektor @ref ActorData.*/
const std::vector<ActorData>&		ActorsApi::GetAllActors()
{
	return Context->actorsManager->GetAllActors();
}

/**@copydoc ActorsManager::GetActorsNames*/
const std::map<std::string, ActorBase*>& ActorsApi::GetActorsNames()
{
	return Context->actorsManager->GetActorsNames();
}

/**@brief Specjalizacja szablonu dla ActorBase. Dzi�ki temu edytor nie b�dzie musia� includowa�
EngineContext i nie b�dzie krzycza�, �e klasa std::mutex mu nie pasuje.*/
template<>
ActorBase*							ActorsApi::CreateActor< ActorBase >		( const std::string& name )
{
	ActorBase* newActor = Context->actorsManager->CreateActor< ActorBase >( name );

	if( newActor == nullptr )
		return nullptr;

	return newActor;
}

/**@brief Specjalizacja szablonu dla ActorBase. Dzi�ki temu edytor nie b�dzie musia� includowa�
EngineContext i nie b�dzie krzycza�, �e klasa std::mutex mu nie pasuje.*/
template<>
ActorBase*							ActorsApi::CreateActor< ActorBase >		( ActorType id )
{
	ActorBase* newActor = Context->actorsManager->CreateActor< ActorBase >( id );

	if( newActor == nullptr )
		return nullptr;

	return newActor;
}

}	// Api
}	// sw
