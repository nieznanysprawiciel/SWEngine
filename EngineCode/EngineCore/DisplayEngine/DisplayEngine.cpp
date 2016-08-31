/**@file DisplayEngine.cpp
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera definicje metod klasy DispalyEngine.
*/


#include "EngineCore/stdafx.h"
#include "DisplayEngine.h"

#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/ModelsManager/ModelsManager.h"
#include "EngineCore/EventsManager/Events/RenderOnceEndedEvent.h"

#include "EngineCore/EngineHelpers/PerformanceCheck.h"
#include "EngineCore/EngineHelpers/ActorsCommonFunctions.h"
#include "GraphicAPI/ResourcesFactory.h"

#include "Common/MemoryLeaks.h"

using namespace DirectX;

const wchar_t CONSTANT_PER_FRAME_BUFFER_NAME[] = L"::DisplayEngine::ConstantPerFrameBuffer";
const wchar_t CONSTANT_PER_MESH_BUFFER_NAME[] = L"::DisplayEngine::ConstantPerMeshBuffer";

REGISTER_PERFORMANCE_CHECK( SKYBOX_RENDERING )
REGISTER_PERFORMANCE_CHECK( INSTANCE_OBJECT_RENDERING )
REGISTER_PERFORMANCE_CHECK( DYNAMIC_OBJECT_RENDERING )
REGISTER_PERFORMANCE_CHECK( PARTICLES_RENDERING )
REGISTER_PERFORMANCE_CHECK( SHORT_LIVE_OBJECTS_RENDERING )
REGISTER_PERFORMANCE_CHECK( SKELETNS_RENDERING )
REGISTER_PERFORMANCE_CHECK( SELF_DRAWING_OBJECTS_RENDERING )


DisplayEngine::DisplayEngine( Engine* engine )
	: engine( engine )
{
	m_defaultCamera = nullptr;
	m_currentCamera = nullptr;
	sky_dome = nullptr;
	m_mainRenderTarget = nullptr;
	m_mainSwapChain = nullptr;

	interpol_matrixes_count = 16;
	interpolated_matrixes = new XMFLOAT4X4[interpol_matrixes_count];

	m_maxQueuedPassesPerFrame = 5;
}


DisplayEngine::~DisplayEngine()
{
	for ( IRenderer* renderer : m_renderers )
		if ( renderer )		delete renderer;

	//kasujemy tablicê interpolowanych macierzy
	delete[] interpolated_matrixes;

	if ( sky_dome )
		delete sky_dome;

	delete m_defaultCamera;
	delete m_mainSwapChain;
}

/**@brief Inicjuje renderer (lub kilka je¿eli renderujemy wielow¹tkowo).
Poza tym inicjuje bufory sta³ych. Przy okazji inicjuje te¿ stany z-bufora do szybkiego
w³¹czania i wy³¹czania algorytmu.

@todo Zrobiæ inicjacjê wielow¹tkow¹. Gdzieœ musi zostaæ podjêta decyzja o liczbie w¹tków.
Trzeba pomyœleæ gdzie.*/
void DisplayEngine::InitRenderer( IRenderer* renderer )
{
	m_renderers.push_back( renderer );		// Na razie nie robimy deferred renderingu.

	m_renderers[0]->InitDepthStates();
}

void DisplayEngine::InitDisplayer( ModelsManager* assetsManager )
{
	modelsManager = assetsManager;
	m_constantsPerFrame	= modelsManager->CreateConstantsBuffer( CONSTANT_PER_FRAME_BUFFER_NAME, nullptr, sizeof( ConstantPerFrame ) );
	m_constantsPerMesh	= modelsManager->CreateConstantsBuffer( CONSTANT_PER_MESH_BUFFER_NAME, nullptr, sizeof( ConstantPerMesh ) );
	assert( m_constantsPerFrame );
	assert( m_constantsPerMesh );

	m_mainRenderTarget = modelsManager->GetRenderTarget( SCREEN_RENDERTARGET_STRING );
	m_mainRenderTarget->AddAssetReference();		/// Uniemo¿liwiamy zwolnienie render targetu przez u¿ytkownika.

	m_mainSwapChain = ResourcesFactory::CreateScreenSwapChain( m_mainRenderTarget );

	m_defaultCamera = new CameraActor();
	SetCurrentCamera( m_defaultCamera );
}

