#include "EngineCore/stdafx.h"
#include "ActorsApi.h"

#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/Actors/ActorInfo.h"

namespace Api
{

/**@brief Pobiera aktor�w z modu�y @ref DisplayEngine.
@deprecated Pozostawione dla zgodno�ci z LightmapTools. Zamiast tego nale�y u�ywa� funkcji
@ref ActorsApi::GetAllActors, kt�ra zwaraca wszystkicj aktor�w w silniku.*/
std::vector<DynamicMeshActor*> ActorsApi::GetSceneObjects()
{ return Context->displayEngine->GetSceneObjects(); }


/**@brief 
@deprecated Pozostawione dla zgodno�ci z LightmapTools. Zamiast tego nale�y u�ywa� funkcji
@ref ActorsApi::AddToModules*/
void ActorsApi::AddDynamicMesh( DynamicMeshActor* object )
{
	Context->displayEngine->AddDynamicMeshObject( object );
	Context->objectList.push_back( object );
}

/**@brief */
void ActorsApi::CleanScene()
{	Context->displayEngine->DeleteAllMeshes();	}


/**@brief Zwraca map� nazw i ich typ�w.*/
const std::map<std::string, ActorType>& ActorsApi::GetRegisteredClasses()
{
	return Context->actorsManager->GetActorFactory().GetRegisteredClasses();
}


/**@brief Dodaje aktora do wszystkich modu��w zgodnie z zawarto�ci� struktury
ActorInfo.*/
void ActorsApi::AddToModules( ActorBase* newActor, ActorInfo actorModules )
{
	assert( newActor != nullptr );		// W trybie release nie powinno si� zdarzy�.

	if( actorModules.enableDisplay )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast< DynamicMeshActor* >( newActor ) );
		Context->displayEngine->AddDynamicMeshObject( static_cast< DynamicMeshActor* >( newActor ) );
	}

	if( actorModules.enableMovement )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast< DynamicActor* >( newActor ) );
		Context->movementEngine->AddMoveableObject( static_cast< DynamicActor* >( newActor ) );
	}

	if( actorModules.enablePreController )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast< DynamicActor* >( newActor ) );
		Context->controllersEngine->AddPreControlled( static_cast< DynamicActor* >( newActor ) );
	}

	if( actorModules.enablePostController )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast< DynamicActor* >( newActor ) );
		Context->controllersEngine->AddPostControlled( static_cast< DynamicActor* >( newActor ) );
	}

	if( actorModules.isCamera )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr::rttr_cast< CameraActor* >( newActor ) );
		Context->displayEngine->AddCamera( static_cast< CameraActor* >( newActor ) );
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

/**@brief Pobiera dane o wszystkich aktorach.

@return Zwraca referencj� na wektor @ref ActorData.*/
const std::vector<ActorData>&		ActorsApi::GetAllActors()
{
	return Context->actorsManager->GetAllActors();
}

/**@brief Specjalizacja szablonu dla ActorBase. Dzi�ki temu edytor nie b�dzie musia� includowa�
EngineContext i nie b�dzie krzycza�, �e klasa std::mutex mu nie pasuje.*/
template<>
ActorBase*							ActorsApi::CreateActor< ActorBase >		( const std::string& name )
{
	ActorBase* newActor = Context->actorsManager->CreateActor< ActorBase >( name, ActorInfoFlag::DisableAll );

	if( newActor == nullptr )
		return nullptr;

	return newActor;
}

}	// Api
