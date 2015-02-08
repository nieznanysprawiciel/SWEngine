#include "..\..\stdafx.h"
#include "DisplayEngine.h"
#include "..\Engine.h"


#include "..\..\memory_leaks.h"

DisplayEngine::DisplayEngine(Engine* engine)
	: engine(engine)
{
	current_camera = nullptr;

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
}

void DisplayEngine::init_const_buffers()
{
	init_buffers( sizeof(ConstantPerFrame), sizeof( ConstantPerMesh ));
}



/*Funkcja renderuj�ca obiekty 3d
 *Wywo�ania BeginScene() i EndScen() s� wykonywane przez klas� Engine.
 *Jest to konieczne, gdy chcemy renderowa� GUI za pomoc� obiektu UI_Engine.*/
void DisplayEngine::display_scene(float time_interval)
{
	set_view_matrix();

	// Ustawiamy format wierzcho�k�w i topologi�
	device_context->IASetInputLayout( mesh_vertex_format );
	device_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Ustawiamy bufor sta�y dla wszystkich meshy
	//shader_data_per_frame.time_lag = 

	device_context->UpdateSubresource( const_per_frame, 0, nullptr, &shader_data_per_frame, 0, 0 );
	device_context->VSSetConstantBuffers( 0, 1, &const_per_frame );
	device_context->PSSetConstantBuffers( 0, 1, &const_per_frame );

	// Ustawiamy smapler
	device_context->PSSetSamplers( 0, 1, &default_sampler );

	//na razie p�tla bez optymalizacji
	for ( unsigned int i = 0; i < meshes.size(); ++i )
	{
		register Dynamic_mesh_object* object = meshes[i];

		// Ustawiamy bufor wierzcho�k�w
		ID3D11Buffer* vertex_buffer = nullptr;
		if ( object->vertex_buffer )
		{
			vertex_buffer = object->vertex_buffer->get();
			unsigned int stride = object->vertex_buffer->get_stride();
			unsigned int offset = 0;
			device_context->IASetVertexBuffers( 0, 1, &vertex_buffer, &stride, &offset );
		}
		else
			continue;	// Je�eli nie ma bufora wierzcho�k�w, to idziemy do nast�pnego mesha

		// Ustawiamy bufor indeks�w, je�eli istnieje
		ID3D11Buffer* index_buffer = nullptr;
		if ( object->index_buffer )
		{
			index_buffer = object->index_buffer->get();
			unsigned int offset = 0;
			device_context->IASetIndexBuffer( index_buffer, INDEX_BUFFER_FORMAT, offset );
		}


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
			MaterialObject* material = model.material;
			shader_data_per_mesh.world_matrix = XMMatrixTranspose( world_transform );	// Transformacja wierzcho�k�w
				// Przepisujemy materia�
			shader_data_per_mesh.Diffuse = material->Diffuse;
			shader_data_per_mesh.Ambient = material->Ambient;
			shader_data_per_mesh.Specular = material->Specular;
			shader_data_per_mesh.Emissive = material->Emissive;
			shader_data_per_mesh.Power = material->Power;
			
			// Ustawiamy shadery
			device_context->VSSetShader( model.vertex_shader->get( ), nullptr, 0 );
			device_context->PSSetShader( model.pixel_shader->get( ), nullptr, 0 );

			// Aktualizujemy bufor sta�ych
			device_context->UpdateSubresource( const_per_mesh, 0, nullptr, &shader_data_per_mesh, 0, 0 );
			device_context->VSSetConstantBuffers( 1, 1, &const_per_mesh );
			device_context->PSSetConstantBuffers( 1, 1, &const_per_mesh );

			// Ustawiamy tekstury
			for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
				if ( model.texture[i] )		// Nie ka�da tekstura w tablicy istnieje
				{
					ID3D11ShaderResourceView* tex = model.texture[i]->get( );
					device_context->PSSetShaderResources( i, 1, &tex );
				}

			// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpo�redni.
			MeshPartObject* part = model.mesh;
			if ( part->use_index_buf )
				device_context->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
			else // Tryb bezpo�redni
				device_context->Draw( part->vertices_count, part->buffer_offset );
		}

	}
}


