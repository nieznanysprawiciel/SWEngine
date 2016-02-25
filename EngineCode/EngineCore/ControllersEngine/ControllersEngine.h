#pragma once
#include "EngineCore/stdafx.h"
#include "EngineCore/Actors/ActorObjects.h"

class Engine;

/**@brief Modu� do zarz�dzania kontrolerami obiekt�w.
@ingroup EngineCore*/
class ControllersEngine
{
private:
	Engine* engine;

	std::vector<DynamicObject*>		m_preControlledObjects;
	std::vector<DynamicObject*>		m_postControlledObjects;

public:
	ControllersEngine( Engine* parent );
	~ControllersEngine();

	void		ProceedControllersPre			( float time_interval );
	void		ProceedControllersPost			( float time_interval );

	bool		AddPreControlled				( DynamicObject* object );
	bool		AddPostControlled				( DynamicObject* object );
};

