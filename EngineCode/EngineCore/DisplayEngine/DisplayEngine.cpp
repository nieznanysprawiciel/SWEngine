/**@file DisplayEngine.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

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

	//kasujemy tablic� interpolowanych macierzy
	delete[] interpolated_matrixes;

	if ( sky_dome )
		delete sky_dome;

	delete m_defaultCamera;
	delete m_mainSwapChain;
}

/**@brief Inicjuje renderer (lub kilka je�eli renderujemy wielow�tkowo).
Poza tym inicjuje bufory sta�ych. Przy okazji inicjuje te� stany z-bufora do szybkiego
w��czania i wy��czania algorytmu.

@todo Zrobi� inicjacj� wielow�tkow�. Gdzie� musi zosta� podj�ta decyzja o liczbie w�tk�w.
Trzeba pomy�le� gdzie.*/
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
	m_mainRenderTarget->AddAssetReference();		/// Uniemo�liwiamy zwolnienie render targetu przez u�ytkownika.

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


/**@brief kopiuje materia� do struktury, kt�ra pos�u�y do zaktualizowania bufora sta�ych.

@param[in] shaderDataPerMesh Struktura docelowa.
@param[in] model ModelPart z kt�rego pobieramy dane.*/
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
//							W�a�ciwe funkcje do renderingu
//-------------------------------------------------------------------------------//

