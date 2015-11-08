#pragma once

#include "Common/SWEngineInclude.h"
#include "EngineEditor/LightmapsTool/ExchangeDataStructs.h"
#include "EngineEditor/LightmapsTool/LightmapWorker.h"


#include <thread>

/**Klasa obs�uguj�ca generowanie lightmap.

Aby w��czy� generowanie nale�y wcisn�� przycisk 1, 2, 3, 4 lub 5.
Wybierana jest jedna z funkcji do generowania.*/
class LightmapLogic : public IGamePlay
{
private:
protected:
	LightmapState			m_lightmapState;
	std::thread*			m_lightmapThread;
	LightmapWorker*			m_lightmapWorker;
public:
	LightmapLogic()
		:	m_lightmapState( LightmapState::ReadyToGenerate ),
			m_lightmapWorker( nullptr ),
			m_lightmapThread( nullptr )
	{}
	~LightmapLogic() = default;


	virtual void	ProceedGameLogic			( float time ) override;
	virtual int		LoadLevel					() override;
	virtual int		UnloadLevel					() override;

private:
	///@name Funkcje obs�ugi event�w.
	///@{
	void			GenerateLightmaps			( Event* );
	void			LoadScene					( Event* );
	///@}
private:
	SceneData*		PrepareSceneData			();
};


