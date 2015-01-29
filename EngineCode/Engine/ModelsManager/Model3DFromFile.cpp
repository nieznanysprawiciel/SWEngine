#include "..\..\stdafx.h"

#include "meshes_textures_materials.h"
#include "ModelsManager.h"

ModelsManager* Model3DFromFile::models_manager = nullptr;




//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//




//----------------------------------------------------------------------------------------------//
//									referenced_object											//
//----------------------------------------------------------------------------------------------//

//==============================================================================================//


referenced_object::referenced_object(int id)
{
	file_references = 0;
	object_references = 0;
	unique_id = id;
}


referenced_object::~referenced_object()
{}

bool referenced_object::can_delete(unsigned int& file_ref, unsigned int& other_ref)
{
	file_ref = file_references;
	other_ref = object_references;

	if (file_references == 0 && object_references == 0)
		return TRUE;
	return FALSE;
}

//----------------------------------------------------------------------------------------------//
//									Model3DFromFile												//
//----------------------------------------------------------------------------------------------//

//==============================================================================================//


//----------------------------------------------------------------------------------------------//
//								contructor, destructor											//
//----------------------------------------------------------------------------------------------//

Model3DFromFile::Model3DFromFile( int id )
: referenced_object( id )
{
	vertex_buffer = nullptr;
	tmp_data = nullptr;
}


Model3DFromFile::~Model3DFromFile( )
{
	if ( tmp_data )
	{
		delete[] tmp_data->table;
		delete tmp_data;
		tmp_data = nullptr;
	}


	//najpierw kasujemy odwo�ania do obiekt�w
	for ( unsigned int k = 0; k < model_parts.size( ); ++k )
	{//ka�dy element mo�e by� nullptrem
		register ModelPart* part = &model_parts[k];

		//ka�dy element mo�e by� nullptrem
		if ( part->material )
			part->material->delete_file_reference( );
		if ( part->index_buffer )
			part->index_buffer->delete_file_reference( );
		if ( part->mesh )
		{
			//kasujemy obiekty MeshParts (to s� jedyne, kt�rymi zarz�dza ta klasa)
			part->mesh->delete_file_reference();
			delete part->mesh;
		}
		if ( part->pixel_shader )
			part->pixel_shader->delete_file_reference( );
		for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
			if ( part->texture )
				part->texture[i]->delete_file_reference( );
		if ( part->vertex_shader )
			part->vertex_shader->delete_file_reference( );
	}


	model_parts.clear( );
}




//----------------------------------------------------------------------------------------------//
//									Funkcje do tworzenia obiektu								//
//----------------------------------------------------------------------------------------------//


/*Funkcje BeginEdit i EndEdit maj� by� wywo�ane przez klas� ModelsManager przed podaniem do loadera
i po zako�czeniu wpisywania danych.

BeginEdit przygotowuje struktur� pod wpisywanie danych.*/
void Model3DFromFile::BeginEdit()
{
	tmp_data = new EditTMP;
	//Inicjujemy tablic� o pocz�tkowej d�ugo�ci domy��nej
	tmp_data->table = new TMP_data*[tmp_data->table_length];
}


/*Funkcje BeginEdit i EndEdit maj� by� wywo�ane przez klas� ModelsManager przed podaniem do loadera
i po zako�czeniu wpisywania danych.

EndEdit zatwierdza wpisane dane, tworzy w�a�ciwa struktur� i wpisuje nie istniej�ce jeszcze obiekty
do ModelsManagera.*/
void Model3DFromFile::EndEdit()
{
	//r�ne rzeczy

	//zwalniamy pami��
	delete[] tmp_data->table;
	delete tmp_data;
	tmp_data = nullptr;
}


/*Funkcje BeginPart i EndPart s� wywo�ywane przez loader i maj� otacza� wszystkie instrukcje dodaj�ce
jedn� cz�� mesha do struktury.*/
void Model3DFromFile::BeginPart()
{
	//sprawdzamy czy tablica nam si� nie sko�czy�a
	if ( tmp_data->current_pointer <= tmp_data->table_length )
	{//alokujemy troszke wi�cej pami�ci i przepisujemy
		unsigned int new_length = tmp_data->table_length << 1;

		TMP_data** new_table = new TMP_data*[new_length];
		for ( int i = 0; i < tmp_data->table_length; ++i )
			new_table[i] = tmp_data->table[i];

		tmp_data->table_length = new_length;
		delete[] tmp_data->table;
		tmp_data->table = new_table;
	}

	//dodajemy do tabeli nowy element
	tmp_data->table[tmp_data->current_pointer] = new TMP_data;
	tmp_data->table[tmp_data->current_pointer]->new_part.mesh = new MeshPartObject;

	//nie jest to konieczne, ale dla konwencji dodajemy
	tmp_data->table[tmp_data->current_pointer]->new_part.mesh->add_file_reference();
}