/**@brief Funkcja do renderowania sceny

Poniewa� klasa UI_Engine tak�e wykonuje renderowanie( wy�wietla elementy interfejsu graficznego ),
funkcja displayEngine nie mo�e sama wywo�ywa�  BeginScene( ) i EndScene( ) z bilbioteki directX, aby nie
by�o podwojnego wywo�ania. Z tego wzgl�du powy�sze funkcje s� wywo�ywane zewnetrznie w p�tli g��wnej przez klas� Engine.

Funkcja displayEngine ma obowi�zek za ka�dym razem od nowa ustawi� macierz widoku i projekcji, poniewa�
mog� by� zmodyfikowane przez UI_Engine.Innymi s�owy nie mo�na za�o�y�, �e jak si� raz ustawi�o macierze,
to przy nast�pnym wywo�aniu b�d� takie same.

@param[in] timeInterval Czas od ostatniej klatki. Przy ustawionej sta�ej @ref FIXED_FRAMES_COUNT czas ten jest sta�y
i wynosi tyle ile warto�� sta�ej FIXED_MOVE_UPDATE_INTERVAL.
@param[in] timeLag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::DisplayScene( float timeInterval, float timeLag )
{
	register IRenderer* renderer = m_renderers[0];		///<@todo Docelowo ma to dzia�a� wielow�tkowo i wybiera� jeden z renderer�w.

	SetViewMatrix( timeLag );

	// Ustawiamy bufor sta�y dla wszystkich meshy
	//shader_data_per_frame.time_lag = 

	// Ustawiamy topologi�
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

	// Ustawiamy format wierzcho�k�w
	renderer->IASetInputLayout( defaultLayout );

	DisplayInstancedMeshes( timeInterval, timeLag );
	DisplayDynamicObjects( timeInterval, timeLag );
	DisplayParticles( timeInterval, timeLag );
	DisplayShortLiveObjects( timeInterval, timeLag );
	DisplaySkeletons( timeInterval, timeLag );
	DisplaySelfDrawingObjects( timeInterval, timeLag );
}

/**@brief Funkcja s�u�y do wy�wietlania meshy instancjonowanych, kt�re s� jednocze�niej obiektami statycznymi
w scenie.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::DisplayInstancedMeshes( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( INSTANCE_OBJECT_RENDERING )


	END_PERFORMANCE_CHECK( INSTANCE_OBJECT_RENDERING )
}

/**@brief Funkcja s�u�y do wy�wietlania obiekt�w dynamicznych, kt�re s� rzadko niszczone.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::DisplayDynamicObjects( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( DYNAMIC_OBJECT_RENDERING )

	register IRenderer* renderer = m_renderers[0];		///<@todo Docelowo ma to dzia�a� wielow�tkowo i wybiera� jeden z renderer�w.

	//na razie p�tla bez optymalizacji
	for ( unsigned int i = 0; i < meshes.size( ); ++i )
	{
		register StaticActor* object = meshes[i];

		// Ustawiamy bufor wierzcho�k�w
		if ( renderer->SetVertexBuffer( object->GetVertexBuffer() ) )
			continue;	// Je�eli nie ma bufora wierzcho�k�w, to idziemy do nast�pnego mesha

		// Ustawiamy bufor indeks�w, je�eli istnieje
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

			// Wype�niamy bufor sta�ych
			ConstantPerMesh shaderDataPerMesh;
			shaderDataPerMesh.world_matrix = XMMatrixTranspose( worldTransform );	// Transformacja wierzcho�k�w
			shaderDataPerMesh.mesh_scale = XMVectorSetW( XMVectorReplicate( object->GetScale() ), 1.0f );

			// Przepisujemy materia�
			CopyMaterial( &shaderDataPerMesh, &model );

			// Ustawiamy shadery
			renderer->SetShaders( model );

			// Aktualizujemy bufor sta�ych
			renderer->UpdateSubresource( m_constantsPerMesh.Ptr(), &shaderDataPerMesh );
			renderer->VSSetConstantBuffers( 1, m_constantsPerMesh.Ptr() );
			renderer->PSSetConstantBuffers( 1, m_constantsPerMesh.Ptr() );

			// Ustawiamy tekstury
			renderer->SetTextures( model );

			// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpo�redni.
			MeshPartObject* part = model.mesh;
			if ( part->use_index_buf )
				renderer->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
			else // Tryb bezpo�redni
				renderer->Draw( part->vertices_count, part->buffer_offset );
		}

	}

	END_PERFORMANCE_CHECK( DYNAMIC_OBJECT_RENDERING )
}

/**@brief Funkcja s�u�y do wy�wietlania system�w cz�stek.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::DisplayParticles( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( PARTICLES_RENDERING )

	END_PERFORMANCE_CHECK( PARTICLES_RENDERING )
}

/**@brief Funkcja s�u�y do wy�wietlania obiekt�w dynamicznych o kr�tkim czasie �ycia
jak np. pociski.

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::DisplayShortLiveObjects( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( SHORT_LIVE_OBJECTS_RENDERING )

	END_PERFORMANCE_CHECK( SHORT_LIVE_OBJECTS_RENDERING )
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
void DisplayEngine::DisplaySkyBox( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( SKYBOX_RENDERING )

	if ( !sky_dome )
		return;

	register IRenderer* renderer = m_renderers[0];		///<@todo Docelowo ma to dzia�a� wielow�tkowo i wybiera� jeden z renderer�w.


	// Ustawiamy format wierzcho�k�w
	renderer->IASetInputLayout( sky_dome->get_vertex_layout() );

	// Aktualizuje bufor wierzcho�k�w. Wstawiane s� nowe kolory.
	// Powinna by� to raczej rzadka czynno��, poniewa� aktualizacja jest kosztowna czasowo
	if ( sky_dome->update_vertex_buffer )
		sky_dome->update_buffers( renderer );

	// Ustawiamy bufor wierzcho�k�w
	if ( renderer->SetVertexBuffer( sky_dome->get_vertex_buffer() ) )
		return;	// Je�eli nie ma bufora wierzcho�k�w, to idziemy do nast�pnego mesha
	// Ustawiamy bufor indeks�w, je�eli istnieje
	renderer->SetIndexBuffer( sky_dome->get_index_buffer() );


	ModelPart* model = sky_dome->get_model_part();

	// Wyliczamy macierz transformacji
	XMVECTOR quaternion = m_currentCamera->GetInterpolatedOrientation( time_lag );
	inverse_camera_orientation( quaternion );

	XMMATRIX rotation_matrix = XMMatrixRotationQuaternion( quaternion );

	// Wype�niamy bufor sta�ych
	ConstantPerMesh shader_data_per_mesh;
	shader_data_per_mesh.world_matrix = XMMatrixTranspose( rotation_matrix );	// Transformacja wierzcho�k�w
	// Przepisujemy materia�
	CopyMaterial( &shader_data_per_mesh, model );

	// Ustawiamy shadery
	renderer->SetShaders( *model );

	// Aktualizujemy bufor sta�ych
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

	renderer->DepthBufferEnable( false );		///< Wy��czamy z-bufor. @todo To musi robi� renderer.

	// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpo�redni.
	const MeshPartObject* part = model->mesh;
	if ( part->use_index_buf )
		renderer->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
	else // Tryb bezpo�redni
		renderer->Draw( part->vertices_count, part->buffer_offset );

	renderer->DepthBufferEnable( true );		///< W��czamy z-bufor spowrotem. @todo To musi robi� renderer.

	END_PERFORMANCE_CHECK( SKYBOX_RENDERING )
}

/**@brief Funkcja s�u�y do wy�wietlania obiekt�w z animacj� szkieletow�, czyli g��wnie postaci


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::DisplaySkeletons( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( SKELETNS_RENDERING )


	END_PERFORMANCE_CHECK( SKELETNS_RENDERING )
}

/**@brief Funkcja wy�wietla obiekty, kt�re maj� zaimplementowany inny
ni� domy�lny algorytm wyswietlania.

@see @ref self_drawing_objects

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::DisplaySelfDrawingObjects( float time_interval, float time_lag )
{
	START_PERFORMANCE_CHECK( SELF_DRAWING_OBJECTS_RENDERING )

	END_PERFORMANCE_CHECK( SELF_DRAWING_OBJECTS_RENDERING )
}

/**@brief Wybiera z kolejki przebiegi do wyrenderowania i renderuje je.

Kolejka zawiera tylko przebiegi, kt�re maj� by� wyrenderowane raz.
Po wyrenderowaniu wysy�any jest event RenderOnceEndedEvent.

@todo To jest tak straszliwie tymczasowa funkcja, �e w�a�ciwie si� nadaje tylko do renderowania
lightmap. Trzeba to napisa� bardzo porz�dnie.

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.*/
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

			//na razie p�tla bez optymalizacji
			for ( unsigned int i = 0; i < meshCollection.size( ); ++i )
			{
				register StaticActor* object = meshCollection[i];

				// Ustawiamy bufor wierzcho�k�w
				if ( renderer->SetVertexBuffer( object->GetVertexBuffer() ) )
					continue;	// Je�eli nie ma bufora wierzcho�k�w, to idziemy do nast�pnego mesha


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

					// Wype�niamy bufor sta�ych
					ConstantPerMesh shaderDataPerMesh;
					shaderDataPerMesh.world_matrix = XMMatrixTranspose( worldTransform );	// Transformacja wierzcho�k�w
					shaderDataPerMesh.mesh_scale = XMVectorSetW( XMVectorReplicate( object->GetScale() ), 1.0f );

					// Przepisujemy materia�
					CopyMaterial( &shaderDataPerMesh, &model );

					// Ustawiamy shadery
					renderer->SetShaders( model );

					// Aktualizujemy bufor sta�ych
					renderer->UpdateSubresource( m_constantsPerMesh.Ptr(), &shaderDataPerMesh );
					renderer->VSSetConstantBuffers( 1, m_constantsPerMesh.Ptr() );
					renderer->PSSetConstantBuffers( 1, m_constantsPerMesh.Ptr() );

					// Ustawiamy tekstury
					renderer->SetTextures( model );

					// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpo�redni.
					MeshPartObject* part = model.mesh;
					if ( part->use_index_buf )
						renderer->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
					else // Tryb bezpo�redni
						renderer->Draw( part->vertices_count, part->buffer_offset );
				}

			}

			RenderOnceEndedEvent*  renderedEvent = new RenderOnceEndedEvent;
			renderedEvent->renderPass = renderPass;
			engine->SendEvent( renderedEvent );
		}
	}
}

