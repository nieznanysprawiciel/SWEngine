#pragma once

#include "EngineEditor/LightmapsTool/LightmapWorker.h"



class LightmapWorkerCPU	:	public LightmapWorker
{
private:
protected:
public:
	LightmapWorkerCPU( SceneData* sceneData );
	~LightmapWorkerCPU() = default;

	/**@brief G��wna funkcja generuj�ca lightmapy. Zaimplementuj w klasie pochodnej.*/
	virtual void	operator()		() override;
};

