#include "EngineEditor/LightmapsTool/stdafx.h"
#include "LightmapWorkerCPU.h"


LightmapWorkerCPU::LightmapWorkerCPU( SceneData* sceneData )
	: LightmapWorker( sceneData )
{}

/**@brief G³ówna funkcja generuj¹ca lightmapy. Zaimplementuj w klasie pochodnej.*/
void LightmapWorkerCPU::Generate		()
{
	// Test
	for( unsigned int i = 0; i < UINT32_MAX; i ++ )
	{
		volatile int a = i;
		if( i != 0 )
			a = a / i;
	}

	m_lightmapState = LightmapState::GenerationEnded;		// Ustaw na koñcu funkcji, ¿eby w¹tek g³ówny móg³ zebraæ wyniki.
}