void DisplayEngine::BeginScene()
{
	//m_renderers[ 0 ]->BeginScene( m_mainRenderTarget );
}

void DisplayEngine::EndScene()
{
	//m_renderers[ 0 ]->Present();
	m_mainSwapChain->Present( 0 );
}

void DisplayEngine::SetMainRenderTarget( RenderTargetObject* renderTarget )
{
	m_mainRenderTarget->DeleteAssetReference();
	m_mainRenderTarget = renderTarget;
	m_mainRenderTarget->AddAssetReference();
}


//-------------------------------------------------------------------------------//
//							Funkcje pomocnicze do renderingu
//-------------------------------------------------------------------------------//


/**@brief kopiuje materia³ do struktury, która pos³u¿y do zaktualizowania bufora sta³ych.

@param[in] shaderDataPerMesh Struktura docelowa.
@param[in] model ModelPart z którego pobieramy dane.*/
void DisplayEngine::CopyMaterial( ConstantPerMesh* shader_data_per_mesh, const ModelPart* model )
{
	MaterialObject* material = model->material;
	shader_data_per_mesh->Diffuse = material->Diffuse;
	shader_data_per_mesh->Ambient = material->Ambient;
	shader_data_per_mesh->Specular = material->Specular;
	shader_data_per_mesh->Emissive = material->Emissive;
	shader_data_per_mesh->Power = material->Power;
}

//-------------------------------------------------------------------------------//
//							W³aœciwe funkcje do renderingu
//-------------------------------------------------------------------------------//