/*Funkcje BeginPart i EndPart s� wywo�ywane przez loader i maj� otacza� wszystkie instrukcje dodaj�ce
jedn� cz�� mesha do struktury.*/
void Model3DFromFile::EndPart()
{
	//inkrementujemy aktualny index wewn�trz tablicy
	tmp_data->current_pointer++;
}



/*Dodaje tekstur� na aktualnej pozycji w tablicy.
Tekstura jest umieszczana zgodnie z indeksem reprezentowanym przez type.*/
unsigned int Model3DFromFile::add_texture( const std::wstring& file_name, TEXTURES_TYPES type )
{
	// Kto� m�g� poda� indeks w tablicy zamiast jednego z enum�w, wi�c musimy pilnowa� czy nie przekroczy� zakresu
	if ( type > ENGINE_MAX_TEXTURES )
		return WRONG_ID;

	TextureObject* texture = models_manager->texture.get( file_name );
	if ( !texture )
	{
		// Nie by�o tekstury, trzeba j� stworzy� i doda�
		texture = TextureObject::create_from_file( file_name );
		if ( !texture )		// Tekstura mog�a mie� z�y format, a nie chcemy dodawa� nullptra do ModelsManagera
			return WRONG_ID;

		models_manager->texture.unsafe_add( file_name, texture );	// Dodali�my tekstur�
	}

	// Teraz musimy doda� tekstur� na odpowiednie miejsce w tablicy
	tmp_data->table[tmp_data->current_pointer]->new_part.texture[type] = texture;

	// Zwracamy id tekstury, ale w zasadzie tylko po to, �eby kto� m�g� sprawdzi� czy wszystko posz�o dobrze
	return texture->get_id();
}

unsigned int Model3DFromFile::add_material( const MaterialObject* material, const std::wstring& material_name )
{

	return WRONG_ID;
}

unsigned int Model3DFromFile::add_vertex_shader( const std::wstring& file_name )
{

	return WRONG_ID;
}

unsigned int Model3DFromFile::add_pixel_shader( const std::wstring& file_name )
{

	return WRONG_ID;
}

unsigned int Model3DFromFile::add_vertex_buffer( const VertexNormalTexCord1* buffer, unsigned int vert_count )
{

	return WRONG_ID;
}

void Model3DFromFile::add_transformation( const DirectX::XMFLOAT4X4& transform )
{
	tmp_data->table[tmp_data->current_pointer]->new_part.mesh->transform_matrix = transform;
}

unsigned int Model3DFromFile::add_index_buffer( unsigned int* buffer, unsigned int ind_count, int vertex_buffer_offset )
{

	return WRONG_ID;
}


//----------------------------------------------------------------------------------------------//
//									Funkcje do korzystania z obiekt�w							//
//----------------------------------------------------------------------------------------------//


const ModelPart* Model3DFromFile::get_part( unsigned int index )
{
	if ( index < model_parts.size( ) )
		return &model_parts[index];
	return nullptr;
}


unsigned int Model3DFromFile::get_parts_count()
{
	return model_parts.size();
}



#ifndef __UNUSED

//-------------------------------------------------------------------------------//
//							wersja DirectX 9
//-------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------------------//
//								contructor, destructor											//
//----------------------------------------------------------------------------------------------//

Model3DFromFile::Model3DFromFile( int id )
: referenced_object( id )
{
}


Model3DFromFile::~Model3DFromFile( )
{
	//najpierw kasujemy odwo�ania do obiekt�w
	for ( unsigned int i = 0; i < mesh_parts.size( ); ++i )
		mesh_parts[i]->mesh_object->delete_file_reference( );
	for ( unsigned int i = 0; i < textures.size( ); ++i )
		textures[i]->delete_file_reference( );;
	for ( unsigned int i = 0; i < materials.size( ); ++i )
		materials[i]->delete_file_reference( );

	//kasujemy obiekty MeshParts (to s� jedyne, kt�rymi zarz�dza ta klasa)
	for ( unsigned int i = 0; i < mesh_parts.size( ); ++i )
		delete mesh_parts[i];
}



//----------------------------------------------------------------------------------------------//
//									building models												//
//----------------------------------------------------------------------------------------------//


unsigned int Model3DFromFile::add_material( const D3DMATERIAL9 &material )
{
	MaterialObject* result;
	unsigned int id;
	result = models_manager->add_material( material, id );

	if ( result == nullptr )
		return WRONG_MATERIAL_ID;

	materials.push_back( result );
	result->add_file_reference( );		//pamietajmy o dodawaniu odwo�a�

	return id;
}

