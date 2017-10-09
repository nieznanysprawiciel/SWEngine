/**@file DisplayEngine.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera definicje metod klasy DispalyEngine.
*/
#include "EngineCore/stdafx.h"

#include "DisplayEngine.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/ModelsManager/AssetsManager.h"
#include "EngineCore/ModelsManager/DefaultAssets.h"
#include "EngineCore/EventsManager/Events/RenderOnceEndedEvent.h"

#include "EngineCore/EngineHelpers/PerformanceCheck.h"
#include "EngineCore/EngineHelpers/ActorsCommonFunctions.h"
#include "swGraphicAPI/Resources/ResourcesFactory.h"

#include "EngineCore/DisplayEngine/RenderPasses/RenderingHelpers.h"

#include "swCommonLib/Common/MemoryLeaks.h"


namespace sw
{



using namespace DirectX;

const wchar_t CONSTANT_PER_FRAME_BUFFER_NAME[] = L"::DisplayEngine::ConstantPerFrameBuffer";
const wchar_t CONSTANT_PER_MESH_BUFFER_NAME[] = L"::DisplayEngine::ConstantPerMeshBuffer";

const wchar_t CAMERA_CONSTANTS_BUFFER_NAME[] = L"::DisplayEngine::CameraConstants";
const wchar_t TRANSFORM_CONSTANTS_BUFFER_NAME[] = L"::DisplayEngine::TransformConstants";
const wchar_t MATERIAL_CONSTANTS_BUFFER_NAME[] = L"::DisplayEngine::MaterialConstants";		///< @todo Temporary. Use constants from MaterialAsset.




/**@brief Sets AssetManager for IRenderPass class.*/
void		SetAssetManager		( AssetsManager* manager )
{
	IRenderPass::s_assetsManager = manager;
}

void		SetFactory			( RenderPassFactory* factory )
{
	IRenderPass::s_renderPassFactory = factory;
}


// ================================ //
//
DisplayEngine::DisplayEngine( Engine* engine )
	: engine( engine )
{
	lightModule = new LightModule();

	m_defaultCamera = nullptr;
	m_currentCamera = nullptr;
	m_skyDome = nullptr;
	m_mainRenderTarget = nullptr;

	m_maxQueuedPassesPerFrame = 5;
}


DisplayEngine::~DisplayEngine()
{
	// gui::RenderingSystem owns.
	m_renderers.clear();

	if( m_skyDome )
		delete m_skyDome;

	delete m_defaultCamera;
	delete lightModule;
}

/**@brief Inicjuje renderer (lub kilka je¿eli renderujemy wielow¹tkowo).
Poza tym inicjuje bufory sta³ych. Przy okazji inicjuje te¿ stany z-bufora do szybkiego
w³¹czania i wy³¹czania algorytmu.

@todo Zrobiæ inicjacjê wielow¹tkow¹. Gdzieœ musi zostaæ podjêta decyzja o liczbie w¹tków.
Trzeba pomyœleæ gdzie.*/
void			DisplayEngine::InitRenderer		( IRenderer* renderer )
{
	m_renderers.push_back( renderer );		// Na razie nie robimy deferred renderingu.
}

/**@brief Inits submodules (LightModule, main render pass), creates default engine buffers,
sets default camera and render target.*/
void			DisplayEngine::InitDisplayer	( AssetsManager* assetsManager, RenderTargetObject* mainRT )
{
	SetAssetManager( assetsManager );
	SetFactory( &m_passFactory );

	// Init submodules
	m_mainPass = m_passFactory.CreateDefaultLogic();
	lightModule->Init( assetsManager );


	// Init constant buffers
	ConstantBufferInitData cameraDataInit;
	cameraDataInit.DataType = TypeID::get< CameraConstants >();
	cameraDataInit.ElementSize = sizeof( CameraConstants );

	m_cameraConstants = assetsManager->CreateConstantsBuffer( CAMERA_CONSTANTS_BUFFER_NAME, cameraDataInit );
	assert( m_cameraConstants );

	ConstantBufferInitData transformInitData;
	transformInitData.ElementSize = sizeof( TransformConstants );
	transformInitData.DataType = TypeID::get< TransformConstants >();

	m_transformConstants = assetsManager->CreateConstantsBuffer( TRANSFORM_CONSTANTS_BUFFER_NAME, transformInitData );
	assert( m_transformConstants );

	ConstantBufferInitData materialInitData;	/// @todo Replace with MaterialAsset buffer.
	materialInitData.ElementSize = sizeof( ConstantPerMesh );
	materialInitData.DataType = TypeID::get< ConstantPerMesh >();

	m_materialConstants = assetsManager->CreateConstantsBuffer( MATERIAL_CONSTANTS_BUFFER_NAME, materialInitData );
	assert( m_materialConstants );


	SetMainRenderTarget( mainRT );

	m_defaultCamera = new CameraActor();
	SetCurrentCamera( m_defaultCamera );
}

// ================================ //
//
void			DisplayEngine::BeginScene()
{
	//m_renderers[ 0 ]->BeginScene( m_mainRenderTarget );
}

// ================================ //
//
void			DisplayEngine::EndScene()
{
	//m_renderers[ 0 ]->Present();
	//m_mainSwapChain->Present( 0 );
}

// ================================ //
//
void			DisplayEngine::SetMainRenderTarget( RenderTargetObject* renderTarget )
{
	m_mainRenderTarget = renderTarget;
	m_mainPass->SetMainRenderTarget( renderTarget );
}


//-------------------------------------------------------------------------------//
//							Funkcje pomocnicze do renderingu
//-------------------------------------------------------------------------------//

// ================================ //
//
RenderContext		DisplayEngine::CreateRenderContext	( float timeInterval, float timeLag )
{
	RenderContext context( meshes, m_interpolatedMatricies );
	context.DisplayEngine = this;
	context.CameraBuffer = m_cameraConstants.Ptr();
	context.TransformBuffer = m_transformConstants.Ptr();
	context.LightBuffer = lightModule->GetLightBuffer();
	context.MaterialConstants = m_materialConstants.Ptr();
	context.Layout = defaultLayout;
	context.LightModule = lightModule;
	context.TimeInterval = timeInterval;
	context.TimeLag = timeLag;

	return context;
}


//-------------------------------------------------------------------------------//
//							W³aœciwe funkcje do renderingu
//-------------------------------------------------------------------------------//

/**@brief Funkcja do renderowania sceny

Poniewa¿ klasa UIEngine tak¿e wykonuje renderowanie( wyœwietla elementy interfejsu graficznego ),
funkcja displayEngine nie mo¿e sama wywo³ywaæ  BeginScene( ) i EndScene( ) z bilbioteki directX, aby nie
by³o podwojnego wywo³ania. Z tego wzglêdu powy¿sze funkcje s¹ wywo³ywane zewnetrznie w pêtli g³ównej przez klasê Engine.

Funkcja displayEngine ma obowi¹zek za ka¿dym razem od nowa ustawiæ macierz widoku i projekcji, poniewa¿
mog¹ byæ zmodyfikowane przez UIEngine.Innymi s³owy nie mo¿na za³o¿yæ, ¿e jak siê raz ustawi³o macierze,
to przy nastêpnym wywo³aniu bêd¹ takie same.

@param[in] timeInterval Czas od ostatniej klatki. Przy ustawionej sta³ej @ref FIXED_FRAMES_COUNT czas ten jest sta³y
i wynosi tyle ile wartoœæ sta³ej FIXED_MOVE_UPDATE_INTERVAL.
@param[in] timeLag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void			DisplayEngine::DisplayScene				( TimeType time, TimeDiff elapsed, TimeRatio timeLag )
{
	if( m_mainRenderTarget )
	{
		IRenderer* renderer = m_renderers[ 0 ];		///<@todo Docelowo ma to dzia³aæ wielow¹tkowo i wybieraæ jeden z rendererów.

		ProcessMainPass( (float)elapsed, (float)timeLag );
	}
}

/**@brief Renders scene using @ref m_mainPass.*/
void			DisplayEngine::ProcessMainPass			( float timeInterval, float timeLag )
{
	IRenderer* renderer = m_renderers[ 0 ];
	RenderContext context = CreateRenderContext( timeInterval, timeLag );

	std::vector< Ptr< IRenderPass > > orderedPasses;
	m_mainPass->NestedPasses( orderedPasses );

	START_PERFORMANCE_CHECK( SHADING_PASSES );		///< @todo It's temporary. We should separate rendering of shadow, environment, custom passes, shading and postprocesisng.

	for( int i = 0; i < orderedPasses.size(); ++i )
	{
		auto& pass = orderedPasses[ i ];

		if( pass->PreRender( renderer, context ) )
		{
			pass->Render( renderer, context, 0, meshes.size() );
			pass->PostRender( renderer, context );
		}
	}

	END_PERFORMANCE_CHECK( SHADING_PASSES );

	DisplaySkyBox( timeInterval, timeLag, context );
}



/**@brief Funkcja s³u¿y do wyœwietlania skyboxa lub te¿ ka¿dej innej formy, która s³u¿y za t³o.

Funkcja ma specyficznym sposób przekazywania sta³ych do shaderów.
Z racji tego, ¿e kopu³ê nieba zawsze wyœwietlamy tak, jakbyœmy stali w œrodku kuli, standardowa
macierz widoku nie ma zbytniego zastosowania, bo zawiera równie¿ translacjê. Z tego powodu sam
obrót kamery jest umieszczany w macierzy œwiata. Oznacza to, ¿e nie ma mo¿liwoœci wykonywania
przekszta³ceñ na wierzcho³kach, musz¹ one byæ w tych pozycjach, w których s¹ w buforze.

Drug¹ spraw¹ jest to, ¿e w zasadzie œrednica kopu³y nie ma wiekszego znaczenia, poniewa¿ na czas renderowania
kopu³y, wy³¹czany jest algorytm z-bufora. Niebo zawsze ma siê znajdowaæ za wszystkimi obiektami, wiêc
dziêki temu kopu³a nie musi obejmowaæ ca³ej sceny.

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::DisplaySkyBox( float timeInterval, float timeLag, const RenderContext& ctx )
{
	START_PERFORMANCE_CHECK( SKYBOX_RENDERING )

		if( !m_skyDome )
			return;

	register IRenderer* renderer = m_renderers[ 0 ];		///<@todo Docelowo ma to dzia³aæ wielow¹tkowo i wybieraæ jeden z rendererów.


	// Aktualizuje bufor wierzcho³ków. Wstawiane s¹ nowe kolory.
	// Powinna byæ to raczej rzadka czynnoœæ, poniewa¿ aktualizacja jest kosztowna czasowo
	if( m_skyDome->NeedsBufferUpdate() )
		m_skyDome->UpdateBuffers( renderer );


	// Wyliczamy macierz transformacji
	XMVECTOR quaternion = m_currentCamera->GetInterpolatedOrientation( timeLag );
	inverse_camera_orientation( quaternion );

	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion( quaternion );

	// Fill transform constants buffer.
	TransformConstants meshTransformData;
	XMStoreFloat4x4( &meshTransformData.WorldMatrix, XMMatrixTranspose( rotationMatrix ) );
	XMStoreFloat4( &meshTransformData.MeshScale, XMVectorSetW( XMVectorReplicate( m_currentCamera->GetFarPlane() ), 1.0f ) );

	// Update only. Buffer is set by default.
	RenderingHelper::UpdateBuffer( renderer, ctx.TransformBuffer, meshTransformData );


	auto mesh = m_skyDome->GetModel();

	assert( mesh->GetSegmentsCount() > 0 );
	auto segment = mesh->GetSegment( 0 );


	SetRenderStateCommand renderCommand;
	renderCommand.TransformBuffer = ctx.TransformBuffer;
	renderCommand.MaterialBuffer = segment->Material->GetMaterialBuffer().Ptr();
	renderCommand.BonesTransforms = nullptr;
	renderCommand.VertexShader = segment->Material->GetVertexShader().Ptr();
	renderCommand.PixelShader = segment->Material->GetPixelShader().Ptr();

	for( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
	{
		renderCommand.Textures[ i ] = segment->Material->GetTexture( i ).Ptr();
		renderCommand.BindToShader[ i ] = (uint8)ShaderType::PixelShader;
	}

	renderer->SetShaderState( renderCommand );

	// Send draw command.
	DrawCommand drawCommand;
	drawCommand.BaseVertex = segment->BaseVertex;
	drawCommand.BufferOffset = segment->BufferOffset;
	drawCommand.NumVertices = segment->NumVertices;
	drawCommand.Topology = segment->Topology;
	drawCommand.VertexBuffer = mesh->GetVertexBufferRawPtr();
	drawCommand.IndexBufer = mesh->GetIndexBufferRawPtr();
	drawCommand.ExtendedIndex = segment->Flags & MeshPartFlags::Use4BytesIndex ? true : false;
	drawCommand.Layout = mesh->GetLayoutRawPtr();

	renderer->Draw( drawCommand );


	END_PERFORMANCE_CHECK( SKYBOX_RENDERING )
}


/**@brief Takes one RenderPass from queue and renders it.

Pass is rendered only once and removed.*/
void				DisplayEngine::RenderFromQueue		( float time_interval, float time_lag )
{

}


//=================================================================//
//					camera functions
//=================================================================//


/**@brief Dodaje obiekt, który ma zostaæ wyœwietlony.

@param[in] object ActorBase, który ma zostaæ dopisany do tablic wyœwietlania.*/
void				DisplayEngine::AddMeshObject( StaticActor* object )
{
	realocate_interpolation_memory();		//powiêkszamy tablicê macierzy interpolacji
							//wykona siê tylko je¿eli jest konieczne
	meshes.push_back( object );
}

/**@brief Usuwa aktora z modu³u.

Funkcja przegl¹da aktorów od ty³u, poniewa¿ bardziej prawdopodobne jest,
¿e usuwamy aktora stworzonego niedawno.*/
void				DisplayEngine::RemoveActor			( ActorBase* actor )
{
	ActorsCommonFunctions::RemoveActor( meshes, static_cast<StaticActor*>( actor ) );
	ActorsCommonFunctions::RemoveActor( cameras, static_cast<CameraActor*>( actor ) );
	lightModule->RemoveActor( actor );

	if( m_currentCamera == actor )
		SetCurrentCamera( m_defaultCamera );
}

/**@brief Kasuje wszystkich aktorów.*/
void				DisplayEngine::RemoveAllActors		()
{
	meshes.clear();
	cameras.clear();
	lightModule->RemoveAllActors();
	SetCurrentCamera( m_defaultCamera );
}

/**@brief Kasuje wszystkie meshe. Pomijane s¹ kamery.*/
void				DisplayEngine::DeleteAllMeshes		()
{
	meshes.clear();
}


/**@brief Dodaje kamerê do spisu kamer w silniku.

@param[in] camera Kamera do dodania.
@return
Funkcja zwraca 0 w przypadku powodzenia.
Je¿eli kamera ju¿ istnia³a wczesniej, to zwracan¹ wartoœci¹ jest 1.
Je¿eli podano wskaŸnik nullptr, zwrócona zostanie wartoœæ 2.*/
int					DisplayEngine::AddCamera			( CameraActor* camera )
{
	if( camera == nullptr )
		return 2;
	for( unsigned int i = 0; i < cameras.size(); ++i )
		if( cameras[ i ] == camera )
			return 1;	//kamera ju¿ istnieje

	cameras.push_back( camera );
	return 0;
}

/**@brief Ustawia aktualnie u¿ywan¹ kamerê.
@param[in] camera Kamera do ustawienia
@return 0 w przypadku powodzenia, 1 je¿eli kamera by³a nullptrem.
Zasadniczo nie ma po co sprawdzaæ wartoœci zwracanej.*/
int					DisplayEngine::SetCurrentCamera		( CameraActor* camera )
{
	if( camera == nullptr )
		return 1;

	m_currentCamera = camera;
	m_mainPass->SetMainCamera( camera );

	return 0;
}

/**@brief Zwraca aktualnie ustawion¹ kamerê g³ówn¹.*/
CameraActor*		DisplayEngine::GetCurrentCamera		()
{
	return m_currentCamera;
}

//=================================================================//
//					interpolation
//=================================================================//

/**@brief Powiêkszamy tablicê macierzy, w których umieszcza siê wyniki interpolacji
po³o¿eñ obiektów tu¿ przed ich wyœwietleniem w kolejnej klatce animacji.

W parametrze nale¿y podaæ minimaln¹ liczbê macierzy o jak¹ siê ma zwiêkszyæ dotychczasowa
tablica.

Nie ma potrzeby przepisywania danych ze starej tablicy nowoutworzonej.
Wyniki s¹ niepotrzebne po ka¿dym wyœwietleniu klatki, a iloœæ obiektów
w silniku nie mo¿e siê zwiêkszyæ miêdzy interpolacj¹, a wyœwietleniem.
@param[in] min Minimalna liczba macierzy o jak¹ nale¿y zwiekszyæ tablicê.*/
void				DisplayEngine::realocate_interpolation_memory	( unsigned int min )
{
	if( m_interpolatedMatricies.size() < min + meshes.size() )
	{
		while( m_interpolatedMatricies.size() < min + meshes.size() )
			m_interpolatedMatricies.resize( 2 * ( m_interpolatedMatricies.size() + 1 ) );	//wielkoœæ tablicy roœnie wyk³adniczo
	}
}

/**@brief Funkcja wykonuje interpolacjê po³o¿eñ dla wszystkich obiektów dynamicznych
w silniku.

Obiekty s¹ one przesuwane o tak¹ odleg³oœæ jaka wynika z ich prêdkoœci
oraz ze zmiennej time_lag, która oznacza czas, jaki up³yn¹³ od ostatniego
przeliczenia klatki.

Pozycjê z ostatniego wyliczenia klatki znajduj¹ siê w zmiennych position
i orientation obiektów dynamicznych. Docelowe macierze przekszta³ceñ obiektów
zostan¹ umieszczone w tablicy m_interpolatedMatricies, w których indeks elementu
odpowiada indeksom w tablicy meshes.

@todo [docelowo do wykonania wielow¹tkowego]

@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].*/
void				DisplayEngine::InterpolatePositions				( TimeRatio timeLag )
{
	for( unsigned int i = 0; i < meshes.size(); ++i )
	{
		StaticActor* object = meshes[ i ];
		interpolate_object2( (float)timeLag, object, &( m_interpolatedMatricies[ i ] ) );
	}
}


/**@brief Funkcja tworzy macierz przekszta³cenia dla podanego obiektu, interpoluj¹c jego pozycjê
z prêdkoœci postêpowej i k¹towej.

@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
@param[in] object Objekt, dla którego liczymy macierz przekszta³cenia.
@param[out] transform_matrix Zmienna, w której zostanie umieszczona interpolowana macierz przekszta³cenia.
*/
void				DisplayEngine::interpolate_object2				( float time_lag, const StaticActor* object, DirectX::XMFLOAT4X4* result_matrix )
{
	XMVECTOR position = object->GetInterpolatedPosition( time_lag );
	XMVECTOR orientation = object->GetInterpolatedOrientation( time_lag );

	XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
	transformation = transformation * XMMatrixTranslationFromVector( position );

	XMStoreFloat4x4( result_matrix, transformation );
}

//=================================================================//
//					light functions
//=================================================================//

// ================================ //
//
LightModule*		DisplayEngine::GetLightModule					()
{
	return lightModule;
}

/**@brief Ustawia nowego skydome'a dla gry.

Nale¿y podaæ w pe³ni skonstruowanego i gotowego do wyœwietlania SkyDome'a.
Funkcja zwraca poprzednio ustawionego SkyDome'a, którego nale¿y zwolniæ samemu.
Aktualnie ustawiony SkyDome jest pod koniec programu zwalniany w destruktorze.

@note Je¿eli t³o ma nie byæ wyœwietlane nale¿y podaæ w parametrze nullptr.

@param[in] dome Nowy SkyDome, który ma zostaæ ustawiony.
@return Zwraca poprzedniego SkyDome'a.*/
SkyDome*			DisplayEngine::SetSkydome						( SkyDome* dome )
{
	SkyDome* old = m_skyDome;
	m_skyDome = dome;
	return old;
}


}	// sw