/**@brief Funkcja do renderowania sceny

Poniewa¿ klasa UI_Engine tak¿e wykonuje renderowanie( wyœwietla elementy interfejsu graficznego ),
funkcja displayEngine nie mo¿e sama wywo³ywaæ  BeginScene( ) i EndScene( ) z bilbioteki directX, aby nie
by³o podwojnego wywo³ania. Z tego wzglêdu powy¿sze funkcje s¹ wywo³ywane zewnetrznie w pêtli g³ównej przez klasê Engine.

Funkcja displayEngine ma obowi¹zek za ka¿dym razem od nowa ustawiæ macierz widoku i projekcji, poniewa¿
mog¹ byæ zmodyfikowane przez UI_Engine.Innymi s³owy nie mo¿na za³o¿yæ, ¿e jak siê raz ustawi³o macierze,
to przy nastêpnym wywo³aniu bêd¹ takie same.

@param[in] timeInterval Czas od ostatniej klatki. Przy ustawionej sta³ej @ref FIXED_FRAMES_COUNT czas ten jest sta³y
i wynosi tyle ile wartoœæ sta³ej FIXED_MOVE_UPDATE_INTERVAL.
@param[in] timeLag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::DisplayScene( float timeInterval, float timeLag )
{
	register IRenderer* renderer = m_renderers[0];		///<@todo Docelowo ma to dzia³aæ wielow¹tkowo i wybieraæ jeden z rendererów.

	SetViewMatrix( timeLag );

	// Ustawiamy bufor sta³y dla wszystkich meshy
	//shader_data_per_frame.time_lag = 

	// Ustawiamy topologiê
	// D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	// D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	renderer->IASetPrimitiveTopology( PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	renderer->UpdateSubresource( m_constantsPerFrame.Ptr(), &shader_data_per_frame );
	renderer->VSSetConstantBuffers( 0, m_constantsPerFrame.Ptr() );
	renderer->PSSetConstantBuffers( 0, m_constantsPerFrame.Ptr() );

	// Ustawiamy sampler
	renderer->SetDefaultSampler();

	RenderFromQueue( timeInterval, timeLag );

	renderer->BeginScene( m_mainRenderTarget );
	// Zaczynamy wyswietlanie
	DisplaySkyBox( timeInterval, timeLag );

	// Ustawiamy format wierzcho³ków
	renderer->IASetInputLayout( defaultLayout );

	DisplayInstancedMeshes( timeInterval, timeLag );
	DisplayDynamicObjects( timeInterval, timeLag );
	DisplayParticles( timeInterval, timeLag );
	DisplayShortLiveObjects( timeInterval, timeLag );
	DisplaySkeletons( timeInterval, timeLag );
	DisplaySelfDrawingObjects( timeInterval, timeLag );
}

/**@brief Funkcja s³u¿y do wyœwietlania meshy instancjonowanych, które s¹ jednoczeœniej obiektami statycznymi
w scenie.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::DisplayInstancedMeshes( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( INSTANCE_OBJECT_RENDERING )


	END_PERFORMANCE_CHECK( INSTANCE_OBJECT_RENDERING )
}

/**@brief Funkcja s³u¿y do wyœwietlania obiektów dynamicznych, które s¹ rzadko niszczone.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::DisplayDynamicObjects( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( DYNAMIC_OBJECT_RENDERING )

	register IRenderer* renderer = m_renderers[0];		///<@todo Docelowo ma to dzia³aæ wielow¹tkowo i wybieraæ jeden z rendererów.

	//na razie pêtla bez optymalizacji
	for ( unsigned int i = 0; i < meshes.size( ); ++i )
	{
		register StaticActor* object = meshes[i];

		// Ustawiamy bufor wierzcho³ków
		if ( renderer->SetVertexBuffer( object->GetVertexBuffer() ) )
			continue;	// Je¿eli nie ma bufora wierzcho³ków, to idziemy do nastêpnego mesha

		// Ustawiamy bufor indeksów, je¿eli istnieje
		renderer->SetIndexBuffer( object->GetIndexBuffer() );


#ifdef _INTERPOLATE_POSITIONS
		XMMATRIX transformation = XMLoadFloat4x4( &(interpolated_matrixes[i]) );
#else
		XMVECTOR translation = XMLoadFloat3( &(object->position) );
		XMVECTOR orientation = XMLoadFloat4( &(object->orientation) );
		XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
		transformation = transformation * XMMatrixTranslationFromVector( translation );
#endif


		for ( unsigned int j = 0; j < object->GetModelParts().size( ); ++j )
		{
			ModelPart& model = object->GetModelParts()[j];

			// Wyliczamy macierz transformacji
			XMMATRIX worldTransform;
			worldTransform = XMLoadFloat4x4( &(model.mesh->transform_matrix) );
			worldTransform = worldTransform * transformation;

			// Wype³niamy bufor sta³ych
			ConstantPerMesh shaderDataPerMesh;
			shaderDataPerMesh.world_matrix = XMMatrixTranspose( worldTransform );	// Transformacja wierzcho³ków
			shaderDataPerMesh.mesh_scale = XMVectorSetW( XMVectorReplicate( object->GetScale() ), 1.0f );

			// Przepisujemy materia³
			CopyMaterial( &shaderDataPerMesh, &model );

			// Ustawiamy shadery
			renderer->SetShaders( model );

			// Aktualizujemy bufor sta³ych
			renderer->UpdateSubresource( m_constantsPerMesh.Ptr(), &shaderDataPerMesh );
			renderer->VSSetConstantBuffers( 1, m_constantsPerMesh.Ptr() );
			renderer->PSSetConstantBuffers( 1, m_constantsPerMesh.Ptr() );

			// Ustawiamy tekstury
			renderer->SetTextures( model );

			// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpoœredni.
			MeshPartObject* part = model.mesh;
			if ( part->use_index_buf )
				renderer->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
			else // Tryb bezpoœredni
				renderer->Draw( part->vertices_count, part->buffer_offset );
		}

	}

	END_PERFORMANCE_CHECK( DYNAMIC_OBJECT_RENDERING )
}

/**@brief Funkcja s³u¿y do wyœwietlania systemów cz¹stek.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::DisplayParticles( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( PARTICLES_RENDERING )

	END_PERFORMANCE_CHECK( PARTICLES_RENDERING )
}

/**@brief Funkcja s³u¿y do wyœwietlania obiektów dynamicznych o krótkim czasie ¿ycia
jak np. pociski.

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::DisplayShortLiveObjects( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( SHORT_LIVE_OBJECTS_RENDERING )

	END_PERFORMANCE_CHECK( SHORT_LIVE_OBJECTS_RENDERING )
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
void DisplayEngine::DisplaySkyBox( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( SKYBOX_RENDERING )

	if ( !sky_dome )
		return;

	register IRenderer* renderer = m_renderers[0];		///<@todo Docelowo ma to dzia³aæ wielow¹tkowo i wybieraæ jeden z rendererów.


	// Ustawiamy format wierzcho³ków
	renderer->IASetInputLayout( sky_dome->get_vertex_layout() );

	// Aktualizuje bufor wierzcho³ków. Wstawiane s¹ nowe kolory.
	// Powinna byæ to raczej rzadka czynnoœæ, poniewa¿ aktualizacja jest kosztowna czasowo
	if ( sky_dome->update_vertex_buffer )
		sky_dome->update_buffers( renderer );

	// Ustawiamy bufor wierzcho³ków
	if ( renderer->SetVertexBuffer( sky_dome->get_vertex_buffer() ) )
		return;	// Je¿eli nie ma bufora wierzcho³ków, to idziemy do nastêpnego mesha
	// Ustawiamy bufor indeksów, je¿eli istnieje
	renderer->SetIndexBuffer( sky_dome->get_index_buffer() );


	ModelPart* model = sky_dome->get_model_part();

	// Wyliczamy macierz transformacji
	XMVECTOR quaternion = m_currentCamera->GetInterpolatedOrientation( time_lag );
	inverse_camera_orientation( quaternion );

	XMMATRIX rotation_matrix = XMMatrixRotationQuaternion( quaternion );

	// Wype³niamy bufor sta³ych
	ConstantPerMesh shader_data_per_mesh;
	shader_data_per_mesh.world_matrix = XMMatrixTranspose( rotation_matrix );	// Transformacja wierzcho³ków
	// Przepisujemy materia³
	CopyMaterial( &shader_data_per_mesh, model );

	// Ustawiamy shadery
	renderer->SetShaders( *model );

	// Aktualizujemy bufor sta³ych
	renderer->UpdateSubresource( m_constantsPerMesh.Ptr(), &shader_data_per_mesh );
	renderer->VSSetConstantBuffers( 1, m_constantsPerMesh.Ptr() );
	renderer->PSSetConstantBuffers( 1, m_constantsPerMesh.Ptr() );

	BufferObject* const_buffer = sky_dome->get_constant_buffer();
	if( const_buffer )
	{
		renderer->VSSetConstantBuffers( 2, const_buffer );
		renderer->PSSetConstantBuffers( 2, const_buffer );
	}

	// Ustawiamy tekstury
	renderer->SetTextures( *model );

	renderer->DepthBufferEnable( false );		///< Wy³¹czamy z-bufor. @todo To musi robiæ renderer.

	// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpoœredni.
	const MeshPartObject* part = model->mesh;
	if ( part->use_index_buf )
		renderer->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
	else // Tryb bezpoœredni
		renderer->Draw( part->vertices_count, part->buffer_offset );

	renderer->DepthBufferEnable( true );		///< W³¹czamy z-bufor spowrotem. @todo To musi robiæ renderer.

	END_PERFORMANCE_CHECK( SKYBOX_RENDERING )
}

/**@brief Funkcja s³u¿y do wyœwietlania obiektów z animacj¹ szkieletow¹, czyli g³ównie postaci


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::DisplaySkeletons( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( SKELETNS_RENDERING )


	END_PERFORMANCE_CHECK( SKELETNS_RENDERING )
}

/**@brief Funkcja wyœwietla obiekty, które maj¹ zaimplementowany inny
ni¿ domyœlny algorytm wyswietlania.

@see @ref self_drawing_objects

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::DisplaySelfDrawingObjects( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( SELF_DRAWING_OBJECTS_RENDERING )

	END_PERFORMANCE_CHECK( SELF_DRAWING_OBJECTS_RENDERING )
}

/**@brief Wybiera z kolejki przebiegi do wyrenderowania i renderuje je.

Kolejka zawiera tylko przebiegi, które maj¹ byæ wyrenderowane raz.
Po wyrenderowaniu wysy³any jest event RenderOnceEndedEvent.

@todo To jest tak straszliwie tymczasowa funkcja, ¿e w³aœciwie siê nadaje tylko do renderowania
lightmap. Trzeba to napisaæ bardzo porz¹dnie.

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.*/
void DisplayEngine::RenderFromQueue( float time_interval, float time_lag )
{
	register IRenderer* renderer = m_renderers[0];

	for( unsigned int i = 0; i < m_maxQueuedPassesPerFrame; ++i )
	{
		if( !m_renderOnceQueue.empty() )
		{
			RenderPass* renderPass = m_renderOnceQueue.front();
			m_renderOnceQueue.pop();

			renderer->BeginScene( renderPass->GetRenderTarget() );
			renderer->IASetInputLayout( renderPass->GetLayout() );

			auto meshCollection = renderPass->GetMeshes();

			//na razie pêtla bez optymalizacji
			for ( unsigned int i = 0; i < meshCollection.size( ); ++i )
			{
				register StaticActor* object = meshCollection[i];

				// Ustawiamy bufor wierzcho³ków
				if ( renderer->SetVertexBuffer( object->GetVertexBuffer() ) )
					continue;	// Je¿eli nie ma bufora wierzcho³ków, to idziemy do nastêpnego mesha


				XMVECTOR translation = object->GetPosition();
				XMVECTOR orientation = object->GetOrientation();
				XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
				transformation = transformation * XMMatrixTranslationFromVector( translation );

				for ( unsigned int j = 0; j < object->GetModelParts().size( ); ++j )
				{
					ModelPart& model = object->GetModelParts()[j];

					// Wyliczamy macierz transformacji
					XMMATRIX worldTransform;
					worldTransform = XMLoadFloat4x4( &(model.mesh->transform_matrix) );
					worldTransform = worldTransform * transformation;

					// Wype³niamy bufor sta³ych
					ConstantPerMesh shaderDataPerMesh;
					shaderDataPerMesh.world_matrix = XMMatrixTranspose( worldTransform );	// Transformacja wierzcho³ków
					shaderDataPerMesh.mesh_scale = XMVectorSetW( XMVectorReplicate( object->GetScale() ), 1.0f );

					// Przepisujemy materia³
					CopyMaterial( &shaderDataPerMesh, &model );

					// Ustawiamy shadery
					renderer->SetShaders( model );

					// Aktualizujemy bufor sta³ych
					renderer->UpdateSubresource( m_constantsPerMesh.Ptr(), &shaderDataPerMesh );
					renderer->VSSetConstantBuffers( 1, m_constantsPerMesh.Ptr() );
					renderer->PSSetConstantBuffers( 1, m_constantsPerMesh.Ptr() );

					// Ustawiamy tekstury
					renderer->SetTextures( model );

					// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpoœredni.
					MeshPartObject* part = model.mesh;
					if ( part->use_index_buf )
						renderer->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
					else // Tryb bezpoœredni
						renderer->Draw( part->vertices_count, part->buffer_offset );
				}

			}

			RenderOnceEndedEvent*  renderedEvent = new RenderOnceEndedEvent;
			renderedEvent->renderPass = renderPass;
			engine->SendEvent( renderedEvent );
		}
	}
}

