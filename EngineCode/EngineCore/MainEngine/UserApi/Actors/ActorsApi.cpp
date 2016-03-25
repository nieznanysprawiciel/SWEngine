#include "EngineCore/stdafx.h"
#include "ActorsApi.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api
{

/**@brief */
std::vector<DynamicMeshObject*> ActorsApi::GetSceneObjects()
{ return Context->displayEngine->GetSceneObjects(); }


/**@brief 
@deprecated*/
void ActorsApi::AddDynamicMesh( DynamicMeshObject* object )
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
		assert( rttr_cast< DynamicMeshObject* >( newActor ) );
		Context->displayEngine->AddDynamicMeshObject( static_cast< DynamicMeshObject* >( newActor ) );
	}

	if( actorModules.enableMovement )
	{
		// @todo Pomyœleæ co zrobiæ w trybie release.
		assert( rttr_cast< DynamicObject* >( newActor ) );
		Context->movementEngine->add_moveable_object( static_cast< DynamicObject* >( newActor ) );
	}

	if( actorModules.enablePreController )
	{
		// @todo Pomyœleæ co zrobiæ w trybie release.
		assert( rttr_cast< DynamicObject* >( newActor ) );
		Context->controllersEngine->AddPreControlled( static_cast< DynamicObject* >( newActor ) );
	}

	if( actorModules.enablePostController )
	{
		// @todo Pomyœleæ co zrobiæ w trybie release.
		assert( rttr_cast< DynamicObject* >( newActor ) );
		Context->controllersEngine->AddPostControlled( static_cast< DynamicObject* >( newActor ) );
	}

	if( actorModules.isCamera )
	{
		// @todo Pomyœleæ co zrobiæ w trybie release.
		assert( rttr_cast< CameraObject* >( newActor ) );
		Context->displayEngine->AddCamera( static_cast< CameraObject* >( newActor ) );
	}

	// @todo Pomyœleæ co zrobiæ w trybie release, je¿eli actor nie przeszed³by przez asserty.
	Context->actorsManager->UpdateActor( newActor, actorModules );

	//@todo Dodaæ do pozosta³ych modu³ów, kiedy bêdzie to mo¿liwe.
}



}	// Api
