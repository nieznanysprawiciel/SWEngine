/**@file DisplayEngine.cpp
@brief Plik zawiera definicje metod klasy DispalyEngine.
*/


#include "stdafx.h"
#include "DisplayEngine.h"
#include "Engine.h"


#include "memory_leaks.h"

DisplayEngine::DisplayEngine(Engine* engine)
	: engine(engine)
{
	current_camera = nullptr;
	sky_dome = nullptr;

	interpol_matrixes_count = 16;
	interpolated_matrixes = new XMFLOAT4X4[interpol_matrixes_count];

	shader_data_per_frame.light_direction[0] = XMFLOAT4( 0, 0, 0, 0 );
	shader_data_per_frame.light_direction[1] = XMFLOAT4( 0, 0, 0, 0 );
	shader_data_per_frame.light_color[0] = XMFLOAT4( 0, 0, 0, 0 );
	shader_data_per_frame.light_color[1] = XMFLOAT4( 0, 0, 0, 0 );
	shader_data_per_frame.ambient_light = XMFLOAT4( 0, 0, 0, 0 );
}


DisplayEngine::~DisplayEngine()
{
	//jeszcze nie jestem pewien, kto odpowiada za zwalnianie kamery, ale na razie kto� musi
	//poniewa� kamera jest obiektem takim jak wszystkie inne, to pewnie b�dzie za to odpowiedzialna
	//klasa Engine
	for ( unsigned int i = 0; i < cameras.size(); ++i )
		delete cameras[i];

	//kasujemy tablic� interpolowanych macierzy
	delete[] interpolated_matrixes;

	if ( sky_dome )
		delete sky_dome;
}

/**@brief Inicjuje bufory sta�ych. Przy okazji inicjuje te� stany z-bufora do szybkiego
w��czania i wy��czania algorytmu.*/
void DisplayEngine::init_const_buffers()
{
	init_buffers( sizeof(ConstantPerFrame), sizeof( ConstantPerMesh ));
	init_depth_states();
}


//-------------------------------------------------------------------------------//
//							Funkcje pomocnicze do renderingu
//-------------------------------------------------------------------------------//

/**@brief Funkcja ustawia tesktury z danego ModelParta w DirectXie.

@param[in] model ModelPart z kt�rego pochodz� tekstury do ustawienia.
@todo SetShaderResource mo�na u�y� do ustawienia od razu ca�ej tablicy. Trzeba umo�liwi� ustawianie
do VS i innych.*/
void DisplayEngine::set_textures( const ModelPart& model )
{
	for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
		if ( model.texture[i] )		// Nie ka�da tekstura w tablicy istnieje
		{
			ID3D11ShaderResourceView* tex = model.texture[i]->get( );
			device_context->PSSetShaderResources( i, 1, &tex );
		}
}

/**@brief Ustawia w kontek�cie urz�dzenia bufor indeks�w.

@param[in] buffer Bufor do ustawienia.*/
void DisplayEngine::set_index_buffer( BufferObject* buffer )
{
	// Ustawiamy bufor indeks�w, je�eli istnieje
	ID3D11Buffer* index_buffer = nullptr;
	if ( buffer )
	{
		index_buffer = buffer->get( );
		unsigned int offset = 0;
		device_context->IASetIndexBuffer( index_buffer, INDEX_BUFFER_FORMAT, offset );
	}
}

/**@brief Ustawia w kontek�cie urz�dzenia bufor wierzcho�k�w.

@param[in] buffer Bufor do ustawienia.
@return Je�eli bufor nie istnieje to zwraca warto�� true. Inaczej false.
Wywo�anie if( set_vertex_buffer() ) ma zwr�ci� tak� warto��, �eby w ifie mo�na by�o
wywo�a� return lub continue, w przypadku braku bufora.*/
bool DisplayEngine::set_vertex_buffer( BufferObject* buffer )
{
	ID3D11Buffer* vertex_buffer = nullptr;
	if ( buffer )
	{
		vertex_buffer = buffer->get( );
		unsigned int stride = buffer->get_stride( );
		unsigned int offset = 0;
		device_context->IASetVertexBuffers( 0, 1, &vertex_buffer, &stride, &offset );

		return false;
	}
	return true;
}

