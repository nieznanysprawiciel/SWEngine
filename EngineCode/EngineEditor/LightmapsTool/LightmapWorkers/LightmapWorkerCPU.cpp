#include "EngineEditor/LightmapsTool/stdafx.h"
#include "LightmapWorkerCPU.h"


LightmapWorkerCPU::LightmapWorkerCPU( SceneData* sceneData )
	: LightmapWorker( sceneData )
{}

/**@brief G��wna funkcja generuj�ca lightmapy. Zaimplementuj w klasie pochodnej.*/
void LightmapWorkerCPU::Generate		()
{
	// Test
	for( unsigned int i = 0; i < UINT32_MAX; i ++ )
	{
		volatile int a = i;
		if( i != 0 )
			a = a / i;
	}

	m_lightmapState = LightmapState::GenerationEnded;		// Ustaw na ko�cu funkcji, �eby w�tek g��wny m�g� zebra� wyniki.
}