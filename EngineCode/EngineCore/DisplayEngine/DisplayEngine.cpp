/**@file DisplayEngine.cpp
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera definicje metod klasy DispalyEngine.
*/


#include "EngineCore/stdafx.h"
#include "DisplayEngine.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/EngineHelpers/PerformanceCheck.h"
#include "EngineCore/ModelsManager/ModelsManager.h"
#include "EngineCore//EventsManager/Event.h"

#include "Common/MemoryLeaks.h"

using namespace DirectX;

const wchar_t CONSTANT_PER_FRAME_BUFFER_NAME[] = L"::DisplayEngine::ConstantPerFrameBuffer";
const wchar_t CONSTANT_PER_MESH_BUFFER_NAME[] = L"::DisplayEngine::ConstantPerMeshBuffer";

USE_PERFORMANCE_CHECK( SKYBOX_RENDERING )
USE_PERFORMANCE_CHECK( INSTANCE_OBJECT_RENDERING )
USE_PERFORMANCE_CHECK( DYNAMIC_OBJECT_RENDERING )
USE_PERFORMANCE_CHECK( PARTICLES_RENDERING )
USE_PERFORMANCE_CHECK( SHORT_LIVE_OBJECTS_RENDERING )
USE_PERFORMANCE_CHECK( SKELETNS_RENDERING )
USE_PERFORMANCE_CHECK( SELF_DRAWING_OBJECTS_RENDERING )


DisplayEngine::DisplayEngine( Engine* engine )
	: engine( engine )
{
	current_camera = nullptr;
	sky_dome = nullptr;
	m_mainRenderTarget = nullptr;

	interpol_matrixes_count = 16;
	interpolated_matrixes = new XMFLOAT4X4[interpol_matrixes_count];

	REGISTER_PERFORMANCE_TASK( SKYBOX_RENDERING )
	REGISTER_PERFORMANCE_TASK( INSTANCE_OBJECT_RENDERING )
	REGISTER_PERFORMANCE_TASK( DYNAMIC_OBJECT_RENDERING )
	REGISTER_PERFORMANCE_TASK( PARTICLES_RENDERING )
	REGISTER_PERFORMANCE_TASK( SHORT_LIVE_OBJECTS_RENDERING )
	REGISTER_PERFORMANCE_TASK( SKELETNS_RENDERING )
	REGISTER_PERFORMANCE_TASK( SELF_DRAWING_OBJECTS_RENDERING )

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
	m_constantsPerFrame	= modelsManager->AddConstantsBuffer( CONSTANT_PER_FRAME_BUFFER_NAME, nullptr, sizeof( ConstantPerFrame ) );
	m_constantsPerMesh	= modelsManager->AddConstantsBuffer( CONSTANT_PER_MESH_BUFFER_NAME, nullptr, sizeof( ConstantPerMesh ) );
	assert( m_constantsPerFrame );
	assert( m_constantsPerMesh );

	m_constantsPerFrame->AddAssetReference();		/// Uniemo�liwiamy zwolnienie bufora przez u�ytkownika.
	m_constantsPerMesh->AddAssetReference();		/// Uniemo�liwiamy zwolnienie bufora przez u�ytkownika.

	m_mainRenderTarget = modelsManager->GetRenderTarget( SCREEN_RENDERTARGET_STRING );
	m_mainRenderTarget->AddAssetReference();		/// Uniemo�liwiamy zwolnienie render targetu przez u�ytkownika.
}

void DisplayEngine::BeginScene()
{
	//m_renderers[ 0 ]->BeginScene( m_mainRenderTarget );
}

