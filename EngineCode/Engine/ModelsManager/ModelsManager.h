#pragma once

#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"

class Engine;
class Loader;

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

