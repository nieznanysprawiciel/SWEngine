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


/**@brief Dodaje aktora do wszystkich modu��w zgodnie z zawarto�ci� struktury
ActorInfo.*/
void ActorsApi::AddToModules( Object* newActor, ActorInfo actorModules )
{
	assert( newActor != nullptr );		// W trybie release nie powinno si� zdarzy�.

	if( actorModules.enableDisplay )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr_cast< DynamicMeshObject* >( newActor ) );
		Context->displayEngine->AddDynamicMeshObject( static_cast< DynamicMeshObject* >( newActor ) );
	}

	if( actorModules.enableMovement )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr_cast< DynamicObject* >( newActor ) );
		Context->movementEngine->add_moveable_object( static_cast< DynamicObject* >( newActor ) );
	}

	if( actorModules.enablePreController )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr_cast< DynamicObject* >( newActor ) );
		Context->controllersEngine->AddPreControlled( static_cast< DynamicObject* >( newActor ) );
	}

	if( actorModules.enablePostController )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr_cast< DynamicObject* >( newActor ) );
		Context->controllersEngine->AddPostControlled( static_cast< DynamicObject* >( newActor ) );
	}

	if( actorModules.isCamera )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( rttr_cast< CameraObject* >( newActor ) );
		Context->displayEngine->AddCamera( static_cast< CameraObject* >( newActor ) );
	}

	// @todo Pomy�le� co zrobi� w trybie release, je�eli actor nie przeszed�by przez asserty.
	Context->actorsManager->UpdateActor( newActor, actorModules );

	//@todo Doda� do pozosta�ych modu��w, kiedy b�dzie to mo�liwe.
}



}	// Api