/*Tworzy macierz projekcji i zapami�tuje j� w polu projection_matrix klasy. W ka�dym wywo�aniu funkcji
display_scene ustawiana jest macierz zapisana w tym polu.*/
void DisplayEngine::set_projection_matrix(float angle, float X_to_Y, float near_plane, float far_plane)
{
	XMMATRIX proj_matrix = XMMatrixPerspectiveFovLH(angle, X_to_Y, near_plane, far_plane);
	proj_matrix = XMMatrixTranspose( proj_matrix );
	XMStoreFloat4x4( &shader_data_per_frame.projection_matrix, proj_matrix );
}


//=================================================================//
//					camera functions
//=================================================================//


/*U�ywa aktualnie ustawionej kamery, �eby stworzy� macierz widoku. Macierz widoku jest ustawiana w directX.
Funkcja jest prywatna, poniewa� jest wywo�ywana podczas renderowania sceny. Aby ingerowa� z zewn�trz
w ustawienie kamery nale�y zmieni� aktualnie ustawion� kamer� na jedn� z innych zapisanych w silniku.
*/
void DisplayEngine::set_view_matrix()
{
	XMMATRIX view_matrix;
	if ( current_camera == nullptr )
		view_matrix  = XMMatrixIdentity( );	//tymczasowe
	else
	{
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
	}

	//Wstawiamy macierz do zmiennej, kt�ra stanie si� potem zawarto�ci� bufora,
	//kt�ry zostanie przekazany do shadera.
	view_matrix = XMMatrixTranspose( view_matrix );
	XMStoreFloat4x4( &shader_data_per_frame.view_matrix, view_matrix );
}

void DisplayEngine::add_dynamic_mesh_object( Dynamic_mesh_object* object )
{
	realocate_interpolation_memory( );		//powi�kszamy tablic� macierzy interpolacji
							//wykona si� tylko je�eli jest konieczne
	meshes.push_back( object );
}


/*Dodaje kamer� do spisu kamer w silniku.
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

/*Powi�kszamy tablic� macierzy, w kt�rych umieszcza si� wyniki interpolacji
po�o�e� obiekt�w tu� przed ich wy�wietleniem w kolejnej klatce animacji.

W parametrze nale�y poda� minimaln� liczb� macierzy o jak� si� ma zwi�kszy� dotychczasowa
tablica.

Nie ma potrzeby przepisywania danych ze starej tablicy nowoutworzonej.
Wyniki s� niepotrzebne po ka�dym wy�wietleniu klatki, a ilo�� obiekt�w
w silniku nie mo�e si� zwi�kszy� mi�dzy interpolacj�, a wy�wietleniem.*/
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

