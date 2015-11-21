#include "EngineEditor/LightmapsTool/stdafx.h"
#include "EngineEditor/LightmapsTool/LightmapLogic.h"

#include "EngineCore/UIEngine/InputAbstractionLayer.h"
#include "EngineCore/DisplayEngine/RenderPass.h"
#include "EngineCore/ModelsManager/DefaultAssets.h"

#include "LightmapWorkers/LightmapWorkerCPU.h"

#include <limits>
#undef max

#include "Common/memory_leaks.h"

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


/**@brief Przygotowuje scenê.*/
int LightmapLogic::LoadLevel					()
{
	auto layer = m_engine->input.GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS1 );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS2 );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS3 );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS4 );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS5 );
	layer->demand_down_event( STANDARD_LAYERS::PROTOTYPE_BUTTONS::LOAD_LIGHTMAP_SCENE );

	// Wczytujemy shadery na przysz³oœæ.
	m_engine->assets.shaders.LoadVertexShaderSync( DEFAULT_COORD_COLOR_VERTEX_SHADER_PATH, &m_layout, DefaultAssets::LAYOUT_COORD_COLOR );
	m_engine->assets.shaders.LoadPixelShaderSync( DEFAULT_COORD_COLOR_PIXEL_SHADER_PATH );
	m_engine->assets.shaders.LoadPixelShaderSync( DEFAULT_LIGHTMAP_PIXEL_SHADER_PATH );
	m_layout->AddObjectReference();


	EventDelegate genLightmap, renderEnded;
	genLightmap.bind( this, &LightmapLogic::GenerateLightmaps );
	renderEnded.bind( this, &LightmapLogic::RenderEnded );
	m_fableEngine->ChangeDelegate( (unsigned int)EventType::KeyDownEvent, genLightmap );
	m_fableEngine->ChangeDelegate( (unsigned int)EventType::RenderOnceEndedEvent, renderEnded );

	// Przygotowanie sceny
	const wchar_t room1ModelString[] = L"levels/Room1/Room1.FBX";
	Model3DFromFile* room1Model = m_engine->assets.models.LoadSync( room1ModelString );

	DynamicMeshObject* room1Object = new DynamicMeshObject;
	DirectX::XMVECTOR position = DirectX::XMVectorSet( 0.0, -300.0, -2000.0, 0.0 );
	room1Object->Teleport( position );

	room1Object->SetModel( room1Model );
	m_engine->actors.AddDynamicMesh( room1Object );


	return 0;
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
			m_lightmapThread->join();		// Czekamy a¿ bêdzie pewne, ¿e w¹tek siê zakoñczy³

			// Zabieramy potrzebne dane i kasujemy obiekty.
			auto sceneData = m_lightmapWorker->GetSceneData();
			sceneData->verticies.clear();
			sceneData->buffers.clear();
			auto result = m_lightmapWorker->MoveResult();

			delete m_lightmapThread;
			delete m_lightmapWorker;
			m_lightmapWorker = nullptr;
			m_lightmapThread = nullptr;

			// Zapamiêtujemy jakie meshe powinny póŸniej dostaæ swoje tekstury. Pomijamy wielokrotne wyst¹pienia.
			// Poszczególne czêœci mesha s¹ u³o¿one ci¹giem w pamiêci.
			DynamicMeshObject* currentObject = nullptr;;
			for( auto meshPart : sceneData->objectParts )
				if( currentObject != meshPart.object )
				{
					m_meshes.push_back( meshPart.object );
					currentObject = meshPart.object;
				}

			delete sceneData;		// Nie bêdzie ju¿ wiêcej potrzebne.

			// Przygotowujemy dane takie same dla wszyskich obiektów.
			VertexShaderObject* vertexShader = m_engine->assets.shaders.GetVertexShaderSync( DEFAULT_COORD_COLOR_VERTEX_SHADER_PATH );
			PixelShaderObject* pixelShader = m_engine->assets.shaders.GetPixelShaderSync( DEFAULT_COORD_COLOR_PIXEL_SHADER_PATH );
			RenderTargetDescriptor renderTargetDesc;
			renderTargetDesc.textureHeight = 512;
			renderTargetDesc.textureWidth = 512;
			renderTargetDesc.textureType = TextureType::TEXTURE_TYPE_TEXTURE2D;
			renderTargetDesc.colorBuffFormat = ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UNORM_SRGB;
			renderTargetDesc.depthStencilFormat = DepthStencilFormat::DEPTH_STENCIL_FORMAT_D16_UNORM;
			
			std::wstring renderTargetName = L"::LightmapGen";
			uint32 counter = 0;

			// Tworzymy bufory wynikowe.
			for( auto& chunk : result )
			{
				std::wstring fullName = renderTargetName + std::to_wstring( counter );
				auto vertexBuff = m_engine->assets.buffers.CreateVertexBufferSync( fullName, chunk, chunk.GetMemorySize() / sizeof( CoordColor ) );

				RenderPass* renderPass = new RenderPass;
				DynamicMeshObject* dynamicMesh = new DynamicMeshObject( vertexBuff, nullptr );
				RenderTargetObject* renderTarget = m_engine->assets.renderTargets.CreateSync( fullName, renderTargetDesc );
				
				// Build mesh
				ModelPart part;
				part.material = m_engine->assets.materials.GetSync( DEFAULT_MATERIAL_STRING );
				part.pixel_shader = pixelShader;
				part.vertex_shader = vertexShader;
				part.mesh = new MeshPartObject;
				part.mesh->vertices_count = chunk.GetMemorySize() / sizeof( CoordColor );

				dynamicMesh->AddModelPart( part );

				// Build Pass information
				renderPass->AddMesh( dynamicMesh );
				renderPass->SetRenderTarget( renderTarget );
				renderPass->SetLayout( m_layout );

				m_engine->rendering.RenderOnce( renderPass );
				m_lightmapBuffers.push_back( dynamicMesh );
				counter++;
			}

			m_lightmapState = LightmapState::RenderingLightmaps;
		}
	}
}

