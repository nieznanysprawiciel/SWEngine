/**@file DisplayEngine.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

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

/**@brief Inicjuje renderer (lub kilka je�eli renderujemy wielow�tkowo).
Poza tym inicjuje bufory sta�ych. Przy okazji inicjuje te� stany z-bufora do szybkiego
w��czania i wy��czania algorytmu.

@todo Zrobi� inicjacj� wielow�tkow�. Gdzie� musi zosta� podj�ta decyzja o liczbie w�tk�w.
Trzeba pomy�le� gdzie.*/
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
//							W�a�ciwe funkcje do renderingu
//-------------------------------------------------------------------------------//

/**@brief Funkcja do renderowania sceny

Poniewa� klasa UIEngine tak�e wykonuje renderowanie( wy�wietla elementy interfejsu graficznego ),
funkcja displayEngine nie mo�e sama wywo�ywa�  BeginScene( ) i EndScene( ) z bilbioteki directX, aby nie
by�o podwojnego wywo�ania. Z tego wzgl�du powy�sze funkcje s� wywo�ywane zewnetrznie w p�tli g��wnej przez klas� Engine.

Funkcja displayEngine ma obowi�zek za ka�dym razem od nowa ustawi� macierz widoku i projekcji, poniewa�
mog� by� zmodyfikowane przez UIEngine.Innymi s�owy nie mo�na za�o�y�, �e jak si� raz ustawi�o macierze,
to przy nast�pnym wywo�aniu b�d� takie same.

@param[in] timeInterval Czas od ostatniej klatki. Przy ustawionej sta�ej @ref FIXED_FRAMES_COUNT czas ten jest sta�y
i wynosi tyle ile warto�� sta�ej FIXED_MOVE_UPDATE_INTERVAL.
@param[in] timeLag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void			DisplayEngine::DisplayScene				( TimeType time, TimeDiff elapsed, TimeRatio timeLag )
{
	if( m_mainRenderTarget )
	{
		IRenderer* renderer = m_renderers[ 0 ];		///<@todo Docelowo ma to dzia�a� wielow�tkowo i wybiera� jeden z renderer�w.

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



/**@brief Funkcja s�u�y do wy�wietlania skyboxa lub te� ka�dej innej formy, kt�ra s�u�y za t�o.

Funkcja ma specyficznym spos�b przekazywania sta�ych do shader�w.
Z racji tego, �e kopu�� nieba zawsze wy�wietlamy tak, jakby�my stali w �rodku kuli, standardowa
macierz widoku nie ma zbytniego zastosowania, bo zawiera r�wnie� translacj�. Z tego powodu sam
obr�t kamery jest umieszczany w macierzy �wiata. Oznacza to, �e nie ma mo�liwo�ci wykonywania
przekszta�ce� na wierzcho�kach, musz� one by� w tych pozycjach, w kt�rych s� w buforze.

Drug� spraw� jest to, �e w zasadzie �rednica kopu�y nie ma wiekszego znaczenia, poniewa� na czas renderowania
kopu�y, wy��czany jest algorytm z-bufora. Niebo zawsze ma si� znajdowa� za wszystkimi obiektami, wi�c
dzi�ki temu kopu�a nie musi obejmowa� ca�ej sceny.

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::DisplaySkyBox( float timeInterval, float timeLag, const RenderContext& ctx )
{
	START_PERFORMANCE_CHECK( SKYBOX_RENDERING )

		if( !m_skyDome )
			return;

	register IRenderer* renderer = m_renderers[ 0 ];		///<@todo Docelowo ma to dzia�a� wielow�tkowo i wybiera� jeden z renderer�w.


	// Aktualizuje bufor wierzcho�k�w. Wstawiane s� nowe kolory.
	// Powinna by� to raczej rzadka czynno��, poniewa� aktualizacja jest kosztowna czasowo
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


/**@brief Dodaje obiekt, kt�ry ma zosta� wy�wietlony.

@param[in] object ActorBase, kt�ry ma zosta� dopisany do tablic wy�wietlania.*/
void				DisplayEngine::AddMeshObject( StaticActor* object )
{
	realocate_interpolation_memory();		//powi�kszamy tablic� macierzy interpolacji
							//wykona si� tylko je�eli jest konieczne
	meshes.push_back( object );
}

/**@brief Usuwa aktora z modu�u.

Funkcja przegl�da aktor�w od ty�u, poniewa� bardziej prawdopodobne jest,
�e usuwamy aktora stworzonego niedawno.*/
void				DisplayEngine::RemoveActor			( ActorBase* actor )
{
	ActorsCommonFunctions::RemoveActor( meshes, static_cast<StaticActor*>( actor ) );
	ActorsCommonFunctions::RemoveActor( cameras, static_cast<CameraActor*>( actor ) );
	lightModule->RemoveActor( actor );

	if( m_currentCamera == actor )
		SetCurrentCamera( m_defaultCamera );
}

/**@brief Kasuje wszystkich aktor�w.*/
void				DisplayEngine::RemoveAllActors		()
{
	meshes.clear();
	cameras.clear();
	lightModule->RemoveAllActors();
	SetCurrentCamera( m_defaultCamera );
}

