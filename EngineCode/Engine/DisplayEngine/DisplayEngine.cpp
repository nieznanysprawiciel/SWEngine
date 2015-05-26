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
	//jeszcze nie jestem pewien, kto odpowiada za zwalnianie kamery, ale na razie ktoœ musi
	//poniewa¿ kamera jest obiektem takim jak wszystkie inne, to pewnie bêdzie za to odpowiedzialna
	//klasa Engine
	for ( unsigned int i = 0; i < cameras.size(); ++i )
		delete cameras[i];

	//kasujemy tablicê interpolowanych macierzy
	delete[] interpolated_matrixes;

	if ( sky_dome )
		delete sky_dome;
}

/**@brief Inicjuje bufory sta³ych. Przy okazji inicjuje te¿ stany z-bufora do szybkiego
w³¹czania i wy³¹czania algorytmu.*/
void DisplayEngine::init_const_buffers()
{
	init_buffers( sizeof(ConstantPerFrame), sizeof( ConstantPerMesh ));
	init_depth_states();
}


//-------------------------------------------------------------------------------//
//							Funkcje pomocnicze do renderingu
//-------------------------------------------------------------------------------//

/**@brief Funkcja ustawia tesktury z danego ModelParta w DirectXie.

@param[in] model ModelPart z którego pochodz¹ tekstury do ustawienia.
@todo SetShaderResource mo¿na u¿yæ do ustawienia od razu ca³ej tablicy. Trzeba umo¿liwiæ ustawianie
do VS i innych.*/
void DisplayEngine::set_textures( const ModelPart& model )
{
	for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
		if ( model.texture[i] )		// Nie ka¿da tekstura w tablicy istnieje
		{
			ID3D11ShaderResourceView* tex = model.texture[i]->get( );
			device_context->PSSetShaderResources( i, 1, &tex );
		}
}

/**@brief Ustawia w kontekœcie urz¹dzenia bufor indeksów.

@param[in] buffer Bufor do ustawienia.*/
void DisplayEngine::set_index_buffer( BufferObject* buffer )
{
	// Ustawiamy bufor indeksów, je¿eli istnieje
	ID3D11Buffer* index_buffer = nullptr;
	if ( buffer )
	{
		index_buffer = buffer->get( );
		unsigned int offset = 0;
		device_context->IASetIndexBuffer( index_buffer, INDEX_BUFFER_FORMAT, offset );
	}
}

/**@brief Ustawia w kontekœcie urz¹dzenia bufor wierzcho³ków.

@param[in] buffer Bufor do ustawienia.
@return Je¿eli bufor nie istnieje to zwraca wartoœæ true. Inaczej false.
Wywo³anie if( set_vertex_buffer() ) ma zwróciæ tak¹ wartoœæ, ¿eby w ifie mo¿na by³o
wywo³aæ return lub continue, w przypadku braku bufora.*/
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

/**@brief kopiuje materia³ do struktury, która pos³u¿y do zaktualizowania bufora sta³ych.

@param[in] shader_data_per_mesh Struktura docelowa.
@param[in] model ModelPart z którego pobieramy dane.*/
void DisplayEngine::copy_material( ConstantPerMesh* shader_data_per_mesh, const ModelPart* model )
{
	MaterialObject* material = model->material;
	shader_data_per_mesh->Diffuse = material->Diffuse;
	shader_data_per_mesh->Ambient = material->Ambient;
	shader_data_per_mesh->Specular = material->Specular;
	shader_data_per_mesh->Emissive = material->Emissive;
	shader_data_per_mesh->Power = material->Power;
}

/**@brief Funkcja w³¹cza lub wy³¹cza z-bufor.

@param[in] state True je¿eli z-bufor ma byæ w³¹czony, false je¿eli wy³¹czony.*/
void DisplayEngine::depth_buffer_enable( bool state )
{
	if ( state )
		device_context->OMSetDepthStencilState( depth_enabled, 1 );
	else
		device_context->OMSetDepthStencilState( depth_disabled, 1 );
}

//-------------------------------------------------------------------------------//
//							W³aœciwe funkcje do renderingu
//-------------------------------------------------------------------------------//

