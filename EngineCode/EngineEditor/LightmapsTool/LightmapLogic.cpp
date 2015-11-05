#include "EngineEditor/LightmapsTool/stdafx.h"
#include "EngineEditor/LightmapsTool/LightmapLogic.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"


/**@brief 

@param[in] time Czas od w³¹czenia silnika.
*/
void LightmapLogic::ProceedGameLogic			( float time )
{

}

/**@brief Przygotowuje scenê.*/
int LightmapLogic::LoadLevel					()
{
	auto layer = m_engine->input.GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS );

	EventDelegate genLightmap;
	genLightmap.bind( this, &LightmapLogic::GenerateLightmaps );
	m_fableEngine->ChangeDelegate( (unsigned int)EventType::KeyDownEvent, genLightmap );

	return 0;
}

/**@brief Funkcja nic nie robi*/
int LightmapLogic::UnloadLevel					()
{	return 0;	}


/**@brief Wywo³uje generowanie lightmap w silniku.

Funkcja pobiera obiekty statyczne ze sceny.

@param[in] genLightmapEvent Event generowany po wciœniêciu odpowiedniego przycisku.
*/
void LightmapLogic::GenerateLightmaps			( Event* keyEvent )
{
	if( keyEvent->type == (unsigned int)EventType::KeyDownEvent )
	{
		KeyDownEvent* genLightmapEvent = static_cast<KeyDownEvent*>( keyEvent );
		if( genLightmapEvent->virtual_index == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS )
		{
		
		}
	}
}