void DisplayEngine::EndScene()
{
	m_renderers[ 0 ]->Present();
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

	renderer->UpdateSubresource( m_constantsPerFrame, &shader_data_per_frame );
	renderer->VSSetConstantBuffers( 0, m_constantsPerFrame );
	renderer->PSSetConstantBuffers( 0, m_constantsPerFrame );

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
		register DynamicMeshActor* object = meshes[i];

		// Ustawiamy bufor wierzcho�k�w
		if ( renderer->SetVertexBuffer( object->vertex_buffer ) )
			continue;	// Je�eli nie ma bufora wierzcho�k�w, to idziemy do nast�pnego mesha

		// Ustawiamy bufor indeks�w, je�eli istnieje
		renderer->SetIndexBuffer( object->index_buffer );


#ifdef _INTERPOLATE_POSITIONS
		XMMATRIX transformation = XMLoadFloat4x4( &(interpolated_matrixes[i]) );
#else
		XMVECTOR translation = XMLoadFloat3( &(object->position) );
		XMVECTOR orientation = XMLoadFloat4( &(object->orientation) );
		XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
		transformation = transformation * XMMatrixTranslationFromVector( translation );
#endif


		for ( unsigned int j = 0; j < object->model_parts.size( ); ++j )
		{
			ModelPart& model = object->model_parts[j];

			// Wyliczamy macierz transformacji
			XMMATRIX worldTransform;
			worldTransform = XMLoadFloat4x4( &(model.mesh->transform_matrix) );
			worldTransform = worldTransform * transformation;

			// Wype�niamy bufor sta�ych
			ConstantPerMesh shaderDataPerMesh;
			shaderDataPerMesh.world_matrix = XMMatrixTranspose( worldTransform );	// Transformacja wierzcho�k�w
			shaderDataPerMesh.mesh_scale = XMVectorSetW( XMVectorReplicate( object->scale ), 1.0f );

			// Przepisujemy materia�
			CopyMaterial( &shaderDataPerMesh, &model );

			// Ustawiamy shadery
			renderer->SetShaders( model );

			// Aktualizujemy bufor sta�ych
			renderer->UpdateSubresource( m_constantsPerMesh, &shaderDataPerMesh );
			renderer->VSSetConstantBuffers( 1, m_constantsPerMesh );
			renderer->PSSetConstantBuffers( 1, m_constantsPerMesh );

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
	XMVECTOR quaternion = current_camera->GetInterpolatedOrientation( time_lag );
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
	renderer->UpdateSubresource( m_constantsPerMesh, &shader_data_per_mesh );
	renderer->VSSetConstantBuffers( 1, m_constantsPerMesh );
	renderer->PSSetConstantBuffers( 1, m_constantsPerMesh );

	BufferObject* const_buffer = sky_dome->get_constant_buffer();	///< @todo Trzeba pobra� tutaj BufferObject zamiast ID3D11Buffer.
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
				register DynamicMeshActor* object = meshCollection[i];

				// Ustawiamy bufor wierzcho�k�w
				if ( renderer->SetVertexBuffer( object->vertex_buffer ) )
					continue;	// Je�eli nie ma bufora wierzcho�k�w, to idziemy do nast�pnego mesha


				XMVECTOR translation = object->GetPosition();
				XMVECTOR orientation = object->GetOrientation();
				XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
				transformation = transformation * XMMatrixTranslationFromVector( translation );

				for ( unsigned int j = 0; j < object->model_parts.size( ); ++j )
				{
					ModelPart& model = object->model_parts[j];

					// Wyliczamy macierz transformacji
					XMMATRIX worldTransform;
					worldTransform = XMLoadFloat4x4( &(model.mesh->transform_matrix) );
					worldTransform = worldTransform * transformation;

					// Wype�niamy bufor sta�ych
					ConstantPerMesh shaderDataPerMesh;
					shaderDataPerMesh.world_matrix = XMMatrixTranspose( worldTransform );	// Transformacja wierzcho�k�w
					shaderDataPerMesh.mesh_scale = XMVectorSetW( XMVectorReplicate( object->scale ), 1.0f );

					// Przepisujemy materia�
					CopyMaterial( &shaderDataPerMesh, &model );

					// Ustawiamy shadery
					renderer->SetShaders( model );

					// Aktualizujemy bufor sta�ych
					renderer->UpdateSubresource( m_constantsPerMesh, &shaderDataPerMesh );
					renderer->VSSetConstantBuffers( 1, m_constantsPerMesh );
					renderer->PSSetConstantBuffers( 1, m_constantsPerMesh );

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
@param[in] far_plane Dalsza p�aszczyzna obcinania*/
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
	if ( current_camera == nullptr )
		view_matrix  = XMMatrixIdentity( );	//tymczasowe
	else
	{
#ifndef _INTERPOLATE_POSITIONS
		//b�dziemy mno�y� macierz translacji * macierz obrotu
		view_matrix = XMMatrixTranslation(
			-current_camera->position.x,
			-current_camera->position.y,
			-current_camera->position.z);
		XMVECTOR quaternion = XMLoadFloat4(&(current_camera->orientation));
		quaternion = XMVectorNegate( quaternion );
		quaternion = XMVectorSetW( quaternion, -XMVectorGetW( quaternion ) );

		XMMATRIX rotation_matrix = XMMatrixRotationQuaternion(quaternion);
		view_matrix = view_matrix * rotation_matrix;
#else
		XMVECTOR position = current_camera->GetInterpolatedPosition( time_lag );
		XMVECTOR orientation = current_camera->GetInterpolatedOrientation( time_lag );
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

@param[in] object EngineObject, kt�ry ma zosta� dopisany do tablic wy�wietlania.*/
void DisplayEngine::AddDynamicMeshObject( DynamicMeshActor* object )
{
	realocate_interpolation_memory( );		//powi�kszamy tablic� macierzy interpolacji
							//wykona si� tylko je�eli jest konieczne
	meshes.push_back( object );
}

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
	
	current_camera = camera;
	return 0;
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
		DynamicMeshActor* object = meshes[i];
		interpolate_object2( time_lag, object, &(interpolated_matrixes[i]) );
	}
}


/**@deprecated
@brief Funkcja tworzy macierz przekszta�cenia dla podanego obiektu, interpoluj�c jego pozycj�
z pr�dko�ci post�powej i k�towej.

@param[in] tima_lag Czas jaki up�yn�� od ostatniego przeliczenia pozycji obiekt�w.
@param[in] object Objekt, dla kt�rego liczymy macierz przekszta�cenia.
@param[out] transform_matrix Zmienna, w kt�rej zostanie umieszczona interpolowana macierz przekszta�cenia.
*/
void DisplayEngine::interpolate_object( float time_lag, const DynamicActor* object, DirectX::XMFLOAT4X4* result_matrix )
{
	XMVECTOR position = object->GetPosition( );
	XMVECTOR orientation = object->GetOrientation( );
	XMVECTOR velocity = object->GetSpeed( );
	XMVECTOR rotation_velocity = object->GetRotationSpeed( );

	position += velocity * time_lag;

#ifdef _QUATERNION_SPEED
	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rotation_velocity = XMQuaternionMultiply( orientation, rotation_velocity );
	//teraz interpolujemy poprzedni� orientacj� i orientacj� po sekundzie
	//ze wsp�czynnikiem r�wnym czasowi jaki up�yn�� faktycznie
	orientation = XMQuaternionSlerp( orientation, rotation_velocity, timeLag );

	/*Du�o oblicze�, mo�e da si� to jako� za�atwi� bez interpolacji...*/
#else


	if ( !XMVector3Equal( rotation_velocity, XMVectorZero( ) ) )
	{
		float rot_angle = XMVectorGetW( rotation_velocity ) * time_lag;					// liczymy k�t obrotu
		rotation_velocity = XMQuaternionRotationAxis( rotation_velocity, rot_angle );	// przerabiamy na kwaternion
		orientation = XMQuaternionMultiply( orientation, rotation_velocity );			// liczymy nowy kwaternion orientacji
	}
#endif

	XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
	transformation = transformation * XMMatrixTranslationFromVector( position );

	XMStoreFloat4x4( result_matrix, transformation );
}

/**@brief Funkcja tworzy macierz przekszta�cenia dla podanego obiektu, interpoluj�c jego pozycj�
z pr�dko�ci post�powej i k�towej.

@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
@param[in] object Objekt, dla kt�rego liczymy macierz przekszta�cenia.
@param[out] transform_matrix Zmienna, w kt�rej zostanie umieszczona interpolowana macierz przekszta�cenia.
*/
void DisplayEngine::interpolate_object2( float time_lag, const DynamicActor* object, DirectX::XMFLOAT4X4* result_matrix )
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

