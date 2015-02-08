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
	//jeszcze nie jestem pewien, kto odpowiada za zwalnianie kamery, ale na razie ktoœ musi
	//poniewa¿ kamera jest obiektem takim jak wszystkie inne, to pewnie bêdzie za to odpowiedzialna
	//klasa Engine
	for ( unsigned int i = 0; i < cameras.size(); ++i )
		delete cameras[i];

	//kasujemy tablicê interpolowanych macierzy
	delete[] interpolated_matrixes;
}

void DisplayEngine::init_const_buffers()
{
	init_buffers( sizeof(ConstantPerFrame), sizeof( ConstantPerMesh ));
}



/*Funkcja renderuj¹ca obiekty 3d
 *Wywo³ania BeginScene() i EndScen() s¹ wykonywane przez klasê Engine.
 *Jest to konieczne, gdy chcemy renderowaæ GUI za pomoc¹ obiektu UI_Engine.*/
void DisplayEngine::display_scene(float time_interval)
{
	set_view_matrix();

	// Ustawiamy format wierzcho³ków i topologiê
	device_context->IASetInputLayout( mesh_vertex_format );
	device_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Ustawiamy bufor sta³y dla wszystkich meshy
	//shader_data_per_frame.time_lag = 

	device_context->UpdateSubresource( const_per_frame, 0, nullptr, &shader_data_per_frame, 0, 0 );
	device_context->VSSetConstantBuffers( 0, 1, &const_per_frame );
	device_context->PSSetConstantBuffers( 0, 1, &const_per_frame );

	// Ustawiamy smapler
	device_context->PSSetSamplers( 0, 1, &default_sampler );

	//na razie pêtla bez optymalizacji
	for ( unsigned int i = 0; i < meshes.size(); ++i )
	{
		register Dynamic_mesh_object* object = meshes[i];

		// Ustawiamy bufor wierzcho³ków
		ID3D11Buffer* vertex_buffer = nullptr;
		if ( object->vertex_buffer )
		{
			vertex_buffer = object->vertex_buffer->get();
			unsigned int stride = object->vertex_buffer->get_stride();
			unsigned int offset = 0;
			device_context->IASetVertexBuffers( 0, 1, &vertex_buffer, &stride, &offset );
		}
		else
			continue;	// Je¿eli nie ma bufora wierzcho³ków, to idziemy do nastêpnego mesha

		// Ustawiamy bufor indeksów, je¿eli istnieje
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

			// Wype³niamy bufor sta³ych
			ConstantPerMesh shader_data_per_mesh;
			MaterialObject* material = model.material;
			shader_data_per_mesh.world_matrix = XMMatrixTranspose( world_transform );	// Transformacja wierzcho³ków
				// Przepisujemy materia³
			shader_data_per_mesh.Diffuse = material->Diffuse;
			shader_data_per_mesh.Ambient = material->Ambient;
			shader_data_per_mesh.Specular = material->Specular;
			shader_data_per_mesh.Emissive = material->Emissive;
			shader_data_per_mesh.Power = material->Power;
			
			// Ustawiamy shadery
			device_context->VSSetShader( model.vertex_shader->get( ), nullptr, 0 );
			device_context->PSSetShader( model.pixel_shader->get( ), nullptr, 0 );

			// Aktualizujemy bufor sta³ych
			device_context->UpdateSubresource( const_per_mesh, 0, nullptr, &shader_data_per_mesh, 0, 0 );
			device_context->VSSetConstantBuffers( 1, 1, &const_per_mesh );
			device_context->PSSetConstantBuffers( 1, 1, &const_per_mesh );

			// Ustawiamy tekstury
			for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
				if ( model.texture[i] )		// Nie ka¿da tekstura w tablicy istnieje
				{
					ID3D11ShaderResourceView* tex = model.texture[i]->get( );
					device_context->PSSetShaderResources( i, 1, &tex );
				}

			// Teraz renderujemy. Wybieramy albo tryb indeksowany, albo bezpoœredni.
			MeshPartObject* part = model.mesh;
			if ( part->use_index_buf )
				device_context->DrawIndexed( part->vertices_count, part->buffer_offset, part->base_vertex );
			else // Tryb bezpoœredni
				device_context->Draw( part->vertices_count, part->buffer_offset );
		}

	}
}


