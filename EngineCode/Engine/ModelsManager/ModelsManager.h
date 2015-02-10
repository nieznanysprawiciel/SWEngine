#pragma once

/**@file ModelsManager.h
Plik zawiera deklaracjê klasy ModelsManager.
*/

#include "..\..\stdafx.h"
#include "meshes_textures_materials.h"
#include "ResourceContainer.h"

class Engine;
class Loader;



//-------------------------------------------------------------------------------//
//							wersja DirectX11
//-------------------------------------------------------------------------------//

/**@brief Zawiera wyniki mo¿liwe do zwrócenia przez ModelsManager.*/
typedef enum MODELS_MANAGER_RESULT
{
	MODELS_MANAGER_OK,					///<Poprawne wykonanie funkcji
	MODELS_MANAGER_LOADER_NOT_FOUND,	///<Nie znaleziono loadera do podanego typu pliku
	MODELS_MANAGER_CANNOT_LOAD			///<Nie da siê wczytaæ pliku
};


/**@brief Klasa przechowuje wszystkie obiekty zasobów w silniku (oprócz dŸwiêków, które raczej
zostan¹ oddelegowane w inne miejsce).

Zasoby s¹ identyfikowane jednoznacznym identyfikatorem, który jest unikalny
jedynie w danej grupie zasobów, lub te¿ nazw¹ pliku na podstawie, którego dany zasób powsta³.

Materia³y s¹ specyficzne, poniewa¿ jeden plik z programu do modelowania raczej zawiera
wiele ró¿nych materia³ów. ¯eby je mo¿na by³o w ten sposób jednoznacznie identyfikowaæ,
do nazwy pliku doklejane s¹ dwa dwukropki :: i nazwa materia³u, jaka zosta³a mu nadana
w pliku, z którego pochodzi.
Oczywiœcie, je¿eli materia³ zosta³ stworzony rêcznie w silniku, to nie ma potrzeby
nadawania mu takiej nazwy, wystarczy, ¿eby by³a ona unikatowa.*/
class ModelsManager
{
	friend Model3DFromFile;
private:
	Engine* engine;			///<WskaŸnik na obiekt g³ówny silnika

	ResourceContainer<VertexShaderObject*>		vertex_shader;		///<Vertex shadery
	ResourceContainer<PixelShaderObject*>		pixel_shader;		///<Pixel shadery
	ResourceContainer<TextureObject*>			texture;			///<Tekstury
	ResourceContainer<BufferObject*>			vertex_buffer;		///<Bufory wierzcho³ków
	ResourceContainer<BufferObject*>			index_buffer;		///<Bufory indeksów
	ResourceContainer<MaterialObject*>			material;			///<Materia³y
	// UWAGA! file_model musi byæ na koñcu. Jego destruktor kasuje odwo³ania do obiektów powy¿ej.
	// Musz¹ one w tym czasie istnieæ, a destruktory s¹ wywo³ywane w kolejnoœci odwrotnej do zadeklarowanej w klasie.
	ResourceContainer<Model3DFromFile*>			file_model;			///<Obiekty modeli 3D z plików

	/*loadery dla ró¿nych formatów plików z modelami*/
	std::vector<Loader*>			loader;			///<Loadery do plików z modelami 3D

public:
	ModelsManager( Engine* engine );
	~ModelsManager( );

	// Funkcje pomocnicze
	VertexShaderObject* find_best_vertex_shader( TextureObject** textures );
	PixelShaderObject* find_best_pixel_shader( TextureObject** textures );
	void set_default_assets( ID3D11VertexShader* vert_shader, ID3D11PixelShader* pix_shader );


	// Funkcje do zarz¹dzania assetami
	MODELS_MANAGER_RESULT load_model_from_file( const std::wstring& file );

	inline Model3DFromFile* get_model( const std::wstring& name ) { return file_model.get( name ); }	///<Zwraca model z pliku o podanej nazwie

private:
	Loader* find_loader( const std::wstring& path );
	

#ifdef __TEST
public:
	void test( );
#endif
};