/**@brief kopiuje materia� do struktury, kt�ra pos�u�y do zaktualizowania bufora sta�ych.

@param[in] shader_data_per_mesh Struktura docelowa.
@param[in] model ModelPart z kt�rego pobieramy dane.*/
void DisplayEngine::copy_material( ConstantPerMesh* shader_data_per_mesh, const ModelPart* model )
{
	MaterialObject* material = model->material;
	shader_data_per_mesh->Diffuse = material->Diffuse;
	shader_data_per_mesh->Ambient = material->Ambient;
	shader_data_per_mesh->Specular = material->Specular;
	shader_data_per_mesh->Emissive = material->Emissive;
	shader_data_per_mesh->Power = material->Power;
}

/**@brief Funkcja w��cza lub wy��cza z-bufor.

@param[in] state True je�eli z-bufor ma by� w��czony, false je�eli wy��czony.*/
void DisplayEngine::depth_buffer_enable( bool state )
{
	if ( state )
		device_context->OMSetDepthStencilState( depth_enabled, 1 );
	else
		device_context->OMSetDepthStencilState( depth_disabled, 1 );
}

//-------------------------------------------------------------------------------//
//							W�a�ciwe funkcje do renderingu
//-------------------------------------------------------------------------------//

/**@brief Funkcja do renderowania sceny

Poniewa� klasa UI_Engine tak�e wykonuje renderowanie( wy�wietla elementy interfejsu graficznego ),
funkcja display_engine nie mo�e sama wywo�ywa�  BeginScene( ) i EndScene( ) z bilbioteki directX, aby nie
by�o podwojnego wywo�ania. Z tego wzgl�du powy�sze funkcje s� wywo�ywane zewnetrznie w p�tli g��wnej przez klas� Engine.

Funkcja display_engine ma obowi�zek za ka�dym razem od nowa ustawi� macierz widoku i projekcji, poniewa�
mog� by� zmodyfikowane przez UI_Engine.Innymi s�owy nie mo�na za�o�y�, �e jak si� raz ustawi�o macierze,
to przy nast�pnym wywo�aniu b�d� takie same.

@param[in] time_interval Czas od ostatniej klatki. Przy ustawionej sta�ej @ref FIXED_FRAMES_COUNT czas ten jest sta�y
i wynosi tyle ile warto�� sta�ej FIXED_MOVE_UPDATE_INTERVAL.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::display_scene(float time_interval, float time_lag)
{
	set_view_matrix( time_lag );

	// Ustawiamy bufor sta�y dla wszystkich meshy
	//shader_data_per_frame.time_lag = 

	// Ustawiamy topologi�
	// D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
	// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	// D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	device_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	device_context->UpdateSubresource( const_per_frame, 0, nullptr, &shader_data_per_frame, 0, 0 );
	device_context->VSSetConstantBuffers( 0, 1, &const_per_frame );
	device_context->PSSetConstantBuffers( 0, 1, &const_per_frame );

	// Ustawiamy sampler
	device_context->PSSetSamplers( 0, 1, &default_sampler );

	// Zaczynamy wyswietlanie
	display_sky_box( time_interval, time_lag );

	// Ustawiamy format wierzcho�k�w
	device_context->IASetInputLayout( default_vertex_layout );

	display_instanced_meshes( time_interval, time_lag );
	display_dynamic_objects( time_interval, time_lag );
	display_particles( time_interval, time_lag );
	display_short_live_objects( time_interval, time_lag );
	display_skeletons( time_interval, time_lag );
	display_self_drawing_objects( time_interval, time_lag );
}

/**@brief Funkcja s�u�y do wy�wietlania meshy instancjonowanych, kt�re s� jednocze�niej obiektami statycznymi
w scenie.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::display_instanced_meshes( float time_interval, float time_lag )
{

}

/**@brief Funkcja s�u�y do wy�wietlania obiekt�w dynamicznych, kt�re s� rzadko niszczone.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::display_dynamic_objects( float time_interval, float time_lag )
{
	//na razie p�tla bez optymalizacji
	for ( unsigned int i = 0; i < meshes.size( ); ++i )
	{
		register Dynamic_mesh_object* object = meshes[i];

		// Ustawiamy bufor wierzcho�k�w
		if ( set_vertex_buffer( object->vertex_buffer ) )
			continue;	// Je�eli nie ma bufora wierzcho�k�w, to idziemy do nast�pnego mesha

		// Ustawiamy bufor indeks�w, je�eli istnieje
		set_index_buffer( object->index_buffer );


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
			XMMATRIX world_transform;
			world_transform = XMLoadFloat4x4( &(model.mesh->transform_matrix) );
			world_transform = world_transform * transformation;

			// Wype�niamy bufor sta�ych
			ConstantPerMesh shader_data_per_mesh;
			shader_data_per_mesh.world_matrix = XMMatrixTranspose( world_transform );	// Transformacja wierzcho�k�w
			shader_data_per_mesh.mesh_scale = XMVectorSetW( XMVectorReplicate( object->scale ), 1.0f );

			// Przepisujemy materia�
			copy_material( &shader_data_per_mesh, &model );

			// Ustawiamy shadery
			device_context->VSSetShader( model.vertex_shader->get( ), nullptr, 0 );
			device_context->PSSetShader( model.pixel_shader->get( ), nullptr, 0 );

			// Aktualizujemy bufor sta�ych
			device_context->UpdateSubresource( const_per_mesh, 0, nullptr, &shader_data_per_mesh, 0, 0 );
			device_context->VSSetConstantBuffers( 1, 1, &const_per_mesh );
			device_context->PSSetConstantBuffers( 1, 1, &const_per_mesh );

			// Ustawiamy tekstury
			set_textures( model );

			// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpo�redni.
			MeshPartObject* part = model.mesh;
			if ( part->use_index_buf )
				device_context->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
			else // Tryb bezpo�redni
				device_context->Draw( part->vertices_count, part->buffer_offset );
		}

	}
}

/**@brief Funkcja s�u�y do wy�wietlania system�w cz�stek.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::display_particles( float time_interval, float time_lag )
{

}

/**@brief Funkcja s�u�y do wy�wietlania obiekt�w dynamicznych o kr�tkim czasie �ycia
jak np. pociski.

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::display_short_live_objects( float time_interval, float time_lag )
{


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
void DisplayEngine::display_sky_box( float time_interval, float time_lag )
{
	if ( !sky_dome )
		return;

	// Ustawiamy format wierzcho�k�w
	device_context->IASetInputLayout( sky_dome->get_vertex_layout() );

	// Aktualizuje bufor wierzcho�k�w. Wstawiane s� nowe kolory.
	// Powinna by� to raczej rzadka czynno��, poniewa� aktualizacja jest kosztowna czasowo
	if ( sky_dome->update_vertex_buffer )
		sky_dome->update_buffers();

	// Ustawiamy bufor wierzcho�k�w
	if ( set_vertex_buffer( sky_dome->get_vertex_buffer() ) )
		return;	// Je�eli nie ma bufora wierzcho�k�w, to idziemy do nast�pnego mesha
	// Ustawiamy bufor indeks�w, je�eli istnieje
	set_index_buffer( sky_dome->get_index_buffer() );


	ModelPart* model = sky_dome->get_model_part();

	// Wyliczamy macierz transformacji
	XMVECTOR quaternion = current_camera->get_interpolated_orientation( time_lag );
	inverse_camera_orientation( quaternion );

	XMMATRIX rotation_matrix = XMMatrixRotationQuaternion( quaternion );

	// Wype�niamy bufor sta�ych
	ConstantPerMesh shader_data_per_mesh;
	shader_data_per_mesh.world_matrix = XMMatrixTranspose( rotation_matrix );	// Transformacja wierzcho�k�w
	// Przepisujemy materia�
	copy_material( &shader_data_per_mesh, model );

	// Ustawiamy shadery
	device_context->VSSetShader( model->vertex_shader->get( ), nullptr, 0 );
	device_context->PSSetShader( model->pixel_shader->get( ), nullptr, 0 );

	// Aktualizujemy bufor sta�ych
	device_context->UpdateSubresource( const_per_mesh, 0, nullptr, &shader_data_per_mesh, 0, 0 );
	device_context->VSSetConstantBuffers( 1, 1, &const_per_mesh );
	device_context->PSSetConstantBuffers( 1, 1, &const_per_mesh );

	ID3D11Buffer* const_buffer = sky_dome->get_constant_buffer();
	device_context->VSSetConstantBuffers( 2, 1, &const_buffer );
	device_context->PSSetConstantBuffers( 2, 1, &const_buffer );

	// Ustawiamy tekstury
	set_textures( *model );

	depth_buffer_enable( false );		// Wy��czamy z-bufor

	// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpo�redni.
	const MeshPartObject* part = model->mesh;
	if ( part->use_index_buf )
		device_context->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
	else // Tryb bezpo�redni
		device_context->Draw( part->vertices_count, part->buffer_offset );

	depth_buffer_enable( true );		// W��czamy z-bufor spowrotem
}

/**@brief Funkcja s�u�y do wy�wietlania obiekt�w z animacj� szkieletow�, czyli g��wnie postaci


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::display_skeletons( float time_interval, float time_lag )
{

}

/**@brief Funkcja wy�wietla obiekty, kt�re maj� zaimplementowany inny
ni� domy�lny algorytm wyswietlania.

@see @ref self_drawing_objects

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U�amek czasu jaki up�yn�� mi�dzy ostani� klatk� a nast�pn�.
Zakres [0,1].
*/
void DisplayEngine::display_self_drawing_objects( float time_interval, float time_lag )
{

}

