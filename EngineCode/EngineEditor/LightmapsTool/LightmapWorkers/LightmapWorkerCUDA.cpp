#include "EngineEditor/LightmapsTool/stdafx.h"

#include "EngineEditor/LightmapsTool/LightmapWorkers/LightmapWorkerCUDA.h"



#include "Common/memory_leaks.h"

using namespace DirectX;

LightmapWorkerCUDA::LightmapWorkerCUDA( SceneData* sceneData )
	: LightmapWorker( sceneData )
{
	m_threshold = 0.04f;
}




