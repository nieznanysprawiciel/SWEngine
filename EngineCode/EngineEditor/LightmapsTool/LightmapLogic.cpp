#include "EngineEditor/LightmapsTool/stdafx.h"
#include "EngineEditor/LightmapsTool/LightmapLogic.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"



struct SceneObjectData
{
	DynamicMeshObject*		object;				///< Obiekt, do którego nale¿y dana czêœæ mesha. Potrzebujemy, ¿eby potem stworzyæ tekstury i ustawiæ dla odpowiednich obiektów.
	unsigned int			chunkIdx;			///< Wskazuje na obiekt z danymi wierzcho³ków w strukturze SceneData.
	DirectX::XMFLOAT4X4		transform;			///< Macierz przekszta³cenia dla danej czêœci mesha.
	unsigned int			bufferOffset;		///< Offset wzglêdem pocz¹tku bufora albo wierzcho³ków.
	unsigned int			verticesCount;		///< Liczba wierzcho³ków w danym kawa³ku mesha.
	DirectX::XMFLOAT4		diffuse;			///< Materia³ - kana³ diffuse, czyli jedyny, który bierzemy pod uwagê.
	DirectX::XMFLOAT4		emissive;			///< Je¿eli wierzcho³ki opisuj¹ Ÿród³o œwiat³a, to ten kana³ powinien byæ niezerowy.
};


struct SceneData
{
	std::vector<MemoryChunk>		verticies;	///< Bufory wierzcho³ków. MemoryChunki s¹ unikatowe. Wiele obiektów mo¿e siê odwo³ywaæ do jednego chunka.
	std::vector<BufferObject*>		buffers;	///< Silnikowe obiekty buforów. Odpowiadaj¹ MemoryChunkom.
	std::vector<SceneObjectData>	objectParts;///< Obiekty s¹ przechowywane w czêœciach, podzielonych po materia³ach.
};

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
			std::vector<DynamicMeshObject*> meshes = m_engine->actors.GetSceneObjects();
			SceneData* sceneData = new SceneData;

			for( auto mesh : meshes )
			{
				auto meshData = mesh->GetModelParts();
				auto vertexBuff = mesh->GetVertexBuffer();

				for( auto& meshPart : meshData )
				{
					SceneObjectData partData;
					partData.object = mesh;
					partData.bufferOffset = meshPart.mesh->buffer_offset;
					partData.verticesCount = meshPart.mesh->vertices_count;
					partData.diffuse = meshPart.material->Diffuse;
					partData.emissive = meshPart.material->Emissive;

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