/**@brief Tworzy macierz projekcji i zapami�tuje j� w polu projection_matrix klasy. W ka�dym wywo�aniu funkcji
display_scene ustawiana jest macierz zapisana w tym polu.
@param[in] angle K�t widzenia w pionie
@param[in] X_to_Y Stosunek Szeroko�ci do wysoko�ci ekranu
@param[in] near_plane Bli�sza p�aszczyzna obcinania
@param[in] far_plane Dalsza p�aszczyzna obcinania*/
void DisplayEngine::set_projection_matrix(float angle, float X_to_Y, float near_plane, float far_plane)
{
	XMMATRIX proj_matrix = XMMatrixPerspectiveFovLH(angle, X_to_Y, near_plane, far_plane);
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
void DisplayEngine::set_view_matrix( float time_lag )
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
		XMVECTOR position = current_camera->get_interpolated_position( time_lag );
		XMVECTOR orientation = current_camera->get_interpolated_orientation( time_lag );
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

@param[in] object Object, kt�ry ma zosta� dopisany do tablic wy�wietlania.*/
void DisplayEngine::add_dynamic_mesh_object( Dynamic_mesh_object* object )
{
	realocate_interpolation_memory( );		//powi�kszamy tablic� macierzy interpolacji
							//wykona si� tylko je�eli jest konieczne
	meshes.push_back( object );
}


/**@brief Dodaje kamer� do spisu kamer w silniku.

@param[in] camera Kamera do dodania.
@return
Funkcja zwraca 0 w przypadku powodzenia.
Je�eli kamera ju� istnia�a wczesniej, to zwracan� warto�ci� jest 1.
Je�eli podano wska�nik nullptr, zwr�cona zostanie warto�� 2.*/
int DisplayEngine::add_camera( Camera_object* camera )
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
int DisplayEngine::set_current_camera( Camera_object* camera )
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
		interpolated_matrixes = new XMFLOAT4X4[interpol_matrixes_count];
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
void DisplayEngine::interpolate_positions( float time_lag )
{
	for ( unsigned int i = 0; i < meshes.size(); ++i )
	{
		Dynamic_mesh_object* object = meshes[i];
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
void DisplayEngine::interpolate_object( float time_lag, const Dynamic_object* object, DirectX::XMFLOAT4X4* result_matrix )
{
	XMVECTOR position = object->get_position( );
	XMVECTOR orientation = object->get_orientation( );
	XMVECTOR velocity = object->get_speed( );
	XMVECTOR rotation_velocity = object->get_rotation_speed( );

	position += velocity * time_lag;

#ifdef _QUATERNION_SPEED
	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rotation_velocity = XMQuaternionMultiply( orientation, rotation_velocity );
	//teraz interpolujemy poprzedni� orientacj� i orientacj� po sekundzie
	//ze wsp�czynnikiem r�wnym czasowi jaki up�yn�� faktycznie
	orientation = XMQuaternionSlerp( orientation, rotation_velocity, time_lag );

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
void DisplayEngine::interpolate_object2( float time_lag, const Dynamic_object* object, DirectX::XMFLOAT4X4* result_matrix )
{
	XMVECTOR position = object->get_interpolated_position( time_lag );
	XMVECTOR orientation = object->get_interpolated_orientation( time_lag );

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
int DisplayEngine::set_directional_light( const DirectX::XMFLOAT4& direction,
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
void DisplayEngine::set_ambient_light( const DirectX::XMFLOAT4& color )
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
SkyDome* DisplayEngine::set_skydome( SkyDome* dome )
{
	SkyDome* old = sky_dome;
	sky_dome = dome;
	return old;
}

