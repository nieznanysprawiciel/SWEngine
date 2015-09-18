#pragma once

#include "Common/SWEngineInclude.h"

class EntryPointGamePlay :	public GamePlay
{
private:

protected:

public:
	EntryPointGamePlay();
	~EntryPointGamePlay();

	// Funkcje czysto wirtualne odziedziczone po GamePlay
	virtual void		proceed_game_play( float time_interval ) override;
	virtual int			load_level() override;
	virtual int			unload_level() override;
};