/*Funkcja wykonuje interpolacj� po�o�e� dla wszystkich obiekt�w dynamicznych
w silniku. S� one przesuwane o tak� odleg�o�� jaka wynika z ich pr�dko�ci
oraz ze zmiennej time_lag, kt�ra oznacza czas, jaki up�yn�� od ostatniego
przeliczenia klatki.

Pozycj� z ostatniego wyliczenia klatki znajduj� si� w zmiennych position
i orientation obiekt�w dynamicznych. Docelowe macierze przekszta�ce� obiekt�w
zostan� umieszczone w tablicy interpolated_matrixes, w kt�rych indeks elementu
odpowiada indeksom w tablicy meshes.

[docelowo do wykonania wielow�tkowego]*/
void DisplayEngine::interpolate_positions( float time_lag )
{
	for ( unsigned int i = 0; i < meshes.size(); ++i )
	{
		Dynamic_mesh_object* object = meshes[i];

		XMVECTOR position = object->get_position();
		XMVECTOR orientation = object->get_orientation( );
		XMVECTOR velocity = object->get_speed();
		XMVECTOR rotation_velocity = object->get_rotation_speed();

		position += velocity*time_lag;

#ifdef _QUATERNION_SPEED
		//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
		rotation_velocity = XMQuaternionMultiply( orientation, rotation_velocity );
		//teraz interpolujemy poprzedni� orientacj� i orientacj� po sekundzie
		//ze wsp�czynnikiem r�wnym czasowi jaki up�yn�� faktycznie
		orientation = XMQuaternionSlerp( orientation, rotation_velocity, time_lag );

		/*Du�o oblicze�, mo�e da si� to jako� za�atwi� bez interpolacji...*/
#else


		if ( XMVector3Equal( rotation_velocity, XMVectorZero()) )
			orientation = XMQuaternionIdentity();
		else		
		{
			float rot_angle = XMVectorGetW( rotation_velocity ) * time_lag;					//liczymy k�t obrotu
			rotation_velocity = XMQuaternionRotationAxis( rotation_velocity, rot_angle );	//przerabiamy na kwaternion
			orientation = XMQuaternionMultiply( orientation, rotation_velocity );			//liczymy nowy kwaternion orientacji
		}		
#endif
		XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
#ifdef _SCALEABLE_OBJECTS
		//skalowanie ca�ego mesha
		transformation = XMMatrixScaling( object->scale, object->scale, object->scale ) * transformation;
#endif
		transformation = transformation * XMMatrixTranslationFromVector( position );
		XMStoreFloat4x4( &(interpolated_matrixes[i]), transformation );
	}
}


//=================================================================//
//					light functions
//=================================================================//

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

void DisplayEngine::set_ambient_light( const DirectX::XMFLOAT4& color )
{
	shader_data_per_frame.ambient_light = color;
}

#ifndef __UNUSED

//-------------------------------------------------------------------------------//
//							wersja DirectX 9
//-------------------------------------------------------------------------------//

void DisplayEngine::display_scene( float time_interval )
{
	LPDIRECT3DDEVICE9 directX_device = engine->directX_device;

	set_view_matrix();
	directX_device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&projection_matrix);	//macierz projekcji jest zawsze ta sama, chyba �e j� kto� jawnie zmodyfikuje

	directX_device->SetVertexShader( nullptr );
	directX_device->SetFVF(VERTEX_NORMAL_TEXTCORDS1);

	//parametry tekstur
	directX_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	directX_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	directX_device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	directX_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );


	//na razie p�tla bez optymalizacji
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		register Dynamic_mesh_object* object = meshes[i];

		#ifdef _INTERPOLATE_POSITIONS
		XMMATRIX transformation = XMLoadFloat4x4( &(interpolated_matrixes[i]) );
		#else
		XMVECTOR translation = XMLoadFloat3( &(object->position) );
		XMVECTOR orientation = XMLoadFloat4( &(object->orientation) );
		XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
		transformation = transformation * XMMatrixTranslationFromVector( translation );
		#endif

		for (unsigned int j = 0; j < object->materials.size(); ++j)
		{
			if ( object->materials[j] != nullptr )
				directX_device->SetMaterial( object->materials[j]->get_material() );
			else
				directX_device->SetMaterial( nullptr );

			if ( object->textures[j] != nullptr )
				directX_device->SetTexture( 0, object->textures[j]->get_texture() );
			else
				directX_device->SetTexture( 0, nullptr );

			XMMATRIX world_transform;
			if ( object->mesh_parts[j] != nullptr )
			{
				world_transform = XMLoadFloat4x4( &(object->mesh_parts[j]->transform_matrix) );
				world_transform = world_transform * transformation;
				//ustawiamy macierz przekszta�cenia dla danego modelu
				directX_device->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&world_transform );

				MeshObject* mesh = object->mesh_parts[j]->mesh_object;

				directX_device->SetStreamSource(
				0, mesh->vertex_buffer,
				0, sizeof( Vertex_Normal_TexCords1 ) );

				directX_device->DrawPrimitive( D3DPT_TRIANGLELIST, 0, mesh->primitives_count );
			}
		}
	}
}


#endif
