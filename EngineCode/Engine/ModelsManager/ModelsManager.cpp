#include "..\..\stdafx.h"
#include "ModelsManager.h"
#include "..\Engine.h"
#include "Loaders\loader_interface.h"
#include "Loaders\FBX_files_loader\FBX_loader.h"


#include "..\..\memory_leaks.h"

//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//

ModelsManager::ModelsManager( Engine* engine )
: engine( engine )
{
	Model3DFromFile::models_manager = this;

	//Loader plik�w FBX jest domy�lnym narz�dziem do wczytywania
	FBX_loader* fbx_loader = new FBX_loader( this );
	loader.push_back( fbx_loader );
}



ModelsManager::~ModelsManager( )
{
	// Kasujemy obiekty do wczytywania danych
	for ( unsigned int i = 0; i < loader.size(); ++i )
		delete loader[i];
}



#ifdef __TEST
void ModelsManager::test( )
{
	
	//stworzymy sobie prosty obiekt do wy�wietlania
	VertexNormalTexCord1 g_Vertices[] =
	{
	// front
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 3.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(3.0f, 3.0f) },

	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(3.0f, 0.0f) },
	{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 3.0f, 3.0f ) },

	// right
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },

	// back
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },

	// left/ left
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },

	// top/ top
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },

	// bootom/ bootom
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },
	};

	
	MaterialObject material;

//#define mat (*material)
#define mat material

	// Set the RGBA for diffuse reflection.
	mat.Diffuse.x = 0.5f;
	mat.Diffuse.y = 0.5f;
	mat.Diffuse.z = 0.67f;
	mat.Diffuse.w = 1.0f;

	// Set the RGBA for ambient reflection.
	mat.Ambient.x = 0.3f;
	mat.Ambient.y = 0.5f;
	mat.Ambient.z = 0.3f;

	// Set the color and sharpness of specular highlights.
	mat.Specular.x = 1.0f;
	mat.Specular.y = 1.0f;
	mat.Specular.z = 1.0f;
	mat.Power = 50.0f;

	// Set the RGBA for emissive color.
	mat.Emissive.x = 0.0f;
	mat.Emissive.y = 0.0f;
	mat.Emissive.z = 0.0f;


	Model3DFromFile* new_model = new Model3DFromFile(L"skrzynia");
	new_model->BeginEdit();
	new_model->BeginPart();
	
	new_model->add_vertex_buffer( g_Vertices, 36 );
	new_model->add_material( &mat, L"::skrzynia_material" );
	//L"..\tylko_do_testow\tex.bmp"
	new_model->add_texture( L"..\tylko_do_testow\tex.bmp" );

	new_model->EndPart();
	new_model->EndEdit();
	/*
	new_model->vertex_buffer = BufferObject::create_from_memory( g_Vertices,
																 sizeof(VertexNormalTexCord1),
																 36,
																 D3D11_BIND_VERTEX_BUFFER );
	vertex_buffer.unsafe_add( L"skrzynia", new_model->vertex_buffer );


	ModelPart part;
	part.mesh = new MeshPartObject;
	part.mesh->vertices_count = 36;
	part.material = material;
	part.pixel_shader = pixel_shader.get( L"default_pixel_shader" );
	part.vertex_shader = vertex_shader.get( L"default_vertex_shader" );

	new_model->model_parts.push_back(part);*/



	file_model.unsafe_add( L"skrzynia", new_model );


	load_model_from_file(L"tylko_do_testow/clone_fighter_rel.FBX");
	load_model_from_file(L"tylko_do_testow/moon/moon.FBX");
	load_model_from_file( L"tylko_do_testow/Nebulon/Nebulon.FBX" );
	load_model_from_file( L"tylko_do_testow/VadersTIE.FBX" );
	load_model_from_file( L"tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX" );
}
#endif






/*Dopasowuje najlepszy z domy�lnych shader�w, kt�ry pasuje do podanej tablicy
tekstur. Tablica ma tyle element�w ile zmienna ENGINE_MAX_TEXTURES.

Ka�da pozycja w tablicy ma przypisane domy�lne znaczenie zgodnie z enumeracj� 
TEXTURES_TYPES. Najlepszy shader jest wybierany na podstawie obecno�ci
lub nieobecno�ci tekstury w tablicy.*/
VertexShaderObject* ModelsManager::find_best_vertex_shader( TextureObject** textures )
{
	// Na razie nie mamy innych domy�lnych shader�w
	return vertex_shader.get( L"default_vertex_shader" );
}