/**@brief Tworzy macierz projekcji i zapamiêtuje j¹ w polu projection_matrix klasy. W ka¿dym wywo³aniu funkcji
DisplayScene ustawiana jest macierz zapisana w tym polu.
@param[in] angle K¹t widzenia w pionie
@param[in] X_to_Y Stosunek Szerokoœci do wysokoœci ekranu
@param[in] near_plane Bli¿sza p³aszczyzna obcinania
@param[in] far_plane Dalsza p³aszczyzna obcinania

@deprecated Zacz¹æ u¿ywaæ danych z kamer, a tê funkcjê zlikwidowaæ*/
void DisplayEngine::SetProjectionMatrix( float angle, float X_to_Y, float near_plane, float far_plane )
{
	XMMATRIX proj_matrix = XMMatrixPerspectiveFovRH( angle, X_to_Y, near_plane, far_plane );
	proj_matrix = XMMatrixTranspose( proj_matrix );
	XMStoreFloat4x4( &shader_data_per_frame.projection_matrix, proj_matrix );
}


//=================================================================//
//					camera functions
//=================================================================//


/**@brief U¿ywa aktualnie ustawionej kamery, ¿eby stworzyæ macierz widoku.

Macierz widoku jest zapisywana w strukturze, która pos³u¿y do wype³nienia bufora sta³ych
Funkcja jest prywatna, poniewa¿ jest wywo³ywana podczas renderowania sceny. Aby ingerowaæ z zewn¹trz
w ustawienie kamery nale¿y zmieniæ aktualnie ustawion¹ kamerê na jedn¹ z innych zapisanych w silniku.

@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::SetViewMatrix( float time_lag )
{
	XMMATRIX view_matrix;
	if ( m_currentCamera == nullptr )
		view_matrix  = XMMatrixIdentity( );	//tymczasowe
	else
	{
#ifndef _INTERPOLATE_POSITIONS
		//bêdziemy mno¿yæ macierz translacji * macierz obrotu
		view_matrix = XMMatrixTranslation(
			-m_currentCamera->position.x,
			-m_currentCamera->position.y,
			-m_currentCamera->position.z);
		XMVECTOR quaternion = XMLoadFloat4(&(m_currentCamera->orientation));
		quaternion = XMVectorNegate( quaternion );
		quaternion = XMVectorSetW( quaternion, -XMVectorGetW( quaternion ) );

		XMMATRIX rotation_matrix = XMMatrixRotationQuaternion(quaternion);
		view_matrix = view_matrix * rotation_matrix;
#else
		XMVECTOR position = m_currentCamera->GetInterpolatedPosition( time_lag );
		XMVECTOR orientation = m_currentCamera->GetInterpolatedOrientation( time_lag );
		inverse_camera_position( position );
		inverse_camera_orientation( orientation );

		view_matrix = XMMatrixTranslationFromVector( position );
		XMMATRIX rotation_matrix = XMMatrixRotationQuaternion( orientation );
		view_matrix = view_matrix * rotation_matrix;
#endif
	}

	//Wstawiamy macierz do zmiennej, która stanie siê potem zawartoœci¹ bufora,
	//który zostanie przekazany do shadera.
	view_matrix = XMMatrixTranspose( view_matrix );
	XMStoreFloat4x4( &shader_data_per_frame.view_matrix, view_matrix );
}

/**@brief Dodaje obiekt, który ma zostaæ wyœwietlony.

@param[in] object ActorBase, który ma zostaæ dopisany do tablic wyœwietlania.*/
void DisplayEngine::AddMeshObject( StaticActor* object )
{
	realocate_interpolation_memory( );		//powiêkszamy tablicê macierzy interpolacji
							//wykona siê tylko je¿eli jest konieczne
	meshes.push_back( object );
}

