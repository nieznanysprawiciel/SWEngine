#include "EngineEditor/LightmapsTool/stdafx.h"
#include "EngineEditor/LightmapsTool/LightmapLogic.h"

#include "EngineCore/UIEngine/InputAbstractionLayer.h"
#include "EngineCore/DisplayEngine/RenderPass.h"
#include "EngineCore/ModelsManager/DefaultAssets.h"

#include "EngineCore/EventsManager/Events/RenderOnceEndedEvent.h"
#include "EngineCore/EventsManager/Events/KeyDownEvent.h"

#include "LightmapWorkers/LightmapWorkerCPU.h"

#include <limits>
#undef max

#include "Common/MemoryLeaks.h"

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
	//auto layer = m_engine->Input.GetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS );
	//layer->DemandDownEvent( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS1 );
	//layer->DemandDownEvent( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS2 );
	//layer->DemandDownEvent( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS3 );
	//layer->DemandDownEvent( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS4 );
	//layer->DemandDownEvent( STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS5 );
	//layer->DemandDownEvent( STANDARD_LAYERS::PROTOTYPE_BUTTONS::LOAD_LIGHTMAP_SCENE );

	//// Wczytujemy shadery na przysz³oœæ.
	//m_engine->Assets.Shaders.LoadVertexShaderSync( DefaultAssets::DEFAULT_COORD_COLOR_VERTEX_SHADER_PATH, &m_layout, DefaultAssets::LAYOUT_COORD_COLOR );
	//m_engine->Assets.Shaders.LoadPixelShaderSync( DefaultAssets::DEFAULT_COORD_COLOR_PIXEL_SHADER_PATH );
	//m_engine->Assets.Shaders.LoadPixelShaderSync( DefaultAssets::DEFAULT_LIGHTMAP_PIXEL_SHADER_PATH );
	//m_layout->AddObjectReference();


	////EventDelegateTmp genLightmap, renderEnded;
	////genLightmap.bind( this, &LightmapLogic::GenerateLightmaps );
	////renderEnded.bind( this, &LightmapLogic::RenderEnded );
	////m_fableEngine->ChangeDelegate( (unsigned int)EventType::KeyDownEvent, genLightmap );
	////m_fableEngine->ChangeDelegate( (unsigned int)EventType::RenderOnceEndedEvent, renderEnded );
	//m_engine->Actors.Communication.AddListenerDelayed< RenderOnceEndedEvent, LightmapLogic >( this, &LightmapLogic::RenderEnded );
	//m_engine->Actors.Communication.AddListenerDelayed< KeyDownEvent, LightmapLogic >( this, &LightmapLogic::GenerateLightmaps );

	//// Przygotowanie sceny
	//const wchar_t room1ModelString[] = L"levels/Room1/Room1.FBX";
	//const wchar_t markerModelString[] = L"levels/Room1/Marker.FBX";
	//Model3DFromFile* room1Model = m_engine->Assets.Models.LoadSync( room1ModelString );
	//Model3DFromFile* markerModel = m_engine->Assets.Models.LoadSync( markerModelString );

	//StaticActor* room1Object = new StaticActor;
	//room1Object->Teleport( DirectX::XMVectorSet( 0.0, -300.0, -2000.0, 0.0 ) );
	////room1Object->TeleportOrientation( DirectX::XMQuaternionRotationNormal( DirectX::XMVectorSet( 0.0, 1.0, 0.0, 0.0 ), DirectX::XM_PIDIV2 ) );

	//room1Object->SetModel( room1Model );
	//m_engine->Actors.AddDynamicMesh( room1Object );

	//// Marker
	//StaticActor* markerObject = new StaticActor;
	//markerObject->Teleport( DirectX::XMVectorSet( 1559.0, 700.0, -1688.0, 0.0 ) );

	//markerObject->SetModel( markerModel );
	//m_engine->Actors.AddDynamicMesh( markerObject );

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
		//// Sprawdzamy stan LightmapWorkera
		//if( m_lightmapWorker->GetState() == LightmapState::GenerationEnded )
		//{
		//	m_lightmapThread->join();		// Czekamy a¿ bêdzie pewne, ¿e w¹tek siê zakoñczy³

		//	// Zabieramy potrzebne dane i kasujemy obiekty.
		//	auto sceneData = m_lightmapWorker->GetSceneData();
		//	sceneData->verticies.clear();
		//	sceneData->buffers.clear();
		//	auto result = m_lightmapWorker->MoveResult();

		//	delete m_lightmapThread;
		//	delete m_lightmapWorker;
		//	m_lightmapWorker = nullptr;
		//	m_lightmapThread = nullptr;

		//	// Zapamiêtujemy jakie meshe powinny póŸniej dostaæ swoje tekstury. Pomijamy wielokrotne wyst¹pienia.
		//	// Poszczególne czêœci mesha s¹ u³o¿one ci¹giem w pamiêci.
		//	StaticActor* currentObject = nullptr;;
		//	for( auto meshPart : sceneData->objectParts )
		//	{
		//		if( currentObject != meshPart.object )
		//		{
		//			m_meshes.push_back( meshPart.object );
		//			currentObject = meshPart.object;
		//		}
		//	}

		//	delete sceneData;		// Nie bêdzie ju¿ wiêcej potrzebne.

		//	// Przygotowujemy dane takie same dla wszyskich obiektów.
		//	VertexShader* vertexShader = m_engine->Assets.Shaders.GetVertexShaderSync( DefaultAssets::DEFAULT_COORD_COLOR_VERTEX_SHADER_PATH );
		//	PixelShader* pixelShader = m_engine->Assets.Shaders.GetPixelShaderSync( DefaultAssets::DEFAULT_COORD_COLOR_PIXEL_SHADER_PATH );
		//	RenderTargetDescriptor renderTargetDesc;
		//	renderTargetDesc.TextureHeight = 1024;
		//	renderTargetDesc.TextureWidth = 1024;
		//	renderTargetDesc.TextureType = TextureType::TEXTURE_TYPE_TEXTURE2D;
		//	renderTargetDesc.ColorBuffFormat = ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UNORM;
		//	renderTargetDesc.DepthStencilFormat = DepthStencilFormat::DEPTH_STENCIL_FORMAT_D16_UNORM;
		//	
		//	std::wstring renderTargetName = L"::LightmapGen";
		//	uint32 counter = 0;

		//	// Tworzymy bufory wynikowe.
		//	for( auto& chunk : result )
		//	{
		//		std::wstring fullName = renderTargetName + std::to_wstring( counter );
		//		auto vertexBuff = m_engine->Assets.Buffers.CreateVertexBufferSync( fullName, chunk, chunk.GetMemorySize() / sizeof( CoordColor ) );

		//		RenderPassDepracated* renderPass = new RenderPassDepracated;
		//		StaticActor* dynamicMesh = new StaticActor( vertexBuff.Ptr(), nullptr );
		//		RenderTargetObject* renderTarget = m_engine->Assets.RenderTargets.CreateSync( fullName, renderTargetDesc );
		//		
		//		// Build mesh
		//		ModelPart part;
		//		part.material = m_engine->Assets.Materials.GetSync( DefaultAssets::DEFAULT_MATERIAL_STRING );
		//		part.pixel_shader = pixelShader;
		//		part.vertex_shader = vertexShader;
		//		part.mesh = new MeshPartObject;
		//		part.mesh->vertices_count = chunk.GetMemorySize() / sizeof( CoordColor );

		//		dynamicMesh->AddModelPart( part );

		//		// Build Pass information
		//		renderPass->AddMesh( dynamicMesh );
		//		renderPass->SetRenderTarget( renderTarget );
		//		renderPass->SetLayout( m_layout );

		//		m_engine->Rendering.RenderOnce( renderPass );
		//		m_lightmapBuffers.push_back( dynamicMesh );
		//		counter++;
		//	}

		//	m_lightmapState = LightmapState::RenderingLightmaps;
		//}
	}
}