/*Dopasowuje najlepszy z domy�lnych shader�w, kt�ry pasuje do podanej tablicy
tekstur. Tablica ma tyle element�w ile zmienna ENGINE_MAX_TEXTURES.

Ka�da pozycja w tablicy ma przypisane domy�lne znaczenie zgodnie z enumeracj�
TEXTURES_TYPES. Najlepszy shader jest wybierany na podstawie obecno�ci
lub nieobecno�ci tekstury w tablicy.*/
PixelShaderObject* ModelsManager::find_best_pixel_shader( TextureObject** textures )
{
	// Na razie nie mamy innych domy�lnych shader�w
	return pixel_shader.get( L"default_pixel_shader" );
}


/* #private
Znajduje Loader pasuj�cy do pliku podanego w parametrze. */
Loader* ModelsManager::find_loader( const std::wstring& path )
{
	for ( unsigned int i = 0; i < loader.size( ); ++i )
	if ( loader[i]->can_load( path ) )
		return loader[i];
	return nullptr;
}


/* #private
Tworzy i wstawia domy�lne warto�ci asset�w do swoich tablic.
Najwa�niejsze s� vertex shadery i pixel shadery, bo bez nich si� nie obejdzie,
ale w tej funkcji mo�na te� stworzy� inne obiekty.*/
void ModelsManager::set_default_assets( ID3D11VertexShader* vert_shader, ID3D11PixelShader* pix_shader )
{
	// Podane w parametrach shadery nie maj� jeszcze swojego obiektu-opakowania, wi�c trzeba go stworzy�
	VertexShaderObject* new_vertex_shader = new VertexShaderObject( vert_shader );
	PixelShaderObject* new_pixel_shader = new PixelShaderObject( pix_shader );

	// Dodajemy shadery. Takich nazwa na pewno nie ma w tablicach i nie b�dzie
	vertex_shader.unsafe_add( DEFAULT_VERTEX_SHADER_STRING, new_vertex_shader );
	pixel_shader.unsafe_add( DEFAULT_PIXEL_SHADER_STRING, new_pixel_shader );

	// Tworzymy defaultowy materai�
	MaterialObject* new_material = new MaterialObject();
	new_material->set_null_material();
	material.unsafe_add( DEFAULT_MATERIAL_STRING, new_material );
}

/*Wczytuje model z podanego pliku.*/
MODELS_MANAGER_RESULT ModelsManager::load_model_from_file( const std::wstring& file )
{
	// Sprawdzamy czy plik nie zosta� ju� wczytany
	Model3DFromFile* new_model = file_model.get( file );
	if ( new_model != nullptr )
		return MODELS_MANAGER_RESULT::MODELS_MANAGER_OK;	// Udajemy, �e wszystko posz�o dobrze

	// Sprawdzamy, kt�ry loader potrafi otworzy� plik
	Loader* loader = find_loader( file );
	if ( loader == nullptr )
		return MODELS_MANAGER_RESULT::MODELS_MANAGER_LOADER_NOT_FOUND;		// �aden nie potrafi

	// Tworzymy obiekt Model3DFromFile, do kt�rego loader wpisze zawarto�� pliku
	new_model = new Model3DFromFile( file );

	// Wszystkie operacje wpisywania musz� by� zamkni�te tymi wywo�aniami
	new_model->BeginEdit();
	LOADER_RESULT result = loader->load_mesh( new_model, file );
	new_model->EndEdit();

	if ( result != LOADER_RESULT::MESH_LOADING_OK )
	{
		// load_mesh powinno zwr�ci� 0
		delete new_model;
		return MODELS_MANAGER_RESULT::MODELS_MANAGER_CANNOT_LOAD;
	}

	// Dodajemy model do tablic
	file_model.unsafe_add( file, new_model );

	return MODELS_MANAGER_RESULT::MODELS_MANAGER_OK;
}



#ifndef __UNUSED
//-------------------------------------------------------------------------------//
//							wersja DirectX9
//-------------------------------------------------------------------------------//



ModelsManager::ModelsManager(Engine* engine)
	: engine(engine)
{
	Model3DFromFile::models_manager = this;

	materials_count = 1;
	textures_count = 1;
	meshes_count = 1;
	models_count = 1;

	//Loader plik�w FBX jest domy�lnym narz�dziem do wczytywania
	FBX_loader* fbx_loader = new FBX_loader(this);
	loaders.push_back(fbx_loader);
}