/**@brief Tworzy macierz projekcji i zapami�tuje j� w polu projection_matrix klasy. W ka�dym wywo�aniu funkcji
DisplayScene ustawiana jest macierz zapisana w tym polu.
@param[in] angle K�t widzenia w pionie
@param[in] X_to_Y Stosunek Szeroko�ci do wysoko�ci ekranu
@param[in] near_plane Bli�sza p�aszczyzna obcinania
@param[in] far_plane Dalsza p�aszczyzna obcinania

@deprecated Zacz�� u�ywa� danych z kamer, a t� funkcj� zlikwidowa�*/
void DisplayEngine::SetProjectionMatrix( float angle, float X_to_Y, float near_plane, float far_plane )
{
	XMMATRIX proj_matrix = XMMatrixPerspectiveFovRH( angle, X_to_Y, near_plane, far_plane );
	proj_matrix = XMMatrixTranspose( proj_matrix );
	XMStoreFloat4x4( &shader_data_per_frame.projection_matrix, proj_matrix );
}


//=================================================================//
//					camera functions
//=================================================================//


/**@brief U�ywa aktualnie ustawionej kamery, �eby stworzy� macierz widoku.

Macierz widoku jest zapisywana w strukturze, kt�ra pos�u�y do wype�nienia bufora sta�ych
Funkcja jest prywatna, poniewa� jest wywo�ywana podczas renderowania sceny. Aby ingerowa� z zewn�trz
w ustawienie kamery nale�y zmieni� aktualnie ustawion� kamer� na jedn� z innych zapisanych w silniku.

@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
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
		//b�dziemy mno�y� macierz translacji * macierz obrotu
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

	//Wstawiamy macierz do zmiennej, kt�ra stanie si� potem zawarto�ci� bufora,
	//kt�ry zostanie przekazany do shadera.
	view_matrix = XMMatrixTranspose( view_matrix );
	XMStoreFloat4x4( &shader_data_per_frame.view_matrix, view_matrix );
}

/**@brief Dodaje obiekt, kt�ry ma zosta� wy�wietlony.

@param[in] object ActorBase, kt�ry ma zosta� dopisany do tablic wy�wietlania.*/
void DisplayEngine::AddMeshObject( StaticActor* object )
{
	realocate_interpolation_memory( );		//powi�kszamy tablic� macierzy interpolacji
							//wykona si� tylko je�eli jest konieczne
	meshes.push_back( object );
}

