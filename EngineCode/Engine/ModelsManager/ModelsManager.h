#pragma once

#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"
#include "ResourceContainer.h"

class Engine;
class Loader;



//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//


typedef enum MODELS_MANAGER_RESULT
{
	MODELS_MANAGER_OK,
	MODELS_MANAGER_LOADER_NOT_FOUND,
	MODELS_MANAGER_CANNOT_LOAD
};


/*Klasa przechowuje wszystkie obiekty zasob�w w silniku (opr�cz d�wi�k�w, kt�re raczej
zostan� oddelegowane w inne miejsce).

Zasoby s� identyfikowane jednoznacznym identyfikatorem, kt�ry jest unikalny
jedynie w danej grupie zasob�w, lub te� nazw� pliku na podstawie kt�rego dany zas�b powsta�.

Materia�y s� specyficzne, poniewa� jeden plik z programu do modelowania raczej zawiera
wiele r�nych materia��w. �eby je mo�na by�o w ten spos�b jednoznacznie identyfikowa�,
do nazwy pliku doklejane s� dwa dwukropki :: i nazwa materia�u, jaka zosta�a mu nadana
w pliku, z kt�rego pochodzi.
Oczywi�cie, jezeli materia� zosta� stworzony r�cznie w silniku, to nie ma potrzeby
nadawania mu takiej nazwy, wystarczy, �eby by�a ona unikatowa.*/
class ModelsManager
{
	friend Model3DFromFile;
private:
	Engine* engine;

	ResourceContainer<VertexShaderObject*>		vertex_shader;
	ResourceContainer<PixelShaderObject*>		pixel_shader;
	ResourceContainer<TextureObject*>			texture;
	ResourceContainer<BufferObject*>			vertex_buffer;
	ResourceContainer<BufferObject*>			index_buffer;
	ResourceContainer<MaterialObject*>			material;
	// UWAGA! file_model musi by� na ko�cu. Jego destruktor kasuje odwo�ania do obiekt�w powy�ej.
	// Musz� one w tym czasie istnie�, a destruktory s� wywo�ywane w kolejno�ci odwrotnej do zadeklarowanej w klasie.
	ResourceContainer<Model3DFromFile*>			file_model;

	/*loadery dla r�nych format�w plik�w z modelami*/
	std::vector<Loader*>			loader;

public:
	ModelsManager( Engine* engine );
	~ModelsManager( );

	// Funkcje pomocnicze
	VertexShaderObject* find_best_vertex_shader( TextureObject** textures );
	PixelShaderObject* find_best_pixel_shader( TextureObject** textures );
	void set_default_assets( ID3D11VertexShader* vert_shader, ID3D11PixelShader* pix_shader );


	// Funkcje do zarz�dzania assetami
	MODELS_MANAGER_RESULT load_model_from_file( const std::wstring& file );

	inline Model3DFromFile* get_model( const std::wstring& name ) { return file_model.get( name ); }

private:
	Loader* find_loader( const std::wstring& path );
	

#ifdef __TEST
public:
	void test( );
#endif
};

//-------------------------------------------------------------------------------//
//							wersja DirectX9
//-------------------------------------------------------------------------------//

#ifndef __UNUSED


class ModelsManager
{
private:
	Engine* engine;

	unsigned int			materials_count;		//nowy materia� dostanie ten identyfikator
	unsigned int			textures_count;			//nowa tekstura dostanie ten identyfikator
	unsigned int			meshes_count;			//nowy mesh dostanie ten identyfikator
	unsigned int			models_count;			//nowy model dostanie ten identyfikator

	/*tutaj s� przechowywane obiekty zawieraj�ce meshe, tekstury i materia�y
	 *ka�dy zawiera tylko jeden ezgemplarz danego mesha (inaczej ni� w obiektach Model3DFromFile)*/
	std::vector<MeshObject*>		meshes;
	std::vector<TextureObject*>		textures;
	std::vector<MaterialObject*>	materials;

	/*wczytane pliki*/
	std::vector<Model3DFromFile*>	models;

	/*loadery dla r�nych format�w plik�w z modelami*/
	std::vector<Loader*>			loaders;
public:
	ModelsManager(Engine* engine);
	~ModelsManager();

	MaterialObject*		add_material(const D3DMATERIAL9 &material, unsigned int& id);
	MaterialObject*		get_material(unsigned int id);
	MeshObject*			add_mesh(Vertex_Normal_TexCords1* vertices, unsigned int vert_num, unsigned int& id);
	MeshObject*			get_mesh(unsigned int id);
	TextureObject*		add_texture(const std::wstring& path, unsigned int& id);
	TextureObject*		get_texture(unsigned int id);

	Model3DFromFile*	add_model(const std::wstring& file_name, unsigned int& id);
	Model3DFromFile*	get_model(unsigned int id);
private:
	bool is_equal(LPDIRECT3DVERTEXBUFFER9 vertex_buffer, Vertex_Normal_TexCords1* vertices, unsigned int vert_num);
	Loader* find_loader(const std::wstring& path);

#ifdef __TEST
public:
	void test();
#endif
};

#endif