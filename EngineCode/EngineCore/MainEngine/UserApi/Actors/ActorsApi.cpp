#include "EngineCore/stdafx.h"
#include "ActorsApi.h"

#include "EngineCore/MainEngine/EngineContext.h"

namespace Api
{

/**@brief */
std::vector<DynamicMeshObject*> ActorsApi::GetSceneObjects()
{ return Context->displayEngine->GetSceneObjects(); }


/**@brief */
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
		assert( dynamic_cast< DynamicMeshObject* >( newActor ) != nullptr );
		Context->displayEngine->AddDynamicMeshObject( static_cast< DynamicMeshObject* >( newActor ) );
	}

	if( actorModules.enableMovement )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( dynamic_cast< DynamicObject* >( newActor ) != nullptr );
		Context->movementEngine->add_moveable_object( static_cast< DynamicObject* >( newActor ) );
	}

	if( actorModules.enablePreController )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( dynamic_cast< DynamicObject* >( newActor ) != nullptr );
		Context->controllersEngine->AddPreControlled( static_cast< DynamicObject* >( newActor ) );
	}

	if( actorModules.enablePostController )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( dynamic_cast< DynamicObject* >( newActor ) != nullptr );
		Context->controllersEngine->AddPostControlled( static_cast< DynamicObject* >( newActor ) );
	}

	if( actorModules.isCamera )
	{
		// @todo Pomy�le� co zrobi� w trybie release.
		assert( dynamic_cast< CameraObject* >( newActor ) != nullptr );
		Context->displayEngine->AddCamera( static_cast< CameraObject* >( newActor ) );
	}

	//@todo Doda� do pozosta�ych modu��w, kiedy b�dzie to mo�liwe.
}



}	// Api
