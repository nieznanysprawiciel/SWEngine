#pragma once


class StaticActor;
class ISerializer;

class SceneHelpers
{
public:

	static void			SetActorInitialPosition		( StaticActor* actor, double mouseX, double mouseY );
	static ISerializer*	CreateSerializerWithContext	();
};

