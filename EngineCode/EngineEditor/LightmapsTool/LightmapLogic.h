#pragma once

#include "EngineCore/GamePlay/IGamePlay.h"


class LightmapLogic : public IGamePlay
{
private:
protected:
public:
	LightmapLogic() = default;
	~LightmapLogic() = default;


	virtual void		ProceedGameLogic			( float time ) override;
	virtual int			LoadLevel					() override;
	virtual int			UnloadLevel					() override;
};