/**@brief Usuwa aktora z modu³u.

Funkcja przegl¹da aktorów od ty³u, poniewa¿ bardziej prawdopodobne jest,
¿e usuwamy aktora stworzonego niedawno.*/
void DisplayEngine::RemoveActor( ActorBase* actor )
{
	ActorsCommonFunctions::RemoveActor( meshes, static_cast< StaticActor* >( actor ) );
	ActorsCommonFunctions::RemoveActor( cameras, static_cast< CameraActor* >( actor ) );

	if( m_currentCamera == actor )
		SetCurrentCamera( m_defaultCamera );
}

/**@brief Kasuje wszystkich aktorów.*/
void DisplayEngine::RemoveAllActors()
{
	meshes.clear();
	cameras.clear();
	SetCurrentCamera( m_defaultCamera );
}

/**@brief Kasuje wszystkie meshe. Pomijane s¹ kamery.*/
void DisplayEngine::DeleteAllMeshes()
{
	meshes.clear();
}


/**@brief Dodaje kamerê do spisu kamer w silniku.

@param[in] camera Kamera do dodania.
@return
Funkcja zwraca 0 w przypadku powodzenia.
Je¿eli kamera ju¿ istnia³a wczesniej, to zwracan¹ wartoœci¹ jest 1.
Je¿eli podano wskaŸnik nullptr, zwrócona zostanie wartoœæ 2.*/
int DisplayEngine::AddCamera( CameraActor* camera )
{
	if ( camera == nullptr )
		return 2;
	for ( unsigned int i = 0; i < cameras.size( ); ++i )
		if ( cameras[i] == camera )
			return 1;	//kamera ju¿ istnieje

	cameras.push_back( camera );
	return 0;
}