#ifdef __TEST
void ModelsManager::test()
{
	/*
		//stworzymy sobie prosty obiekt do wy�wietlania
		Vertex_Normal_TexCords1 g_Vertices[] =
		{
		// front
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 3.0f), },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(3.0f, 3.0f), },

		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(3.0f, 0.0f), },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 3.0f, 3.0f ), },

		// right
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), },

		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ), },

		// back
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), },

		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ), },

		// left/ left
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), },

		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), },
		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ), },

		// top/ top
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), },

		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ), },

		// bootom/ bootom
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ), },
		};

		D3DMATERIAL9 mat;

		// Set the RGBA for diffuse reflection.
		mat.Diffuse.r = 0.5f;
		mat.Diffuse.g = 0.5f;
		mat.Diffuse.b = 0.67f;
		mat.Diffuse.a = 1.0f;

		// Set the RGBA for ambient reflection.
		mat.Ambient.r = 0.3f;
		mat.Ambient.g = 0.5f;
		mat.Ambient.b = 0.3f;
		mat.Ambient.a = 1.0f;

		// Set the color and sharpness of specular highlights.
		mat.Specular.r = 1.0f;
		mat.Specular.g = 1.0f;
		mat.Specular.b = 1.0f;
		mat.Specular.a = 1.0f;
		mat.Power = 50.0f;

		// Set the RGBA for emissive color.
		mat.Emissive.r = 0.0f;
		mat.Emissive.g = 0.0f;
		mat.Emissive.b = 0.0f;
		mat.Emissive.a = 0.0f;*/

	Model3DFromFile* new_model = new Model3DFromFile( models_count );
	++models_count;
	/*
	XMMATRIX matrix1 = XMMatrixIdentity();
	DirectX::XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, matrix1);
	new_model->add_mesh(g_Vertices, 36, matrix);
	//"..\tylko_do_testow\tex.bmp"
	if ( new_model->add_texture( L"tylko_do_testow/tex.bmp" ) == WRONG_TEXTURE_ID )
	new_model->add_null_texture();
	new_model->add_material(mat);
	*/
	new_model->file_path = "tylko_do_testow/clone_fighter_rel.FBX";
	int result = loaders[0]->load_mesh( new_model, L"tylko_do_testow/clone_fighter_rel.FBX" );
	models.push_back( new_model );

	new_model = new Model3DFromFile( models_count );
	++models_count;

	new_model->file_path = "tylko_do_testow/moon/moon.FBX";
	result = loaders[0]->load_mesh( new_model, L"tylko_do_testow/moon/moon.FBX" );
	models.push_back( new_model );

	new_model = new Model3DFromFile( models_count );
	++models_count;

	new_model->file_path = "tylko_do_testow/Nebulon/Nebulon.FBX";
	result = loaders[0]->load_mesh( new_model, L"tylko_do_testow/Nebulon/Nebulon.FBX" );
	models.push_back( new_model );


	new_model = new Model3DFromFile( models_count );
	++models_count;

	new_model->file_path = "tylko_do_testow/VadersTIE.FBX";
	result = loaders[0]->load_mesh( new_model, L"tylko_do_testow/VadersTIE.FBX" );
	models.push_back( new_model );

	new_model = new Model3DFromFile( models_count );
	++models_count;

	new_model->file_path = "tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX";
	result = loaders[0]->load_mesh( new_model, L"tylko_do_testow/TIE_Fighter/TIE_Fighter.FBX" );
	models.push_back( new_model );

}
#endif


ModelsManager::~ModelsManager()
{
	for (unsigned int i = 0; i < meshes.size(); ++i)
		delete meshes[i], meshes[i] = nullptr;
	for (unsigned int i = 0; i < textures.size(); ++i)
		delete textures[i], textures[i] = nullptr;
	for (unsigned int i = 0; i < materials.size(); ++i)
		delete materials[i], materials[i] = nullptr;

	meshes.clear();
	textures.clear();
	materials.clear();
}


/*Funkcje add_(...) sprawdzaj� czy obiekt o takich samych danych istnieje. Je�eli nie istnieje,
 *jest tworzony. Funkcje zwracaj� wska�nik na stworzony obiekt lub wczesniej istnij�cy, je�eli
 *nie by�o potrzeby tworzenia nowego. W zmiennej id znajduje si� unikalny idetyfikator obiektu,
 *za pomoc� kt�rego mo�na si� do niego odwo�ywa� (co jest wskazan� metod� w trakcie dzia�ania silnika
 *poniewa� dost�p do danych jest szybszy). Pobranie wska�nieka za pomoca funkcji get_(...).
 *
 *Wszystkie potencjalne b�edne sytuacje sygnalizowane s� nullptr.*/

