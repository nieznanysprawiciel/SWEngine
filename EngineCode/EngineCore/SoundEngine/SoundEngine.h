#pragma once
/**
@file SoundEngine.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


class Engine;



namespace sw
{


/**@brief Modu³ do obs³ugi dŸwiêków.
@ingroup EngineCore*/
class SoundEngine
{
private:
	Engine*	engine;

public:
	SoundEngine( Engine* engine );
	~SoundEngine();


};

}	// sw