/**@brief Funkcja do renderowania sceny

Poniewa¿ klasa UI_Engine tak¿e wykonuje renderowanie( wyœwietla elementy interfejsu graficznego ),
funkcja display_engine nie mo¿e sama wywo³ywaæ  BeginScene( ) i EndScene( ) z bilbioteki directX, aby nie
by³o podwojnego wywo³ania. Z tego wzglêdu powy¿sze funkcje s¹ wywo³ywane zewnetrznie w pêtli g³ównej przez klasê Engine.

Funkcja display_engine ma obowi¹zek za ka¿dym razem od nowa ustawiæ macierz widoku i projekcji, poniewa¿
mog¹ byæ zmodyfikowane przez UI_Engine.Innymi s³owy nie mo¿na za³o¿yæ, ¿e jak siê raz ustawi³o macierze,
to przy nastêpnym wywo³aniu bêd¹ takie same.

@param[in] time_interval Czas od ostatniej klatki. Przy ustawionej sta³ej @ref FIXED_FRAMES_COUNT czas ten jest sta³y
i wynosi tyle ile wartoœæ sta³ej FIXED_MOVE_UPDATE_INTERVAL.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::display_scene(float time_interval, float time_lag)
{
	set_view_matrix( time_lag );

	// Ustawiamy bufor sta³y dla wszystkich meshy
	//shader_data_per_frame.time_lag = 

	// Ustawiamy topologiê
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

	// Ustawiamy format wierzcho³ków
	device_context->IASetInputLayout( default_vertex_layout );

	display_instanced_meshes( time_interval, time_lag );
	display_dynamic_objects( time_interval, time_lag );
	display_particles( time_interval, time_lag );
	display_short_live_objects( time_interval, time_lag );
	display_skeletons( time_interval, time_lag );
	display_self_drawing_objects( time_interval, time_lag );
}

/**@brief Funkcja s³u¿y do wyœwietlania meshy instancjonowanych, które s¹ jednoczeœniej obiektami statycznymi
w scenie.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::display_instanced_meshes( float time_interval, float time_lag )
{

}

/**@brief Funkcja s³u¿y do wyœwietlania obiektów dynamicznych, które s¹ rzadko niszczone.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::display_dynamic_objects( float time_interval, float time_lag )
{
	//na razie pêtla bez optymalizacji
	for ( unsigned int i = 0; i < meshes.size( ); ++i )
	{
		register Dynamic_mesh_object* object = meshes[i];

		// Ustawiamy bufor wierzcho³ków
		if ( set_vertex_buffer( object->vertex_buffer ) )
			continue;	// Je¿eli nie ma bufora wierzcho³ków, to idziemy do nastêpnego mesha

		// Ustawiamy bufor indeksów, je¿eli istnieje
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

			// Wype³niamy bufor sta³ych
			ConstantPerMesh shader_data_per_mesh;
			shader_data_per_mesh.world_matrix = XMMatrixTranspose( world_transform );	// Transformacja wierzcho³ków
			shader_data_per_mesh.mesh_scale = XMVectorSetW( XMVectorReplicate( object->scale ), 1.0f );

			// Przepisujemy materia³
			copy_material( &shader_data_per_mesh, &model );

			// Ustawiamy shadery
			device_context->VSSetShader( model.vertex_shader->get( ), nullptr, 0 );
			device_context->PSSetShader( model.pixel_shader->get( ), nullptr, 0 );

			// Aktualizujemy bufor sta³ych
			device_context->UpdateSubresource( const_per_mesh, 0, nullptr, &shader_data_per_mesh, 0, 0 );
			device_context->VSSetConstantBuffers( 1, 1, &const_per_mesh );
			device_context->PSSetConstantBuffers( 1, 1, &const_per_mesh );

			// Ustawiamy tekstury
			set_textures( model );

			// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpoœredni.
			MeshPartObject* part = model.mesh;
			if ( part->use_index_buf )
				device_context->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
			else // Tryb bezpoœredni
				device_context->Draw( part->vertices_count, part->buffer_offset );
		}

	}
}

/**@brief Funkcja s³u¿y do wyœwietlania systemów cz¹stek.


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::display_particles( float time_interval, float time_lag )
{

}

/**@brief Funkcja s³u¿y do wyœwietlania obiektów dynamicznych o krótkim czasie ¿ycia
jak np. pociski.

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::display_short_live_objects( float time_interval, float time_lag )
{


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
void DisplayEngine::display_sky_box( float time_interval, float time_lag )
{
	if ( !sky_dome )
		return;

	// Ustawiamy format wierzcho³ków
	device_context->IASetInputLayout( sky_dome->get_vertex_layout() );

	// Aktualizuje bufor wierzcho³ków. Wstawiane s¹ nowe kolory.
	// Powinna byæ to raczej rzadka czynnoœæ, poniewa¿ aktualizacja jest kosztowna czasowo
	if ( sky_dome->update_vertex_buffer )
		sky_dome->update_buffers();

	// Ustawiamy bufor wierzcho³ków
	if ( set_vertex_buffer( sky_dome->get_vertex_buffer() ) )
		return;	// Je¿eli nie ma bufora wierzcho³ków, to idziemy do nastêpnego mesha
	// Ustawiamy bufor indeksów, je¿eli istnieje
	set_index_buffer( sky_dome->get_index_buffer() );


	ModelPart* model = sky_dome->get_model_part();

	// Wyliczamy macierz transformacji
	XMVECTOR quaternion = current_camera->get_interpolated_orientation( time_lag );
	inverse_camera_orientation( quaternion );

	XMMATRIX rotation_matrix = XMMatrixRotationQuaternion( quaternion );

	// Wype³niamy bufor sta³ych
	ConstantPerMesh shader_data_per_mesh;
	shader_data_per_mesh.world_matrix = XMMatrixTranspose( rotation_matrix );	// Transformacja wierzcho³ków
	// Przepisujemy materia³
	copy_material( &shader_data_per_mesh, model );

	// Ustawiamy shadery
	device_context->VSSetShader( model->vertex_shader->get( ), nullptr, 0 );
	device_context->PSSetShader( model->pixel_shader->get( ), nullptr, 0 );

	// Aktualizujemy bufor sta³ych
	device_context->UpdateSubresource( const_per_mesh, 0, nullptr, &shader_data_per_mesh, 0, 0 );
	device_context->VSSetConstantBuffers( 1, 1, &const_per_mesh );
	device_context->PSSetConstantBuffers( 1, 1, &const_per_mesh );

	ID3D11Buffer* const_buffer = sky_dome->get_constant_buffer();
	device_context->VSSetConstantBuffers( 2, 1, &const_buffer );
	device_context->PSSetConstantBuffers( 2, 1, &const_buffer );

	// Ustawiamy tekstury
	set_textures( *model );

	depth_buffer_enable( false );		// Wy³¹czamy z-bufor

	// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpoœredni.
	const MeshPartObject* part = model->mesh;
	if ( part->use_index_buf )
		device_context->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
	else // Tryb bezpoœredni
		device_context->Draw( part->vertices_count, part->buffer_offset );

	depth_buffer_enable( true );		// W³¹czamy z-bufor spowrotem
}

/**@brief Funkcja s³u¿y do wyœwietlania obiektów z animacj¹ szkieletow¹, czyli g³ównie postaci


@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::display_skeletons( float time_interval, float time_lag )
{

}

/**@brief Funkcja wyœwietla obiekty, które maj¹ zaimplementowany inny
ni¿ domyœlny algorytm wyswietlania.

@see @ref self_drawing_objects

@param[in] time_interval Czas od ostatniej klatki.
@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
*/
void DisplayEngine::display_self_drawing_objects( float time_interval, float time_lag )
{

}