/**@brief Usuwa aktora z modu�u.

Funkcja przegl�da aktor�w od ty�u, poniewa� bardziej prawdopodobne jest,
�e usuwamy aktora stworzonego niedawno.*/
void DisplayEngine::RemoveActor( ActorBase* actor )
{
	ActorsCommonFunctions::RemoveActor( meshes, static_cast< StaticActor* >( actor ) );
	ActorsCommonFunctions::RemoveActor( cameras, static_cast< CameraActor* >( actor ) );

	if( m_currentCamera == actor )
		SetCurrentCamera( m_defaultCamera );
}

/**@brief Kasuje wszystkich aktor�w.*/
void DisplayEngine::RemoveAllActors()
{
	meshes.clear();
	cameras.clear();
	SetCurrentCamera( m_defaultCamera );
}

/**@brief Kasuje wszystkie meshe. Pomijane s� kamery.*/
void DisplayEngine::DeleteAllMeshes()
{
	meshes.clear();
}


/**@brief Dodaje kamer� do spisu kamer w silniku.

@param[in] camera Kamera do dodania.
@return
Funkcja zwraca 0 w przypadku powodzenia.
Je�eli kamera ju� istnia�a wczesniej, to zwracan� warto�ci� jest 1.
Je�eli podano wska�nik nullptr, zwr�cona zostanie warto�� 2.*/
int DisplayEngine::AddCamera( CameraActor* camera )
{
	if ( camera == nullptr )
		return 2;
	for ( unsigned int i = 0; i < cameras.size( ); ++i )
		if ( cameras[i] == camera )
			return 1;	//kamera ju� istnieje

	cameras.push_back( camera );
	return 0;
}

/**@brief Ustawia aktualnie u�ywan� kamer�.
@param[in] camera Kamera do ustawienia
@return 0 w przypadku powodzenia, 1 je�eli kamera by�a nullptrem.
Zasadniczo nie ma po co sprawdza� warto�ci zwracanej.*/
int DisplayEngine::SetCurrentCamera( CameraActor* camera )
{
	if ( camera == nullptr )
		return 1;
	
	m_currentCamera = camera;
	return 0;
}

