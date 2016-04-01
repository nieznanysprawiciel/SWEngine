#include "EngineCore/stdafx.h"
#include "ActorsApi.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api
{

/**@brief */
std::vector<DynamicMeshActor*> ActorsApi::GetSceneObjects()
{ return Context->displayEngine->GetSceneObjects(); }


/**@brief 
@deprecated*/
void ActorsApi::AddDynamicMesh( DynamicMeshActor* object )
{
	Context->displayEngine->AddDynamicMeshObject( object );
	Context->objectList.push_back( object );
}

/**@brief */
void ActorsApi::CleanScene()
{	Context->displayEngine->DeleteAllMeshes();	}


/**@brief Dodaje aktora do wszystkich modu³ów zgodnie z zawartoœci¹ struktury
ActorInfo.*/
void ActorsApi::AddToModules( Object* newActor, ActorInfo actorModules )
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
		Context->movementEngine->add_moveable_object( static_cast< DynamicActor* >( newActor ) );
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



}	// Api