/**@brief Ustawia aktualnie u¿ywan¹ kamerê.
@param[in] camera Kamera do ustawienia
@return 0 w przypadku powodzenia, 1 je¿eli kamera by³a nullptrem.
Zasadniczo nie ma po co sprawdzaæ wartoœci zwracanej.*/
int DisplayEngine::SetCurrentCamera( CameraActor* camera )
{
	if ( camera == nullptr )
		return 1;
	
	m_currentCamera = camera;
	return 0;
}

/**@brief Zwraca aktualnie ustawion¹ kamerê g³ówn¹.*/
CameraActor* DisplayEngine::GetCurrentCamera()
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
void DisplayEngine::realocate_interpolation_memory( unsigned int min )
{
	if ( interpol_matrixes_count < min + meshes.size() )
	{
		while ( interpol_matrixes_count < min + meshes.size() )
			interpol_matrixes_count <<= 1;	//wielkoœæ tablicy roœnie wyk³adniczo

		delete[] interpolated_matrixes;
		interpolated_matrixes = new DirectX::XMFLOAT4X4[interpol_matrixes_count];
	}
}

/**@brief Funkcja wykonuje interpolacjê po³o¿eñ dla wszystkich obiektów dynamicznych
w silniku.

Obiekty s¹ one przesuwane o tak¹ odleg³oœæ jaka wynika z ich prêdkoœci
oraz ze zmiennej time_lag, która oznacza czas, jaki up³yn¹³ od ostatniego
przeliczenia klatki.

Pozycjê z ostatniego wyliczenia klatki znajduj¹ siê w zmiennych position
i orientation obiektów dynamicznych. Docelowe macierze przekszta³ceñ obiektów
zostan¹ umieszczone w tablicy interpolated_matrixes, w których indeks elementu
odpowiada indeksom w tablicy meshes.

@todo [docelowo do wykonania wielow¹tkowego]

@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].*/
void DisplayEngine::InterpolatePositions( float time_lag )
{
	for ( unsigned int i = 0; i < meshes.size(); ++i )
	{
		StaticActor* object = meshes[i];
		interpolate_object2( time_lag, object, &(interpolated_matrixes[i]) );
	}
}


