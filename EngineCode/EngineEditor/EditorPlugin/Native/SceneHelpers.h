#pragma once
/**
@file SceneHelpers.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

class ISerializer;


namespace sw
{

class StaticActor;



/**@brief */
class SceneHelpers
{
public:

	static void			SetActorInitialPosition		( StaticActor* actor, double mouseX, double mouseY );
	static ISerializer*	CreateSerializerWithContext	();
};

}	// sw