/**@brief Kasuje wszystkie meshe. Pomijane s� kamery.*/
void				DisplayEngine::DeleteAllMeshes		()
{
	meshes.clear();
}


/**@brief Dodaje kamer� do spisu kamer w silniku.

@param[in] camera Kamera do dodania.
@return
Funkcja zwraca 0 w przypadku powodzenia.
Je�eli kamera ju� istnia�a wczesniej, to zwracan� warto�ci� jest 1.
Je�eli podano wska�nik nullptr, zwr�cona zostanie warto�� 2.*/
int					DisplayEngine::AddCamera			( CameraActor* camera )
{
	if( camera == nullptr )
		return 2;
	for( unsigned int i = 0; i < cameras.size(); ++i )
		if( cameras[ i ] == camera )
			return 1;	//kamera ju� istnieje

	cameras.push_back( camera );
	return 0;
}

/**@brief Ustawia aktualnie u�ywan� kamer�.
@param[in] camera Kamera do ustawienia
@return 0 w przypadku powodzenia, 1 je�eli kamera by�a nullptrem.
Zasadniczo nie ma po co sprawdza� warto�ci zwracanej.*/
int					DisplayEngine::SetCurrentCamera		( CameraActor* camera )
{
	if( camera == nullptr )
		return 1;

	m_currentCamera = camera;
	m_mainPass->SetMainCamera( camera );

	return 0;
}

/**@brief Zwraca aktualnie ustawion� kamer� g��wn�.*/
CameraActor*		DisplayEngine::GetCurrentCamera		()
{
	return m_currentCamera;
}

//=================================================================//
//					interpolation
//=================================================================//

/**@brief Powi�kszamy tablic� macierzy, w kt�rych umieszcza si� wyniki interpolacji
po�o�e� obiekt�w tu� przed ich wy�wietleniem w kolejnej klatce animacji.

W parametrze nale�y poda� minimaln� liczb� macierzy o jak� si� ma zwi�kszy� dotychczasowa
tablica.

Nie ma potrzeby przepisywania danych ze starej tablicy nowoutworzonej.
Wyniki s� niepotrzebne po ka�dym wy�wietleniu klatki, a ilo�� obiekt�w
w silniku nie mo�e si� zwi�kszy� mi�dzy interpolacj�, a wy�wietleniem.
@param[in] min Minimalna liczba macierzy o jak� nale�y zwiekszy� tablic�.*/
void				DisplayEngine::realocate_interpolation_memory	( unsigned int min )
{
	if( m_interpolatedMatricies.size() < min + meshes.size() )
	{
		while( m_interpolatedMatricies.size() < min + meshes.size() )
			m_interpolatedMatricies.resize( 2 * ( m_interpolatedMatricies.size() + 1 ) );	//wielko�� tablicy ro�nie wyk�adniczo
	}
}

/**@brief Funkcja wykonuje interpolacj� po�o�e� dla wszystkich obiekt�w dynamicznych
w silniku.

Obiekty s� one przesuwane o tak� odleg�o�� jaka wynika z ich pr�dko�ci
oraz ze zmiennej time_lag, kt�ra oznacza czas, jaki up�yn�� od ostatniego
przeliczenia klatki.

Pozycj� z ostatniego wyliczenia klatki znajduj� si� w zmiennych position
i orientation obiekt�w dynamicznych. Docelowe macierze przekszta�ce� obiekt�w
zostan� umieszczone w tablicy m_interpolatedMatricies, w kt�rych indeks elementu
odpowiada indeksom w tablicy meshes.

@todo [docelowo do wykonania wielow�tkowego]

@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].*/
void				DisplayEngine::InterpolatePositions				( TimeRatio timeLag )
{
	for( unsigned int i = 0; i < meshes.size(); ++i )
	{
		StaticActor* object = meshes[ i ];
		interpolate_object2( (float)timeLag, object, &( m_interpolatedMatricies[ i ] ) );
	}
}


/**@brief Funkcja tworzy macierz przekszta�cenia dla podanego obiektu, interpoluj�c jego pozycj�
z pr�dko�ci post�powej i k�towej.

@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
@param[in] object Objekt, dla kt�rego liczymy macierz przekszta�cenia.
@param[out] transform_matrix Zmienna, w kt�rej zostanie umieszczona interpolowana macierz przekszta�cenia.
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

Nale�y poda� w pe�ni skonstruowanego i gotowego do wy�wietlania SkyDome'a.
Funkcja zwraca poprzednio ustawionego SkyDome'a, kt�rego nale�y zwolni� samemu.
Aktualnie ustawiony SkyDome jest pod koniec programu zwalniany w destruktorze.

@note Je�eli t�o ma nie by� wy�wietlane nale�y poda� w parametrze nullptr.

@param[in] dome Nowy SkyDome, kt�ry ma zosta� ustawiony.
@return Zwraca poprzedniego SkyDome'a.*/
SkyDome*			DisplayEngine::SetSkydome						( SkyDome* dome )
{
	SkyDome* old = m_skyDome;
	m_skyDome = dome;
	return old;
}


}	// sw

