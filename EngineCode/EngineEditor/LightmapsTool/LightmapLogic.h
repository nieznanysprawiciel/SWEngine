#pragma once

#include "Common/SWEngineInclude.h"
#include "EngineEditor/LightmapsTool/ExchangeDataStructs.h"
#include "EngineEditor/LightmapsTool/LightmapWorker.h"


#include <thread>


class LightmapLogic : public IGamePlay
{
private:
protected:
	LightmapState			m_lightmapState;
	std::thread				m_lightmapThread;
	LightmapWorker*		m_lightmapWorker;
public:
	LightmapLogic()
		:	m_lightmapState( LightmapState::ReadyToGenerate ),
			m_lightmapWorker( nullptr )
	{}
	~LightmapLogic() = default;


	virtual void	ProceedGameLogic			( float time ) override;
	virtual int		LoadLevel					() override;
	virtual int		UnloadLevel					() override;

private:
	void			GenerateLightmaps			( Event* );
};


