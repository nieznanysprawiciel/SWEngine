#pragma once

#include "Common/SWEngineInclude.h"

/**@defgroup TestEngine
@brief Modu� do testowania silnika.

Modu� linkuje EngineCore oraz wszystkie inne potrzebne biblioteki i wywo�uje funkcje testowe.
*/


class EntryPointGamePlay :	public IGamePlay
{
private:

protected:

public:
	EntryPointGamePlay();
	~EntryPointGamePlay();

	// Funkcje czysto wirtualne odziedziczone po IGamePlay
	virtual void		proceed_game_play( float time_interval ) override;
	virtual int			load_level() override;
	virtual int			unload_level() override;
};



