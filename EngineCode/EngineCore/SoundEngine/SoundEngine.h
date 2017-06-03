#pragma once
/**
@file SoundEngine.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


class Engine;



namespace sw
{


/**@brief Modu� do obs�ugi d�wi�k�w.
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