/*Tworzy macierz projekcji i zapamiêtuje j¹ w polu projection_matrix klasy. W ka¿dym wywo³aniu funkcji
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


/*U¿ywa aktualnie ustawionej kamery, ¿eby stworzyæ macierz widoku. Macierz widoku jest ustawiana w directX.
Funkcja jest prywatna, poniewa¿ jest wywo³ywana podczas renderowania sceny. Aby ingerowaæ z zewn¹trz
w ustawienie kamery nale¿y zmieniæ aktualnie ustawion¹ kamerê na jedn¹ z innych zapisanych w silniku.
*/
void DisplayEngine::set_view_matrix()
{
	XMMATRIX view_matrix;
	if ( current_camera == nullptr )
		view_matrix  = XMMatrixIdentity( );	//tymczasowe
	else
	{
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
	}

	//Wstawiamy macierz do zmiennej, która stanie siê potem zawartoœci¹ bufora,
	//który zostanie przekazany do shadera.
	view_matrix = XMMatrixTranspose( view_matrix );
	XMStoreFloat4x4( &shader_data_per_frame.view_matrix, view_matrix );
}

void DisplayEngine::add_dynamic_mesh_object( Dynamic_mesh_object* object )
{
	realocate_interpolation_memory( );		//powiêkszamy tablicê macierzy interpolacji
							//wykona siê tylko je¿eli jest konieczne
	meshes.push_back( object );
}


/*Dodaje kamerê do spisu kamer w silniku.
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

/*Powiêkszamy tablicê macierzy, w których umieszcza siê wyniki interpolacji
po³o¿eñ obiektów tu¿ przed ich wyœwietleniem w kolejnej klatce animacji.

W parametrze nale¿y podaæ minimaln¹ liczbê macierzy o jak¹ siê ma zwiêkszyæ dotychczasowa
tablica.

Nie ma potrzeby przepisywania danych ze starej tablicy nowoutworzonej.
Wyniki s¹ niepotrzebne po ka¿dym wyœwietleniu klatki, a iloœæ obiektów
w silniku nie mo¿e siê zwiêkszyæ miêdzy interpolacj¹, a wyœwietleniem.*/
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

/*Funkcja wykonuje interpolacjê po³o¿eñ dla wszystkich obiektów dynamicznych
w silniku. S¹ one przesuwane o tak¹ odleg³oœæ jaka wynika z ich prêdkoœci
oraz ze zmiennej time_lag, która oznacza czas, jaki up³yn¹³ od ostatniego
przeliczenia klatki.

Pozycjê z ostatniego wyliczenia klatki znajduj¹ siê w zmiennych position
i orientation obiektów dynamicznych. Docelowe macierze przekszta³ceñ obiektów
zostan¹ umieszczone w tablicy interpolated_matrixes, w których indeks elementu
odpowiada indeksom w tablicy meshes.

[docelowo do wykonania wielow¹tkowego]*/
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
		//teraz interpolujemy poprzedni¹ orientacjê i orientacjê po sekundzie
		//ze wspóczynnikiem równym czasowi jaki up³yn¹³ faktycznie
		orientation = XMQuaternionSlerp( orientation, rotation_velocity, time_lag );

		/*Du¿o obliczeñ, mo¿e da siê to jakoœ za³atwiæ bez interpolacji...*/
#else


		if ( XMVector3Equal( rotation_velocity, XMVectorZero()) )
			orientation = XMQuaternionIdentity();
		else		
		{
			float rot_angle = XMVectorGetW( rotation_velocity ) * time_lag;					//liczymy k¹t obrotu
			rotation_velocity = XMQuaternionRotationAxis( rotation_velocity, rot_angle );	//przerabiamy na kwaternion
			orientation = XMQuaternionMultiply( orientation, rotation_velocity );			//liczymy nowy kwaternion orientacji
		}		
#endif
		XMMATRIX transformation = XMMatrixRotationQuaternion( orientation );
#ifdef _SCALEABLE_OBJECTS
		//skalowanie ca³ego mesha
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
	light_dir = XMVectorNegate( light_dir );		// Robimy to, ¿eby shader nie musia³ odwracaæ
	XMStoreFloat4( &shader_data_per_frame.light_direction[index], light_dir );
	
	// Przycinamy wektor do przedzia³u [0.0 , 1.0]
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
	directX_device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&projection_matrix);	//macierz projekcji jest zawsze ta sama, chyba ¿e j¹ ktoœ jawnie zmodyfikuje

	directX_device->SetVertexShader( nullptr );
	directX_device->SetFVF(VERTEX_NORMAL_TEXTCORDS1);

	//parametry tekstur
	directX_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	directX_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	directX_device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	directX_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );


	//na razie pêtla bez optymalizacji
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
				//ustawiamy macierz przekszta³cenia dla danego modelu
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
