#pragma once
/**@file ModelsManager.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy ModelsManager.
*/

#include "GraphicAPI/MeshResources.h"
#include "Model3DFromFile.h"
#include "ResourceContainer.h"
#include "DefaultAssets.h"

class Engine;
class ILoader;




//-------------------------------------------------------------------------------//
//							ModelsManager
//-------------------------------------------------------------------------------//

/**@brief Zawiera wyniki mo�liwe do zwr�cenia przez ModelsManager.*/
typedef enum MODELS_MANAGER_RESULT
{
	MODELS_MANAGER_OK,					///<Poprawne wykonanie funkcji
	MODELS_MANAGER_LOADER_NOT_FOUND,	///<Nie znaleziono loadera do podanego typu pliku
	MODELS_MANAGER_CANNOT_LOAD			///<Nie da si� wczyta� pliku
};


/**@brief Klasa przechowuje wszystkie obiekty zasob�w w silniku (opr�cz d�wi�k�w, kt�re raczej
zostan� oddelegowane w inne miejsce).

Zasoby s� identyfikowane jednoznacznym identyfikatorem, kt�ry jest unikalny
jedynie w danej grupie zasob�w, lub te� nazw� pliku na podstawie, kt�rego dany zas�b powsta�.

Materia�y s� specyficzne, poniewa� jeden plik z programu do modelowania raczej zawiera
wiele r�nych materia��w. �eby je mo�na by�o w ten spos�b jednoznacznie identyfikowa�,
do nazwy pliku doklejane s� dwa dwukropki :: i nazwa materia�u, jaka zosta�a mu nadana
w pliku, z kt�rego pochodzi.
Oczywi�cie, je�eli materia� zosta� stworzony r�cznie w silniku, to nie ma potrzeby
nadawania mu takiej nazwy, wystarczy, �eby by�a ona unikatowa.*/
class ModelsManager
{
	friend Model3DFromFile;
private:
	Engine* engine;			///<Wska�nik na obiekt g��wny silnika

	ResourceContainer<VertexShaderObject>		vertex_shader;		///<Vertex shadery.
	ResourceContainer<PixelShaderObject>		pixel_shader;		///<Pixel shadery.
	ResourceContainer<TextureObject>			texture;			///<Tekstury.
	ResourceContainer<BufferObject>				vertex_buffer;		///<Bufory wierzcho�k�w.
	ResourceContainer<BufferObject>				index_buffer;		///<Bufory indeks�w.
	ResourceContainer<BufferObject>				constant_buffer;	///<Bufory sta�ych dla shader�w.
	ResourceContainer<MaterialObject>			material;			///<Materia�y.
	ResourceContainer<ShaderInputLayoutObject>	vertexLayout;		///<Layouty dla r�znych format�w wierzcho�k�w.
	// UWAGA! file_model musi by� na ko�cu. Jego destruktor kasuje odwo�ania do obiekt�w powy�ej. Podobnie RenderTargetObject odwo�uje si� do tekstur.
	// Musz� one w tym czasie istnie�, a destruktory s� wywo�ywane w kolejno�ci odwrotnej do zadeklarowanej w klasie.
	ResourceContainer<RenderTargetObject>		render_target;		///<Obiekty mog�ce s�u�y� za render target.
	ResourceContainer<Model3DFromFile>			file_model;			///<Obiekty modeli 3D z plik�w

	/*loadery dla r�nych format�w plik�w z modelami*/
	std::vector<ILoader*>						loader;				///<Loadery do plik�w z modelami 3D

public:
	ModelsManager( Engine* engine );
	~ModelsManager();

	// Funkcje pomocnicze
	VertexShaderObject*				find_best_vertex_shader		( TextureObject** textures );
	PixelShaderObject*				find_best_pixel_shader		( TextureObject** textures );


	// Funkcje do zarz�dzania assetami
	MODELS_MANAGER_RESULT			load_model_from_file		( const std::wstring& file );

	inline Model3DFromFile*			get_model					( const std::wstring& name ) { return file_model.get( name ); }	///<Zwraca model z pliku o podanej nazwie
	TextureObject*					add_texture					( const std::wstring& file_name );
	MaterialObject*					add_material				( const MaterialObject* material, const std::wstring& material_name );
	VertexShaderObject*				add_vertex_shader			( const std::wstring& file_name, const std::string& shader_entry );
	VertexShaderObject*				add_vertex_shader			( const std::wstring& file_name, const std::string& shader_entry, ShaderInputLayoutObject** layout, InputLayoutDescriptor* layout_desc );
	PixelShaderObject*				add_pixel_shader			( const std::wstring& file_name, const std::string& shader_entry );
	BufferObject*					add_vertex_buffer			( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count );
	BufferObject*					add_index_buffer			( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count );
	BufferObject*					AddConstantsBuffer			( const std::wstring& name, const void* buffer, unsigned int size );
private:
	ILoader*						find_loader					( const std::wstring& path );
	

#ifdef __TEST
public:
	void test( );
#endif
};