MaterialObject*	ModelsManager::add_material(const D3DMATERIAL9 &material, unsigned int& id)
{
	for (unsigned int i = 0; i < materials.size(); ++i)
		if (*(materials[i]) == material)
		{
			id = materials[i]->unique_id;
			return materials[i];
		}

	//nie znale�lismy takiego samego materia�u, wiec go tworzymy
	MaterialObject* new_material = new MaterialObject(materials_count, material);
	materials.push_back(new_material);

	//inkrementujemy liczniki
	id = materials_count;
	++materials_count;

	return new_material;
}


MaterialObject*	ModelsManager::get_material(unsigned int id)
{
	unsigned int left = 0;
	unsigned int right = materials.size() - 1;
	unsigned int pivot;

	while (left <= right)
	{
		pivot = ((right + left) >> 1);
		if (materials[pivot]->unique_id < id)
			left = pivot + 1;
		else if (materials[pivot]->unique_id > id)
			right = pivot - 1;
		else
			return materials[pivot];
	}

	//nie znale�li�my materia�u
	return nullptr;
}

/*Funkcja dodaje nowy mesh do kolekcji, je�eli �aden poprzedni nie by� taki sam.
Wierzcho�ki spod wska�nika vertices s� kopiowane do bufora docelowego DirectXa. Za zwolnienie pamieci po buforze
odpowiada jego tw�rca. Bufor nie jest potrzebny po zako�czeniu dzia�ania tej funkcji.*/
MeshObject*	ModelsManager::add_mesh(Vertex_Normal_TexCords1* vertices, unsigned int vert_num, unsigned int& id)
{
	/*Dodaj�c mesha sprawdzamy czy nie ma ju� takiego. Wymaga to por�wnania wszystkich wierzcho�k�w.
	Wygl�da to na kosztown� operacj�, ale w zasadzie g��wny koszt idzie na por�wnanie meshy, kt�re s� takie same.
	Pozosta�e por�wnania powinny si� zako�czy� po pierwszych kilku wierzcho�kach.*/
	for (unsigned int i = 0; i < meshes.size(); ++i)
		if (is_equal(meshes[i]->vertex_buffer, vertices, vert_num))
		{
			id = meshes[i]->unique_id;
			return meshes[i];
		}

	//nie znale�li�my takiego samego mesha, wi�c tworzymy nowego
	MeshObject* new_mesh = new MeshObject(meshes_count);
	new_mesh->primitives_count = vert_num / 3;
	id = meshes_count;
	++meshes_count;

	HRESULT result = engine->directX_device->CreateVertexBuffer(vert_num*sizeof(Vertex_Normal_TexCords1),
							0, VERTEX_NORMAL_TEXTCORDS1, D3DPOOL_DEFAULT, &(new_mesh->vertex_buffer), nullptr);
	if (result != D3D_OK)
	{//mog�o np zabrakn�� pamieci, albo inne r�ne rzeczy
		delete new_mesh;
		id = WRONG_MESH_ID;
		--meshes_count;
		return nullptr;
	}

	Vertex_Normal_TexCords1* pVertices;
	result = new_mesh->vertex_buffer->Lock(0, 0, (void**)&pVertices, 0);
	
	if (result != D3D_OK)
	{//a tutaj to nie wiem co mog�o si� sta�, ale funkcja Lock mo�e zwroci� b��d
		new_mesh->vertex_buffer->Unlock();
		delete new_mesh;
		id = WRONG_MESH_ID;
		--meshes_count;
		return nullptr;
	}

	//kopiujemy zawarto�� tego co dostali�my do docelowego bufora
	memcpy(pVertices, vertices, vert_num*sizeof(Vertex_Normal_TexCords1));
	new_mesh->vertex_buffer->Unlock();

	//uda�o si� wszystko, dodajemy do wektora
	meshes.push_back(new_mesh);

	return new_mesh;
}

MeshObject*	ModelsManager::get_mesh(unsigned int id)
{
	unsigned int left = 0;
	unsigned int right = meshes.size() - 1;
	unsigned int pivot;

	while (left <= right)
	{
		pivot = ((right + left) >> 1);
		if (meshes[pivot]->unique_id < id)
			left = pivot + 1;
		else if (meshes[pivot]->unique_id > id)
			right = pivot - 1;
		else
			return meshes[pivot];
	}

	//nie znale�li�my materia�u
	return nullptr;
}

