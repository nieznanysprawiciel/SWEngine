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


/**@defgroup ResourcesManagment Zarz�dzenie zasobami
@ingroup EngineCore
@brief Zarz�dzanie zasobami jak vertex i pixel shadery, bufory, tekstury, materia�y, render targety i modele z plik�w.

Do zarz�dzania zasobami s�u�y klasa @ref ModelsManager. Przechowuje ona wszystkie zasoby i jako jedyna ma prawo
do kasowania posiadanych przez siebie obiekt�w. Wszystkie operacje typu wczytywanie zasob�w odbywaj� si�
poprzez wywo�anie odpowiednich funkcji tej klasy.

@ref ModelsManager posiada tablic� loader�w (@ref ILoader), kt�re s�u�� do wczytywania plik�w z modelami.
Modele wczytane z plik�w umieszczane i przechowywane s� w klasie @ref Model3DFromFile.
*/


//-------------------------------------------------------------------------------//
//							ModelsManager
//-------------------------------------------------------------------------------//

/**@brief Zawiera wyniki mo�liwe do zwr�cenia przez ModelsManager.*/
typedef enum ModelsManagerResult
{
	MODELS_MANAGER_OK,					///<Poprawne wykonanie funkcji
	MODELS_MANAGER_LOADER_NOT_FOUND,	///<Nie znaleziono loadera do podanego typu pliku
	MODELS_MANAGER_CANNOT_LOAD			///<Nie da si� wczyta� pliku
};


/**
@ingroup ResourcesManagment
@brief Klasa przechowuje wszystkie obiekty zasob�w w silniku (opr�cz d�wi�k�w, kt�re raczej
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
	Engine* m_engine;			///<Wska�nik na obiekt g��wny silnika

	ResourceContainer<VertexShaderObject>		m_vertexShader;		///<Vertex shadery.
	ResourceContainer<PixelShaderObject>		m_pixelShader;		///<Pixel shadery.
	ResourceContainer<TextureObject>			m_texture;			///<Tekstury.
	ResourceContainer<BufferObject>				m_vertexBuffer;		///<Bufory wierzcho�k�w.
	ResourceContainer<BufferObject>				m_indexBuffer;		///<Bufory indeks�w.
	ResourceContainer<BufferObject>				m_constantBuffer;	///<Bufory sta�ych dla shader�w.
	ResourceContainer<MaterialObject>			m_material;			///<Materia�y.
	ResourceContainer<ShaderInputLayoutObject>	m_vertexLayout;		///<Layouty dla r�znych format�w wierzcho�k�w.
	// UWAGA! m_fileModel musi by� na ko�cu. Jego destruktor kasuje odwo�ania do obiekt�w powy�ej. Podobnie RenderTargetObject odwo�uje si� do tekstur.
	// Musz� one w tym czasie istnie�, a destruktory s� wywo�ywane w kolejno�ci odwrotnej do zadeklarowanej w klasie.
	ResourceContainer<RenderTargetObject>		m_renderTarget;		///<Obiekty mog�ce s�u�y� za render target.
	ResourceContainer<Model3DFromFile>			m_fileModel;		///<Obiekty modeli 3D z plik�w

	/*loadery dla r�nych format�w plik�w z modelami*/
	std::vector<ILoader*>						m_loader;				///<Loadery do plik�w z modelami 3D

public:
	ModelsManager( Engine* engine );
	~ModelsManager();

	// Funkcje pomocnicze
	VertexShaderObject*				FindBestVertexShader		( TextureObject** textures );
	PixelShaderObject*				FindBestPixelShader			( TextureObject** textures );


	// Funkcje do zarz�dzania assetami
	ModelsManagerResult			LoadModelFromFile			( const std::wstring& file );

	inline Model3DFromFile*			GetModel					( const std::wstring& name ) { return m_fileModel.get( name ); }	///<Zwraca model z pliku o podanej nazwie
	TextureObject*					AddTexture					( const std::wstring& file_name );
	MaterialObject*					AddMaterial				( MaterialObject* material, const std::wstring& material_name );
	VertexShaderObject*				AddVertexShader				( const std::wstring& file_name, const std::string& shader_entry );
	VertexShaderObject*				AddVertexShader				( const std::wstring& file_name, const std::string& shader_entry, ShaderInputLayoutObject** layout, InputLayoutDescriptor* layout_desc );
	PixelShaderObject*				AddPixelShader				( const std::wstring& file_name, const std::string& shader_entry );
	BufferObject*					AddVertexBuffer				( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count );
	BufferObject*					AddIndexBuffer				( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count );
	BufferObject*					AddConstantsBuffer			( const std::wstring& name, const void* buffer, unsigned int size );
private:
	ILoader*						FindLoader					( const std::wstring& path );
	

#ifdef __TEST
public:
	void test( );
#endif
};


