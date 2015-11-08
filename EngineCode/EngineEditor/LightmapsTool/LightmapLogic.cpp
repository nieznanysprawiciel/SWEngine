#include "EngineEditor/LightmapsTool/stdafx.h"
#include "EngineEditor/LightmapsTool/LightmapLogic.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"




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

/**@brief Wykonuje operacje przygotowuj¹ce lightmapy do u¿ycia.

Je¿eli trwa generowanie lightmap, to funkcja sprawdza cyklicznie czy siê nie skoñczy³o.
Nastêpnie dane s¹ odbierane od LightmapWorkera i przetwarzane do struktur silnikowych.

@param[in] time Czas od w³¹czenia silnika.
*/
void LightmapLogic::ProceedGameLogic			( float time )
{
	if( m_lightmapState == LightmapState::Generating )
	{
		// Sprawdzamy stan LightmapWorkera
		if( m_lightmapWorker->GetState() == LightmapState::GenerationEnded )
		{
			m_lightmapThread.join();		// Czekamy a¿ bêdzie pewne, ¿e w¹tek siê zakoñczy³



			m_lightmapState = LightmapState::ReadyToGenerate;
		}
	}
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
	if( m_lightmapState != LightmapState::ReadyToGenerate )
		return;

	if( keyEvent->type == (unsigned int)EventType::KeyDownEvent )
	{
		m_lightmapState = LightmapState::Generating;

		KeyDownEvent* genLightmapEvent = static_cast<KeyDownEvent*>( keyEvent );
		if( genLightmapEvent->virtual_index == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS )
		{
			std::vector<DynamicMeshObject*> meshes = m_engine->actors.GetSceneObjects();
			SceneData* sceneData = new SceneData;

			for( auto mesh : meshes )
			{
				auto meshData = mesh->GetModelParts();
				auto vertexBuff = mesh->GetVertexBuffer();

				DirectX::XMVECTOR objectPos = mesh->get_position();
				DirectX::XMVECTOR objectRot = mesh->get_orientation();		// Quaternion orientation
				// Zak³adam, ¿e nie obs³ugujemy skalowania, bo na razie nie wiem czy silnik bêdzie je obs³ugiwa³ czy to bêdzie gdzieœ prekalkulowane.
				DirectX::XMMATRIX objectTransform = DirectX::XMMatrixTranslationFromVector( objectPos );
				objectTransform = DirectX::XMMatrixMultiply( DirectX::XMMatrixRotationQuaternion( objectRot ), objectTransform );

				for( auto& meshPart : meshData )
				{
					// Transformacje !!!!!!

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

			// Tutaj wysy³amy dane do innych w¹tków, ¿eby liczy³y wszystko w tle.
			// @todo

		}
	}
}

