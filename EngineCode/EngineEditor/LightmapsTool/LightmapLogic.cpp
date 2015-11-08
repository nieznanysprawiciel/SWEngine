#include "EngineEditor/LightmapsTool/stdafx.h"
#include "EngineEditor/LightmapsTool/LightmapLogic.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"

#include "LightmapWorkers/LightmapWorkerCPU.h"


template<typename Type>
bool FindInVector( std::vector<Type>& vector, Type value, size_t& index )
{
	for( size_t i = 0; i < vector.size(); ++i )
		if( vector[ i ] == value )
		{
			index = i;
			return true;
		}
	return false;
}

/**@brief Wykonuje operacje przygotowuj�ce lightmapy do u�ycia.

Je�eli trwa generowanie lightmap, to funkcja sprawdza cyklicznie czy si� nie sko�czy�o.
Nast�pnie dane s� odbierane od LightmapWorkera i przetwarzane do struktur silnikowych.

@param[in] time Czas od w��czenia silnika.
*/
void LightmapLogic::ProceedGameLogic			( float time )
{
	if( m_lightmapState == LightmapState::Generating )
	{
		// Sprawdzamy stan LightmapWorkera
		if( m_lightmapWorker->GetState() == LightmapState::GenerationEnded )
		{
			m_lightmapThread->join();		// Czekamy a� b�dzie pewne, �e w�tek si� zako�czy�

			// @todo Tu trzeba napisa� przekazanie wynik�w generowania do silnika.

			delete m_lightmapThread;
			delete m_lightmapWorker;
			m_lightmapWorker = nullptr;
			m_lightmapThread = nullptr;
			m_lightmapState = LightmapState::ReadyToGenerate;
		}
	}
}

/**@brief Przygotowuje scen�.*/
int LightmapLogic::LoadLevel					()
{
	auto layer = m_engine->input.GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS1 );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS2 );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS3 );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS4 );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS5 );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::LOAD_LIGHTMAP_SCENE );

	EventDelegate genLightmap;
	genLightmap.bind( this, &LightmapLogic::GenerateLightmaps );
	m_fableEngine->ChangeDelegate( (unsigned int)EventType::KeyDownEvent, genLightmap );

	return 0;
}

/**@brief Funkcja wywo�ywana w momencie zmiany levelu na inny.*/
int LightmapLogic::UnloadLevel					()
{	return 0;	}


/**@brief Wywo�uje generowanie lightmap w silniku.

Funkcja pobiera obiekty statyczne ze sceny.
Poniewa� mechanizm event�w w silniku nie pozwala na razie na podczepianie wielu funkcji
obs�ugi pod jeden event, to wywo�ywana jest te� funkcja LoadScene, je�eli wci�ni�to przycisk L.

@param[in] genLightmapEvent Event generowany po wci�ni�ciu odpowiedniego przycisku.
*/
void LightmapLogic::GenerateLightmaps			( Event* keyEvent )
{
	if( m_lightmapState != LightmapState::ReadyToGenerate )
		return;

	if( keyEvent->type == (unsigned int)EventType::KeyDownEvent )
	{
		KeyDownEvent* genLightmapEvent = static_cast<KeyDownEvent*>( keyEvent );
		if( genLightmapEvent->virtual_index == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS1 ||
			genLightmapEvent->virtual_index == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS2 ||
			genLightmapEvent->virtual_index == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS3 ||
			genLightmapEvent->virtual_index == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS4 ||
			genLightmapEvent->virtual_index == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS5 )
		{
			SceneData* sceneData = PrepareSceneData();

			// Tutaj wysy�amy dane do innych w�tk�w, �eby liczy�y wszystko w tle.
			if( genLightmapEvent->virtual_index == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS1 )
			{
				LightmapWorkerCPU* worker = new LightmapWorkerCPU( sceneData );
				m_lightmapWorker = worker;
				m_lightmapThread = new std::thread( &LightmapWorkerCPU::Generate, std::ref( worker ) );
			}

			m_lightmapState = LightmapState::Generating;
		}
		else if( genLightmapEvent->virtual_index == STANDARD_LAYERS::PROTOTYPE_BUTTONS::LOAD_LIGHTMAP_SCENE )
			LoadScene( genLightmapEvent );
	}
}

/**Zbiera dane o obiektach na scenie i przygotowuje do wys�ania do LightmapWorkera.
@return Zwraca dane sceny w strukturze SceneData.*/
SceneData* LightmapLogic::PrepareSceneData			()
{
	std::vector<DynamicMeshObject*> meshes = m_engine->actors.GetSceneObjects();
	SceneData* sceneData = new SceneData;

	for( auto mesh : meshes )
	{
		auto meshData = mesh->GetModelParts();
		auto vertexBuff = mesh->GetVertexBuffer();

		DirectX::XMVECTOR objectPos = mesh->get_position();
		DirectX::XMVECTOR objectRot = mesh->get_orientation();		// Quaternion orientation
		// Zak�adam, �e nie obs�ugujemy skalowania, bo na razie nie wiem czy silnik b�dzie je obs�ugiwa� czy to b�dzie gdzie� prekalkulowane.
		DirectX::XMMATRIX objectTransform = DirectX::XMMatrixTranslationFromVector( objectPos );
		objectTransform = DirectX::XMMatrixMultiply( DirectX::XMMatrixRotationQuaternion( objectRot ), objectTransform );

		for( auto& meshPart : meshData )
		{
			SceneObjectData partData;
			partData.object = mesh;
			partData.bufferOffset = meshPart.mesh->buffer_offset;
			partData.verticesCount = meshPart.mesh->vertices_count;
			partData.diffuse = meshPart.material->Diffuse;
			partData.emissive = meshPart.material->Emissive;

			DirectX::XMMATRIX partTransform = DirectX::XMLoadFloat4x4( &meshPart.mesh->transform_matrix );
			objectTransform = DirectX::XMMatrixMultiply( partTransform, objectTransform );
			DirectX::XMStoreFloat4x4( &partData.transform, objectTransform );

			size_t index;
			if( FindInVector( sceneData->buffers, vertexBuff, index ) )
				partData.chunkIdx = index;
			else
			{
				sceneData->buffers.push_back( vertexBuff );
				MemoryChunk vertexChunk = vertexBuff->CopyData();
						
				sceneData->verticies.push_back( std::move( vertexChunk ) );
				partData.chunkIdx = sceneData->verticies.size() - 1;
			}

			sceneData->objectParts.push_back( partData );
		}
	}

	return sceneData;
}

/**@brief Obs�uga wci�niecia przycisku do �adowania sceny.

@todo Tutaj p�niej znajdzie si� kod do �adowania jakiej� sceny, albo nawet kilku.
By� mo�e trzeba b�dzie zrobi� jakis plik konfiguracyjny do wczytywania, ale
w wersji pocz�tkowej mo�e by� wszystko tworzone z kodu.*/
void LightmapLogic::LoadScene					( Event* keyEvent )
{

}

