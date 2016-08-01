#pragma once

#include "Common/SWEngineInclude.h"
#include "EngineEditor/LightmapsTool/ExchangeDataStructs.h"
#include "EngineEditor/LightmapsTool/LightmapWorker.h"

#include <set>
#include <thread>

class KeyDownEvent;

/**Klasa obs³uguj¹ca generowanie lightmap.

Aby w³¹czyæ generowanie nale¿y wcisn¹æ przycisk 1, 2, 3, 4 lub 5.
Wybierana jest jedna z funkcji do generowania.*/
class LightmapLogic : public IGamePlay
{
private:
protected:
	LightmapState			m_lightmapState;
	std::thread*			m_lightmapThread;
	LightmapWorker*			m_lightmapWorker;

	std::vector<DynamicMeshActor*>		m_meshes;
	std::vector<DynamicMeshActor*>		m_lightmapBuffers;
	ShaderInputLayout*			m_layout;
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
	///@name Funkcje obs³ugi eventów.
	///@{
	void			GenerateLightmaps			( const EngineObject* sender, Event* );
	void			RenderEnded					( const EngineObject* sender, Event* );
	void			LoadScene					( Event* );
	///@}
private:
	SceneData*		PrepareSceneData			();
};


