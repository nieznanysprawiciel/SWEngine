#pragma once

#include "Common/SWEngineInclude.h"

/**@defgroup TestEngine
@ingroup ModulesStructure
@brief Modu³ do testowania silnika.

Modu³ linkuje @ref EngineCore oraz wszystkie inne potrzebne biblioteki i wywo³uje funkcje testowe.
*/


class EntryPointGamePlay :	public IGamePlay
{
private:

protected:

public:
	EntryPointGamePlay();
	~EntryPointGamePlay();

	// Funkcje czysto wirtualne odziedziczone po IGamePlay
	virtual void		ProceedGameLogic( float time_interval ) override;
	virtual int			LoadLevel() override;
	virtual int			UnloadLevel() override;
};