/*Je�eli jakis materia� zosta� dodany bezpo�rednio do ModelsManagera, to mo�na si� do niego odwo�ywa� po id.
Zwraca nullptr je�eli nie istnieje taki materia� lub wska�nik na ten materia�.

Materia� nie zostaje dodany do listy, je�eli nie istnia�!!!!!!!!!!*/
MaterialObject* Model3DFromFile::add_material( unsigned int id )
{
	MaterialObject* result = models_manager->get_material( id );
	if ( result != nullptr )
	{
		materials.push_back( result );
		result->add_file_reference( );		//pamietajmy o dodawaniu odwo�a�
	}

	return result;
}

/*Niekt�re kawa�ki mesha mog� nie mie� materia�u.*/
void Model3DFromFile::add_null_material( )
{
	materials.push_back( nullptr );
}


unsigned int Model3DFromFile::add_texture( const std::wstring& path )
{
	TextureObject* result;
	unsigned int id;
	result = models_manager->add_texture( path, id );

	if ( result == nullptr )
		return WRONG_TEXTURE_ID;

	textures.push_back( result );
	result->add_file_reference( );		//pamietajmy o dodawaniu odwo�a�

	return id;
}


/*Je�eli jaka� tekstura zosta�a dodana bezpo�rednio do ModelsManagera, to mo�na si� do niej odwo�ywa� po id.
Zwraca nullptr je�eli nie istnieje taka tekstura lub wska�nik na teksture, je�eli istnieje.

Tekstura nie zostaje dodana do listy je�eli nie istnia�a!!!!!!*/
TextureObject* Model3DFromFile::add_texture( unsigned int id )
{
	TextureObject* result = models_manager->get_texture( id );
	if ( result != nullptr )
	{
		textures.push_back( result );
		result->add_file_reference( );		//pamietajmy o dodawaniu odwo�a�
	}

	return result;
}

/*Niekt�re kawa�ki mesha mog� nie mie� tekstury.*/
void Model3DFromFile::add_null_texture( )
{
	textures.push_back( nullptr );
}


MeshPart* Model3DFromFile::add_mesh( unsigned int id, DirectX::XMFLOAT4X4& matrix )
{
	MeshObject* result = models_manager->get_mesh( id );
	if ( result == nullptr )
		return nullptr;

	MeshPart* new_mesh_part = new MeshPart;
	new_mesh_part->transform_matrix = matrix;
	new_mesh_part->mesh_object = result;

	mesh_parts.push_back( new_mesh_part );
	result->add_file_reference( );		//pamietajmy o dodawaniu odwo�a�

	return new_mesh_part;
}


/*Dodajemy wierzcho�ki do listy modelu z danego pliku, przypisuj�c mu podan� w ostatnim parametrze macierz przekszta�cenia.
Mesh jest na pocz�tku dodawany do ModelsManagera, je�eli ju� istnia�, nie zostaje zduplikowany.*/
unsigned int Model3DFromFile::add_mesh( Vertex_Normal_TexCords1* vertices, unsigned int vert_num, const DirectX::XMFLOAT4X4& matrix )
{
	MeshObject* result;
	unsigned int id;
	result = models_manager->add_mesh( vertices, vert_num, id );
	if ( result == nullptr )	return WRONG_MESH_ID;		//co� posz�o nie tak

	MeshPart* mesh_part = new MeshPart( );
	mesh_part->mesh_object = result;
	mesh_part->transform_matrix = matrix;

	mesh_parts.push_back( mesh_part );

	result->add_file_reference( );		//pami�tajmy o dodawaniu odwo�a�

	return id;
}

void Model3DFromFile::add_reference( )
{
	++references;
}

void Model3DFromFile::delete_reference( )
{
	--references;
}

/*Sprawdza czy s� jakie� odwo�ania do podanego pliku. Je�eli nie ma to znaczy, �e obiekt mo�e
zostac bezpiecznie usuni�ty.*/
bool Model3DFromFile::can_delete( unsigned int& ref )
{
	if ( references > 0 )
		return FALSE;
	return TRUE;
}

unsigned int Model3DFromFile::get_parts_count( )
{
	unsigned int size1 = materials.size( );
	unsigned int size2 = textures.size( );
	unsigned int size3 = mesh_parts.size( );

	unsigned int min = size1 < size2 ? size1 : size2;
	return min < size3 ? min : size3;
}

MaterialObject* Model3DFromFile::get_material( unsigned int index )
{
	if ( index < materials.size( ) )
		return materials[index];
	return nullptr;
}

TextureObject* Model3DFromFile::get_texture( unsigned int index )
{
	if ( index < textures.size( ) )
		return textures[index];
	return nullptr;
}

MeshPart* Model3DFromFile::get_mesh_part( unsigned int index )
{
	if ( index < mesh_parts.size( ) )
		return mesh_parts[index];
	return nullptr;
}

#endif