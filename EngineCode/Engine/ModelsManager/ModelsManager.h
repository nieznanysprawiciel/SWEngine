#pragma once

#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"
#include "ResourceContainer.h"

class Engine;
class Loader;

//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//

/*Klasa przechowuje wszystkie obiekty zasobów w silniku (oprócz dŸwiêków, które raczej
zostan¹ oddelegowane w inne miejsce).

Zasoby s¹ identyfikowane jednoznacznym identyfikatorem, który jest unikalny
jedynie w danej grupie zasobów, lub te¿ nazw¹ pliku na podstawie którego dany zasób powsta³.

Materia³y s¹ specyficzne, poniewa¿ jeden plik z programu do modelowania raczej zawiera
wiele ró¿nych materia³ów. ¯eby je mo¿na by³o w ten sposób jednoznacznie identyfikowaæ,
do nazwy pliku doklejane s¹ dwa dwukropki :: i nazwa materia³u, jaka zosta³a mu nadana
w pliku, z którego pochodzi.
Oczywiœcie, jezeli materia³ zosta³ stworzony rêcznie w silniku, to nie ma potrzeby
nadawania mu takiej nazwy, wystarczy, ¿eby by³a ona unikatowa.*/
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
	ResourceContainer<Model3DFromFile*>			file_model;
	ResourceContainer<MaterialObject*>			material;

	/*loadery dla ró¿nych formatów plików z modelami*/
	std::vector<Loader*>			loader;

public:
	ModelsManager( Engine* engine );
	~ModelsManager( );



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

	unsigned int			materials_count;		//nowy materia³ dostanie ten identyfikator
	unsigned int			textures_count;			//nowa tekstura dostanie ten identyfikator
	unsigned int			meshes_count;			//nowy mesh dostanie ten identyfikator
	unsigned int			models_count;			//nowy model dostanie ten identyfikator

	/*tutaj s¹ przechowywane obiekty zawieraj¹ce meshe, tekstury i materia³y
	 *ka¿dy zawiera tylko jeden ezgemplarz danego mesha (inaczej ni¿ w obiektach Model3DFromFile)*/
	std::vector<MeshObject*>		meshes;
	std::vector<TextureObject*>		textures;
	std::vector<MaterialObject*>	materials;

	/*wczytane pliki*/
	std::vector<Model3DFromFile*>	models;

	/*loadery dla ró¿nych formatów plików z modelami*/
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