/**@brief Event jest wysy³any po wyrenderowaniu lightmapy.

Funkcja zbiera kolejne lightmapy i wstawia do odpowiednich obiektów.
Ponadto kasuje obiekty stworzone do jednorazowego renderowania.*/
void LightmapLogic::RenderEnded( const EngineObject* sender, Event* renderEndedEvent )
{
	if( renderEndedEvent->type == (unsigned int)EventType::RenderOnceEndedEvent )
	{
		//RenderOnceEndedEvent* castedEvent = static_cast<RenderOnceEndedEvent*>( renderEndedEvent );
		//RenderPassDepracated* renderPass = castedEvent->renderPass;

		//auto meshes = renderPass->GetMeshes();
		//StaticActor* realMesh = nullptr;

		//for( unsigned int i = 0; i < m_lightmapBuffers.size(); ++i )
		//{
		//	if( m_lightmapBuffers[i] == meshes[0] )
		//	{
		//		realMesh = m_meshes[ i ];
		//		m_meshes.erase( m_meshes.begin() + i );
		//		m_lightmapBuffers.erase( m_lightmapBuffers.begin() + i );
		//		break;
		//	}
		//}

		//if( realMesh )
		//{
		//	PixelShader* pixelShader = m_engine->Assets.Shaders.GetPixelShaderSync( DefaultAssets::DEFAULT_LIGHTMAP_PIXEL_SHADER_PATH );
		//	TextureObject* lightmap = renderPass->GetRenderTarget()->GetColorBuffer();
		//	m_engine->Actors.Assets.ChangeTextures( realMesh, lightmap, 0, std::numeric_limits<uint16>::max(), TextureUse::TEX_DIFFUSE );	// W przysz³oœci TEX_LIGHTMAP
		//	m_engine->Actors.Assets.ChangePixelShaders( realMesh, pixelShader, 0, std::numeric_limits<uint16>::max() );
		//}
		//
		//for( auto mesh : meshes )
		//{
		//	auto& modelParts = mesh->GetModelParts();
		//	for( auto& part : modelParts )
		//		delete part.mesh;
		//	delete mesh;
		//}
		//delete renderPass;

		//if( m_meshes.size() == 0 )
		//	// Wszystkie lightmapy skoñczone
		//	m_lightmapState = LightmapState::ReadyToGenerate;
	}
}