/**@brief Event jest wysy³any po wyrenderowaniu lightmapy.

Funkcja zbiera kolejne lightmapy i wstawia do odpowiednich obiektów.
Ponadto kasuje obiekty stworzone do jednorazowego renderowania.*/
void LightmapLogic::RenderEnded( Event* renderEndedEvent )
{
	if( renderEndedEvent->type == (unsigned int)EventType::RenderOnceEndedEvent )
	{
		RenderOnceEndedEvent* castedEvent = static_cast<RenderOnceEndedEvent*>( renderEndedEvent );
		RenderPass* renderPass = castedEvent->renderPass;

		auto meshes = renderPass->GetMeshes();
		DynamicMeshObject* realMesh = nullptr;

		for( unsigned int i = 0; i < m_lightmapBuffers.size(); ++i )
		{
			if( m_lightmapBuffers[i] == meshes[0] )
			{
				realMesh = m_meshes[ i ];
				m_meshes.erase( m_meshes.begin() + i );
				m_lightmapBuffers.erase( m_lightmapBuffers.begin() + i );
				break;
			}
		}

		if( realMesh )
		{
			PixelShaderObject* pixelShader = m_engine->assets.shaders.GetPixelShaderSync( DEFAULT_LIGHTMAP_PIXEL_SHADER_PATH );
			TextureObject* lightmap = renderPass->GetRenderTarget()->GetColorBuffer();
			m_engine->actors.assets.ChangeTextures( realMesh, lightmap, 0, std::numeric_limits<uint16>::max(), TextureUse::TEX_LIGHTMAP );
			m_engine->actors.assets.ChangePixelShaders( realMesh, pixelShader, 0, std::numeric_limits<uint16>::max() );
		}
		
		for( auto mesh : meshes )
		{
			auto& modelParts = mesh->GetModelParts();
			for( auto& part : modelParts )
				delete part.mesh;
			delete mesh;
		}
		delete renderPass;

		if( m_meshes.size() == 0 )
			// Wszystkie lightmapy skoñczone
			m_lightmapState = LightmapState::ReadyToGenerate;
	}
}

/**@brief Funkcja wywo³ywana w momencie zmiany levelu na inny.*/
int LightmapLogic::UnloadLevel					()
{	
	m_layout->DeleteObjectReference();
	m_fableEngine->DeleteDelegate( (unsigned int)EventType::KeyDownEvent );		// Po zmianie klasy IGameLogic na inn¹, ktoœ móg³by nadal próbowaæ wywo³aæ nieistniej¹cy kawa³ek kodu.
	m_fableEngine->DeleteDelegate( (unsigned int)EventType::RenderOnceEndedEvent );
	return 0;
}


/**@brief Wywo³uje generowanie lightmap w silniku.

Funkcja pobiera obiekty statyczne ze sceny.
Poniewa¿ mechanizm eventów w silniku nie pozwala na razie na podczepianie wielu funkcji
obs³ugi pod jeden event, to wywo³ywana jest te¿ funkcja LoadScene, je¿eli wciœniêto przycisk L.

@param[in] genLightmapEvent Event generowany po wciœniêciu odpowiedniego przycisku.
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

			// Tutaj wysy³amy dane do innych w¹tków, ¿eby liczy³y wszystko w tle.
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

/**Zbiera dane o obiektach na scenie i przygotowuje do wys³ania do LightmapWorkera.
@return Zwraca dane sceny w strukturze SceneData.*/
SceneData* LightmapLogic::PrepareSceneData			()
{
	std::vector<DynamicMeshObject*> meshes = m_engine->actors.GetSceneObjects();
	SceneData* sceneData = new SceneData;

	for( auto mesh : meshes )
	{
		auto meshData = mesh->GetModelParts();
		auto vertexBuff = mesh->GetVertexBuffer();

		DirectX::XMVECTOR objectPos = mesh->GetPosition();
		DirectX::XMVECTOR objectRot = mesh->GetOrientation();		// Quaternion orientation
		// Zak³adam, ¿e nie obs³ugujemy skalowania, bo na razie nie wiem czy silnik bêdzie je obs³ugiwa³ czy to bêdzie gdzieœ prekalkulowane.
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

/**@brief Obs³uga wciœniecia przycisku do ³adowania sceny.

@todo Tutaj póŸniej znajdzie siê kod do ³adowania jakiejœ sceny, albo nawet kilku.
Byæ mo¿e trzeba bêdzie zrobiæ jakis plik konfiguracyjny do wczytywania, ale
w wersji pocz¹tkowej mo¿e byæ wszystko tworzone z kodu.*/
void LightmapLogic::LoadScene					( Event* keyEvent )
{

}

