#include "EngineCore/stdafx.h"
#include "ActorsApi.h"

#include "EngineCore/MainEngine/EngineContext.h"
#include "EngineCore/Actors/ActorInfo.h"

namespace Api
{

/**@brief Pobiera aktorów z modu³y @ref DisplayEngine.
@deprecated Pozostawione dla zgodnoœci z LightmapTools. Zamiast tego nale¿y u¿ywaæ funkcji
@ref ActorsApi::GetAllActors, która zwaraca wszystkicj aktorów w silniku.*/
std::vector<DynamicMeshActor*> ActorsApi::GetSceneObjects()
{ return Context->displayEngine->GetSceneObjects(); }


/**@brief 
@deprecated Pozostawione dla zgodnoœci z LightmapTools. Zamiast tego nale¿y u¿ywaæ funkcji
@ref ActorsApi::AddToModules*/
void ActorsApi::AddDynamicMesh( DynamicMeshActor* object )
{
	Context->displayEngine->AddDynamicMeshObject( object );
	Context->objectList.push_back( object );
}

/**@brief */
void ActorsApi::CleanScene()
{	Context->displayEngine->DeleteAllMeshes();	}


/**@brief Zwraca mapê nazw i ich typów.*/
const std::map<std::string, ActorType>& ActorsApi::GetRegisteredClasses()
{
	return Context->actorsManager->GetActorFactory().GetRegisteredClasses();
}


/**@brief Dodaje aktora do wszystkich modu³ów zgodnie z zawartoœci¹ struktury
ActorInfo.*/
void ActorsApi::AddToModules( ActorBase* newActor, ActorInfo actorModules )
{
	assert( newActor != nullptr );		// W trybie release nie powinno siê zdarzyæ.

	if( actorModules.enableDisplay )
	{
		// @todo Pomyœleæ co zrobiæ w trybie release.
		assert( rttr::rttr_cast< DynamicMeshActor* >( newActor ) );
		Context->displayEngine->AddDynamicMeshObject( static_cast< DynamicMeshActor* >( newActor ) );
	}

	if( actorModules.enableMovement )
	{
		// @todo Pomyœleæ co zrobiæ w trybie release.
		assert( rttr::rttr_cast< DynamicActor* >( newActor ) );
		Context->movementEngine->AddMoveableObject( static_cast< DynamicActor* >( newActor ) );
	}

	if( actorModules.enablePreController )
	{
		// @todo Pomyœleæ co zrobiæ w trybie release.
		assert( rttr::rttr_cast< DynamicActor* >( newActor ) );
		Context->controllersEngine->AddPreControlled( static_cast< DynamicActor* >( newActor ) );
	}

	if( actorModules.enablePostController )
	{
		// @todo Pomyœleæ co zrobiæ w trybie release.
		assert( rttr::rttr_cast< DynamicActor* >( newActor ) );
		Context->controllersEngine->AddPostControlled( static_cast< DynamicActor* >( newActor ) );
	}

	if( actorModules.isCamera )
	{
		// @todo Pomyœleæ co zrobiæ w trybie release.
		assert( rttr::rttr_cast< CameraActor* >( newActor ) );
		Context->displayEngine->AddCamera( static_cast< CameraActor* >( newActor ) );
	}

	// @todo Pomyœleæ co zrobiæ w trybie release, je¿eli actor nie przeszed³by przez asserty.
	Context->actorsManager->UpdateActor( newActor, actorModules );

	//@todo Dodaæ do pozosta³ych modu³ów, kiedy bêdzie to mo¿liwe.
}

/**@brief Zwraca dane o podanym aktorze.*/
const ActorData*					ActorsApi::FindActor				( ActorBase* actor )
{
	return Context->actorsManager->FindActor( actor );
}

/**@brief Pobiera dane o wszystkich aktorach.

@return Zwraca referencjê na wektor @ref ActorData.*/
const std::vector<ActorData>&		ActorsApi::GetAllActors()
{
	return Context->actorsManager->GetAllActors();
}

/**@brief Specjalizacja szablonu dla ActorBase. Dziêki temu edytor nie bêdzie musia³ includowaæ
EngineContext i nie bêdzie krzycza³, ¿e klasa std::mutex mu nie pasuje.*/
template<>
ActorBase*							ActorsApi::CreateActor< ActorBase >		( const std::string& name )
{
	ActorBase* newActor = Context->actorsManager->CreateActor< ActorBase >( name, ActorInfoFlag::DisableAll );

	if( newActor == nullptr )
		return nullptr;

	return newActor;
}

}	// Api