/**@brief Funkcja tworzy macierz przekszta³cenia dla podanego obiektu, interpoluj¹c jego pozycjê
z prêdkoœci postêpowej i k¹towej.

@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
@param[in] object Objekt, dla którego liczymy macierz przekszta³cenia.
@param[out] transform_matrix Zmienna, w której zostanie umieszczona interpolowana macierz przekszta³cenia.
*/
void DisplayEngine::interpolate_object2( float time_lag, const StaticActor* object, DirectX::XMFLOAT4X4* result_matrix )
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

/**@brief Ustawia œwiat³o kierunkowe
@param[in] direction Kierunek œwiecenia œwiat³a
@param[in] color Kolor œwiat³a
@param[in] index Indeks œwiat³a w tablicy œwiate³. Maksymalnie wynosi @ref ENGINE_MAX_LIGHTS - 1.
@return 0 w przypadku powodzenia, -1 je¿eli jest niepoprawny indeks.
*/
int DisplayEngine::SetDirectionalLight( const DirectX::XMFLOAT4& direction,
										  const DirectX::XMFLOAT4& color,
										  unsigned int index )
{
	if ( index >= ENGINE_MAX_LIGHTS )
		return -1;

	XMVECTOR light_dir = XMLoadFloat4( &direction );
	XMVECTOR light_color = XMLoadFloat4( &color );

	// Normalizujemy wektor
	light_dir = XMVector3Normalize( light_dir );
	light_dir = XMVectorNegate( light_dir );		// Robimy to, ¿eby shader nie musia³ odwracaæ
	XMStoreFloat4( &shader_data_per_frame.light_direction[index], light_dir );
	
	// Przycinamy wektor do przedzia³u [0.0 , 1.0]
	light_color = XMVectorSaturate( light_color );
	XMStoreFloat4( &shader_data_per_frame.light_color[index], light_color );

	return 0;
}

/**@brief Ustawia œwiat³o ambient.
@param[in] color Kolor œwiat³a.*/
void DisplayEngine::SetAmbientLight( const DirectX::XMFLOAT4& color )
{
	shader_data_per_frame.ambient_light = color;
}

/**@brief Ustawia nowego skydome'a dla gry.

Nale¿y podaæ w pe³ni skonstruowanego i gotowego do wyœwietlania SkyDome'a.
Funkcja zwraca poprzednio ustawionego SkyDome'a, którego nale¿y zwolniæ samemu.
Aktualnie ustawiony SkyDome jest pod koniec programu zwalniany w destruktorze.

@note Je¿eli t³o ma nie byæ wyœwietlane nale¿y podaæ w parametrze nullptr.

@param[in] dome Nowy SkyDome, który ma zostaæ ustawiony.
@return Zwraca poprzedniego SkyDome'a.*/
SkyDome* DisplayEngine::SetSkydome( SkyDome* dome )
{
	SkyDome* old = sky_dome;
	sky_dome = dome;
	return old;
}