/**@brief Funkcja wywo³ywana w momencie zmiany levelu na inny.*/
int LightmapLogic::UnloadLevel					()
{	
	m_layout->DeleteObjectReference();
	m_engine->Actors.Communication.RemoveListenerDelayed< KeyDownEvent >( this );
	m_engine->Actors.Communication.RemoveListenerDelayed< RenderOnceEndedEvent >( this );

	return 0;
}


/**@brief Wywo³uje generowanie lightmap w silniku.

Funkcja pobiera obiekty statyczne ze sceny.
Poniewa¿ mechanizm eventów w silniku nie pozwala na razie na podczepianie wielu funkcji
obs³ugi pod jeden event, to wywo³ywana jest te¿ funkcja LoadScene, je¿eli wciœniêto przycisk L.

@param[in] genLightmapEvent Event generowany po wciœniêciu odpowiedniego przycisku.
*/
void LightmapLogic::GenerateLightmaps			( const EngineObject* sender, Event* keyEvent )
{
	if( m_lightmapState != LightmapState::ReadyToGenerate )
		return;

	if( keyEvent->type == (unsigned int)EventType::KeyDownEvent )
	{
		KeyDownEvent* genLightmapEvent = static_cast<KeyDownEvent*>( keyEvent );
		if( genLightmapEvent->VirtualIndex == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS1 ||
			genLightmapEvent->VirtualIndex == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS2 ||
			genLightmapEvent->VirtualIndex == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS3 ||
			genLightmapEvent->VirtualIndex == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS4 ||
			genLightmapEvent->VirtualIndex == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS5 )
		{
			SceneData* sceneData = PrepareSceneData();

			// Tutaj wysy³amy dane do innych w¹tków, ¿eby liczy³y wszystko w tle.
			if( genLightmapEvent->VirtualIndex == STANDARD_LAYERS::PROTOTYPE_BUTTONS::GENERATE_LIGHTMAPS1 )
			{
				LightmapWorkerCPU* worker = new LightmapWorkerCPU( sceneData );
				m_lightmapWorker = worker;
				m_lightmapThread = new std::thread( &LightmapWorkerCPU::Generate, std::ref( worker ) );
			}

			m_lightmapState = LightmapState::Generating;
		}
		else if( genLightmapEvent->VirtualIndex == STANDARD_LAYERS::PROTOTYPE_BUTTONS::LOAD_LIGHTMAP_SCENE )
			LoadScene( genLightmapEvent );
	}
}