/**@brief Zwraca aktualnie ustawion� kamer� g��wn�.*/
CameraActor* DisplayEngine::GetCurrentCamera()
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
void DisplayEngine::realocate_interpolation_memory( unsigned int min )
{
	if ( interpol_matrixes_count < min + meshes.size() )
	{
		while ( interpol_matrixes_count < min + meshes.size() )
			interpol_matrixes_count <<= 1;	//wielko�� tablicy ro�nie wyk�adniczo

		delete[] interpolated_matrixes;
		interpolated_matrixes = new DirectX::XMFLOAT4X4[interpol_matrixes_count];
	}
}

/**@brief Funkcja wykonuje interpolacj� po�o�e� dla wszystkich obiekt�w dynamicznych
w silniku.

Obiekty s� one przesuwane o tak� odleg�o�� jaka wynika z ich pr�dko�ci
oraz ze zmiennej time_lag, kt�ra oznacza czas, jaki up�yn�� od ostatniego
przeliczenia klatki.

Pozycj� z ostatniego wyliczenia klatki znajduj� si� w zmiennych position
i orientation obiekt�w dynamicznych. Docelowe macierze przekszta�ce� obiekt�w
zostan� umieszczone w tablicy interpolated_matrixes, w kt�rych indeks elementu
odpowiada indeksom w tablicy meshes.

@todo [docelowo do wykonania wielow�tkowego]

@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].*/
void DisplayEngine::InterpolatePositions( float time_lag )
{
	for ( unsigned int i = 0; i < meshes.size(); ++i )
	{
		StaticActor* object = meshes[i];
		interpolate_object2( time_lag, object, &(interpolated_matrixes[i]) );
	}
}


/**@brief Funkcja tworzy macierz przekszta�cenia dla podanego obiektu, interpoluj�c jego pozycj�
z pr�dko�ci post�powej i k�towej.

@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
@param[in] object Objekt, dla kt�rego liczymy macierz przekszta�cenia.
@param[out] transform_matrix Zmienna, w kt�rej zostanie umieszczona interpolowana macierz przekszta�cenia.
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

/**@brief Ustawia �wiat�o kierunkowe
@param[in] direction Kierunek �wiecenia �wiat�a
@param[in] color Kolor �wiat�a
@param[in] index Indeks �wiat�a w tablicy �wiate�. Maksymalnie wynosi @ref ENGINE_MAX_LIGHTS - 1.
@return 0 w przypadku powodzenia, -1 je�eli jest niepoprawny indeks.
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
	light_dir = XMVectorNegate( light_dir );		// Robimy to, �eby shader nie musia� odwraca�
	XMStoreFloat4( &shader_data_per_frame.light_direction[index], light_dir );
	
	// Przycinamy wektor do przedzia�u [0.0 , 1.0]
	light_color = XMVectorSaturate( light_color );
	XMStoreFloat4( &shader_data_per_frame.light_color[index], light_color );

	return 0;
}

/**@brief Ustawia �wiat�o ambient.
@param[in] color Kolor �wiat�a.*/
void DisplayEngine::SetAmbientLight( const DirectX::XMFLOAT4& color )
{
	shader_data_per_frame.ambient_light = color;
}

/**@brief Ustawia nowego skydome'a dla gry.

Nale�y poda� w pe�ni skonstruowanego i gotowego do wy�wietlania SkyDome'a.
Funkcja zwraca poprzednio ustawionego SkyDome'a, kt�rego nale�y zwolni� samemu.
Aktualnie ustawiony SkyDome jest pod koniec programu zwalniany w destruktorze.

@note Je�eli t�o ma nie by� wy�wietlane nale�y poda� w parametrze nullptr.

@param[in] dome Nowy SkyDome, kt�ry ma zosta� ustawiony.
@return Zwraca poprzedniego SkyDome'a.*/
SkyDome* DisplayEngine::SetSkydome( SkyDome* dome )
{
	SkyDome* old = sky_dome;
	sky_dome = dome;
	return old;
}