TextureObject* ModelsManager::add_texture(const std::wstring& path, unsigned int& id)
{
	for (unsigned int i = 0; i < textures.size(); ++i)
		if (*(textures[i]) == path)
		{
			id = textures[i]->unique_id;
			return textures[i];
		}

	//nie znale�lismy takiej samej tekstury, wiec j� tworzymy
	LPDIRECT3DTEXTURE9 direct_tex;
	HRESULT result = D3DXCreateTextureFromFile( engine->directX_device, path.c_str(), &direct_tex );
	if ( result != D3D_OK )
	{
		id = WRONG_TEXTURE_ID;
		return nullptr;
	}
	/*if ( result == D3DERR_NOTAVAILABLE )
		return nullptr;
	else if ( result == D3DERR_OUTOFVIDEOMEMORY )
		return nullptr;
	else if ( result == D3DERR_INVALIDCALL )
		return nullptr;
	else if ( result == D3DXERR_INVALIDDATA )
		return nullptr;
	else if ( result == E_OUTOFMEMORY )
		return nullptr;*/
	TextureObject* new_texture = new TextureObject(textures_count, path, direct_tex);
	textures.push_back(new_texture);

	//inkrementujemy liczniki
	id = textures_count;
	++textures_count;

	return new_texture;
}

TextureObject* ModelsManager::get_texture(unsigned int id)
{
	unsigned int left = 0;
	unsigned int right = textures.size() - 1;
	unsigned int pivot;

	while (left <= right)
	{
		pivot = ((right + left) >> 1);
		if (textures[pivot]->unique_id < id)
			left = pivot + 1;
		else if (textures[pivot]->unique_id > id)
			right = pivot - 1;
		else
			return textures[pivot];
	}

	//nie znale�li�my materia�u
	return nullptr;
}


Model3DFromFile* ModelsManager::add_model(const std::wstring& file_name, unsigned int& id)
{
	Loader* loader = find_loader(file_name);
	if (loader == nullptr)
	{
		id = WRONG_MODEL_FILE_ID;
		return nullptr;
	}

	Model3DFromFile* new_model = new Model3DFromFile(models_count);
	int result = loader->load_mesh(new_model, file_name);
	if (result == MESH_LOADING_OK)
	{
		id = models_count;
		++models_count;
		models.push_back(new_model);
		//trzeba zoptymalizowa� ustawienie obiekt�w pod k�tem wy�wietlania
		//new_model->optimize();
		return new_model;
	}

	//je�eli tu jeste�my to �adowanie modelu si� nie powiod�o
	id = WRONG_MODEL_FILE_ID;
	delete new_model;		//tutaj s� kasowane tak�e referencje do obiekt�w, wi�c nie musimy si� o nic martwi�
	return nullptr;
}

Model3DFromFile* ModelsManager::get_model(unsigned int id)
{
	unsigned int left = 0;
	unsigned int right = models.size() - 1;
	unsigned int pivot;

	while (left <= right)
	{
		pivot = ((right + left) >> 1);
		if (models[pivot]->unique_id < id)
			left = pivot + 1;
		else if (models[pivot]->unique_id > id)
			right = pivot - 1;
		else
			return models[pivot];
	}

	//nie znale�li�my materia�u
	return nullptr;
}


/*Funkcja pomocnicza do por�wnywania meshy. Korzysta z zaimplementownych operator�w != dla danego typu wierzcho�ka.
Por�wnywane s� wszystkie dane, tj. pozycja, normalna i wsp�rz�dne tekstury.*/
bool ModelsManager::is_equal(LPDIRECT3DVERTEXBUFFER9 vertex_buffer, Vertex_Normal_TexCords1* vertices, unsigned int vert_num)
{
	//por�wnujemy rozmiary bufor�w
	D3DVERTEXBUFFER_DESC description;
	vertex_buffer->GetDesc(&description);
	if (description.Size / sizeof(Vertex_Normal_TexCords1) != vert_num)
		return FALSE;

	Vertex_Normal_TexCords1* buffer_ptr;
	if (FAILED(vertex_buffer->Lock(0, 0, (void**)&buffer_ptr, D3DLOCK_READONLY)))
		return FALSE;	//je�eli by� jaki� b��d to mamy problem, bo nie ma go jak zakomunikowa�
	//w takiej sytuacji mog� si� dublowa� meshe

	//wykonujemy por�wnania
	unsigned int i = 0;
	for (; i < vert_num; ++i)
		if (buffer_ptr[i] != vertices[i])
			break;

	vertex_buffer->Unlock();

	if (i < vert_num)
		return FALSE;
	//doszli�my do ko�ca i wszystkie wierzcho�ki by�y r�wne
	return TRUE;
}

/*Znajduje Loader pasuj�cy do pliku podanego w parametrze. */
Loader* ModelsManager::find_loader(const std::wstring& path)
{
	for (unsigned int i = 0; i < loaders.size(); ++i)
		if (loaders[i]->can_load(path))
			return loaders[i];
	return nullptr;
}


#endif