/**Zbiera dane o obiektach na scenie i przygotowuje do wys³ania do LightmapWorkera.
@return Zwraca dane sceny w strukturze SceneData.*/
SceneData* LightmapLogic::PrepareSceneData			()
{
	std::vector<StaticActor*> meshes = m_engine->Actors.GetSceneObjects();
	SceneData* sceneData = new SceneData;

	//for( auto mesh : meshes )
	//{
	//	auto meshData = mesh->GetModelParts();
	//	auto vertexBuff = mesh->GetVertexBuffer();

	//	DirectX::XMVECTOR objectPos = mesh->GetPosition();
	//	DirectX::XMVECTOR objectRot = mesh->GetOrientation();		// Quaternion orientation
	//	// Zak³adam, ¿e nie obs³ugujemy skalowania, bo na razie nie wiem czy silnik bêdzie je obs³ugiwa³ czy to bêdzie gdzieœ prekalkulowane.
	//	DirectX::XMMATRIX objectTransform = DirectX::XMMatrixTranslationFromVector( objectPos );
	//	objectTransform = DirectX::XMMatrixMultiply( DirectX::XMMatrixRotationQuaternion( objectRot ), objectTransform );
	//	//objectTransform = DirectX::XMMatrixRotationQuaternion( objectRot ) * objectTransform;

	//	for( auto& meshPart : meshData )
	//	{
	//		SceneObjectData partData;
	//		partData.object = mesh;
	//		partData.bufferOffset = meshPart.mesh->buffer_offset;
	//		partData.verticesCount = meshPart.mesh->vertices_count;
	//		partData.diffuse = meshPart.material->Diffuse;
	//		partData.emissive = meshPart.material->Emissive;

	//		DirectX::XMMATRIX partTransform = DirectX::XMLoadFloat4x4( &meshPart.mesh->transform_matrix );
	//		//objectTransform = partTransform * objectTransform;
	//		partTransform = DirectX::XMMatrixMultiply( partTransform, objectTransform );
	//		DirectX::XMStoreFloat4x4( &partData.transform, partTransform );

	//		size_t index;
	//		if( FindInVector( sceneData->buffers, vertexBuff, index ) )
	//			partData.chunkIdx = index;
	//		else
	//		{
	//			sceneData->buffers.push_back( vertexBuff );
	//			MemoryChunk vertexChunk = vertexBuff->CopyData();
	//					
	//			sceneData->verticies.push_back( std::move( vertexChunk ) );
	//			partData.chunkIdx = sceneData->verticies.size() - 1;
	//		}

	//		sceneData->objectParts.push_back( partData );
	//	}
	//}

	return sceneData;
}

/**@brief Obs³uga wciœniecia przycisku do ³adowania sceny.

@todo Tutaj póŸniej znajdzie siê kod do ³adowania jakiejœ sceny, albo nawet kilku.
Byæ mo¿e trzeba bêdzie zrobiæ jakis plik konfiguracyjny do wczytywania, ale
w wersji pocz¹tkowej mo¿e byæ wszystko tworzone z kodu.*/
void LightmapLogic::LoadScene					( Event* keyEvent )
{

}