/**@brief Tworzy macierz projekcji i zapamiêtuje j¹ w polu projection_matrix klasy. W ka¿dym wywo³aniu funkcji
display_scene ustawiana jest macierz zapisana w tym polu.
@param[in] angle K¹t widzenia w pionie
@param[in] X_to_Y Stosunek Szerokoœci do wysokoœci ekranu
@param[in] near_plane Bli¿sza p³aszczyzna obcinania
@param[in] far_plane Dalsza p³aszczyzna obcinania*/
void DisplayEngine::set_projection_matrix(float angle, float X_to_Y, float near_plane, float far_plane)
{
	XMMATRIX proj_matrix = XMMatrixPerspectiveFovLH(angle, X_to_Y, near_plane, far_plane);
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
void DisplayEngine::set_view_matrix( float time_lag )
{
	XMMATRIX view_matrix;
	if ( current_camera == nullptr )
		view_matrix  = XMMatrixIdentity( );	//tymczasowe
	else
	{
#ifndef _INTERPOLATE_POSITIONS
		//bêdziemy mno¿yæ macierz translacji * macierz obrotu
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

	//Wstawiamy macierz do zmiennej, która stanie siê potem zawartoœci¹ bufora,
	//który zostanie przekazany do shadera.
	view_matrix = XMMatrixTranspose( view_matrix );
	XMStoreFloat4x4( &shader_data_per_frame.view_matrix, view_matrix );
}

/**@brief Dodaje obiekt, który ma zostaæ wyœwietlony.

@param[in] object Object, który ma zostaæ dopisany do tablic wyœwietlania.*/
void DisplayEngine::add_dynamic_mesh_object( Dynamic_mesh_object* object )
{
	realocate_interpolation_memory( );		//powiêkszamy tablicê macierzy interpolacji
							//wykona siê tylko je¿eli jest konieczne
	meshes.push_back( object );
}


/**@brief Dodaje kamerê do spisu kamer w silniku.

@param[in] camera Kamera do dodania.
@return
Funkcja zwraca 0 w przypadku powodzenia.
Je¿eli kamera ju¿ istnia³a wczesniej, to zwracan¹ wartoœci¹ jest 1.
Je¿eli podano wskaŸnik nullptr, zwrócona zostanie wartoœæ 2.*/
int DisplayEngine::add_camera( Camera_object* camera )
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
		interpolated_matrixes = new XMFLOAT4X4[interpol_matrixes_count];
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
void DisplayEngine::interpolate_positions( float time_lag )
{
	for ( unsigned int i = 0; i < meshes.size(); ++i )
	{
		Dynamic_mesh_object* object = meshes[i];
		interpolate_object2( time_lag, object, &(interpolated_matrixes[i]) );
	}
}


/**@deprecated
@brief Funkcja tworzy macierz przekszta³cenia dla podanego obiektu, interpoluj¹c jego pozycjê
z prêdkoœci postêpowej i k¹towej.

@param[in] tima_lag Czas jaki up³yn¹³ od ostatniego przeliczenia pozycji obiektów.
@param[in] object Objekt, dla którego liczymy macierz przekszta³cenia.
@param[out] transform_matrix Zmienna, w której zostanie umieszczona interpolowana macierz przekszta³cenia.
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
	//teraz interpolujemy poprzedni¹ orientacjê i orientacjê po sekundzie
	//ze wspóczynnikiem równym czasowi jaki up³yn¹³ faktycznie
	orientation = XMQuaternionSlerp( orientation, rotation_velocity, time_lag );

	/*Du¿o obliczeñ, mo¿e da siê to jakoœ za³atwiæ bez interpolacji...*/
#else


	if ( !XMVector3Equal( rotation_velocity, XMVectorZero( ) ) )
	{
		float rot_angle = XMVectorGetW( rotation_velocity ) * time_lag;					// liczymy k¹t obrotu
		rotation_velocity = XMQuaternionRotationAxis( rotation_velocity, rot_angle );	// przerabiamy na kwaternion
		orientation = XMQuaternionMultiply( orientation, rotation_velocity );			// liczymy nowy kwaternion orientacji
	}
#endif

	XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
	transformation = transformation * XMMatrixTranslationFromVector( position );

	XMStoreFloat4x4( result_matrix, transformation );
}

/**@brief Funkcja tworzy macierz przekszta³cenia dla podanego obiektu, interpoluj¹c jego pozycjê
z prêdkoœci postêpowej i k¹towej.

@param[in] time_lag U³amek czasu jaki up³yn¹³ miêdzy ostani¹ klatk¹ a nastêpn¹.
Zakres [0,1].
@param[in] object Objekt, dla którego liczymy macierz przekszta³cenia.
@param[out] transform_matrix Zmienna, w której zostanie umieszczona interpolowana macierz przekszta³cenia.
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

/**@brief Ustawia œwiat³o kierunkowe
@param[in] direction Kierunek œwiecenia œwiat³a
@param[in] color Kolor œwiat³a
@param[in] index Indeks œwiat³a w tablicy œwiate³. Maksymalnie wynosi @ref ENGINE_MAX_LIGHTS - 1.
@return 0 w przypadku powodzenia, -1 je¿eli jest niepoprawny indeks.
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
	light_dir = XMVectorNegate( light_dir );		// Robimy to, ¿eby shader nie musia³ odwracaæ
	XMStoreFloat4( &shader_data_per_frame.light_direction[index], light_dir );
	
	// Przycinamy wektor do przedzia³u [0.0 , 1.0]
	light_color = XMVectorSaturate( light_color );
	XMStoreFloat4( &shader_data_per_frame.light_color[index], light_color );

	return 0;
}

/**@brief Ustawia œwiat³o ambient.
@param[in] color Kolor œwiat³a.*/
void DisplayEngine::set_ambient_light( const DirectX::XMFLOAT4& color )
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
SkyDome* DisplayEngine::set_skydome( SkyDome* dome )
{
	SkyDome* old = sky_dome;
	sky_dome = dome;
	return old;
}

