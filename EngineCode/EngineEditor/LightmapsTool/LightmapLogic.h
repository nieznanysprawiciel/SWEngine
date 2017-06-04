#pragma once

#include "EngineCOre/MainEngine/SWEngineInclude.h"
#include "EngineEditor/LightmapsTool/ExchangeDataStructs.h"
#include "EngineEditor/LightmapsTool/LightmapWorker.h"

#include <set>
#include <thread>

namespace sw {
	class KeyDownEvent;
}

/**Klasa obs³uguj¹ca generowanie lightmap.

Aby w³¹czyæ generowanie nale¿y wcisn¹æ przycisk 1, 2, 3, 4 lub 5.
Wybierana jest jedna z funkcji do generowania.*/
class LightmapLogic : public sw::IGamePlay
{
private:
protected:
	LightmapState			m_lightmapState;
	std::thread*			m_lightmapThread;
	LightmapWorker*			m_lightmapWorker;

	std::vector<StaticActor*>		m_meshes;
	std::vector<StaticActor*>		m_lightmapBuffers;
	ShaderInputLayout*				m_layout;
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
	void			GenerateLightmaps			( const EngineObject* sender, sw::Event* );
	void			RenderEnded					( const EngineObject* sender, sw::Event* );
	void			LoadScene					( sw::Event* );
	///@}
private:
	SceneData*		PrepareSceneData			();
};


