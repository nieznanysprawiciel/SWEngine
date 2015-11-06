#include "EngineEditor/LightmapsTool/stdafx.h"
#include "EngineEditor/LightmapsTool/LightmapLogic.h"
#include "EngineCore/UIEngine/InputAbstractionLayer.h"



struct SceneObjectData
{
	DynamicMeshObject*		object;				///< Obiekt, do kt�rego nale�y dana cz�� mesha. Potrzebujemy, �eby potem stworzy� tekstury i ustawi� dla odpowiednich obiekt�w.
	unsigned int			chunkIdx;			///< Wskazuje na obiekt z danymi wierzcho�k�w w strukturze SceneData.
	DirectX::XMFLOAT4X4		transform;			///< Macierz przekszta�cenia dla danej cz�ci mesha.
	unsigned int			bufferOffset;		///< Offset wzgl�dem pocz�tku bufora albo wierzcho�k�w.
	unsigned int			verticesCount;		///< Liczba wierzcho�k�w w danym kawa�ku mesha.
	DirectX::XMFLOAT4		diffuse;			///< Materia� - kana� diffuse, czyli jedyny, kt�ry bierzemy pod uwag�.
	DirectX::XMFLOAT4		emissive;			///< Je�eli wierzcho�ki opisuj� �r�d�o �wiat�a, to ten kana� powinien by� niezerowy.
};


struct SceneData
{
	std::vector<MemoryChunk>		verticies;	///< Bufory wierzcho�k�w. MemoryChunki s� unikatowe. Wiele obiekt�w mo�e si� odwo�ywa� do jednego chunka.
	std::vector<BufferObject*>		buffers;	///< Silnikowe obiekty bufor�w. Odpowiadaj� MemoryChunkom.
	std::vector<SceneObjectData>	objectParts;///< Obiekty s� przechowywane w cz�ciach, podzielonych po materia�ach.
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

@param[in] time Czas od w��czenia silnika.
*/
void LightmapLogic::ProceedGameLogic			( float time )
{

}

/**@brief Przygotowuje scen�.*/
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


/**@brief Wywo�uje generowanie lightmap w silniku.

Funkcja pobiera obiekty statyczne ze sceny.

@param[in] genLightmapEvent Event generowany po wci�ni�ciu odpowiedniego przycisku.
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

			// Tutaj wysy�amy dane do innych w�tk�w, �eby liczy�y wszystko w tle.
			// @todo

		}
	}
}

