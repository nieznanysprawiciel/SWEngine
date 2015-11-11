#pragma once

#include "EngineEditor/LightmapsTool/ExchangeDataStructs.h"


class LightmapWorker
{
private:
protected:
	SceneData*					m_data;
	LightmapState				m_lightmapState;
	std::vector<MemoryChunk>	m_resultData;			///<Zawiera wspó³rzêdne UV oraz dane kolorów wierzcho³ków. (Nie przechowuje wierzcho³ków!!)
public:
	LightmapWorker( SceneData* sceneData )
			:	m_data( sceneData ),
				m_lightmapState( LightmapState::Generating )
	{}
	~LightmapWorker() = default;

	SceneData*					GetSceneData	()	{ return m_data; }
	LightmapState				GetState		()	{ return m_lightmapState; }
	std::vector<MemoryChunk>&	GetResult		()	{ return m_resultData; }

	/**@brief G³ówna funkcja generuj¹ca lightmapy. Zaimplementuj w klasie pochodnej.*/
	virtual void				Generate		() = 0;
};

