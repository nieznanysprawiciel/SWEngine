#pragma once

#include "SW_engine_include.h"

class EntryPoint :	public GamePlay
{
private:

protected:

public:
	EntryPoint();
	~EntryPoint();

	// Funkcje czysto wirtualne odziedziczone po GamePlay
	virtual void proceed_game_play( float time_interval ) override;
	virtual int load_level() override;
	virtual int unload_level() override;
};



