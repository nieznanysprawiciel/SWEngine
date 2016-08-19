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
@ingroup EngineCore

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

	ResourceContainer<VertexShader>				m_vertexShader;		///<Vertex shadery.
	ResourceContainer<PixelShader>				m_pixelShader;		///<Pixel shadery.
	ResourceContainer<TextureObject>			m_texture;			///<Tekstury.
	ResourceContainer<BufferObject>				m_vertexBuffer;		///<Bufory wierzcho�k�w.
	ResourceContainer<BufferObject>				m_indexBuffer;		///<Bufory indeks�w.
	ResourceContainer<BufferObject>				m_constantBuffer;	///<Bufory sta�ych dla shader�w.
	ResourceContainer<MaterialObject>			m_material;			///<Materia�y.
	ResourceContainer<ShaderInputLayout>		m_vertexLayout;		///<Layouty dla r�znych format�w wierzcho�k�w.
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
	VertexShader*					FindBestVertexShader		( TextureObject** textures );
	PixelShader*					FindBestPixelShader			( TextureObject** textures );


	// Funkcje do zarz�dzania assetami
	ModelsManagerResult				LoadModelFromFile			( const std::wstring& file );
	RenderTargetObject*				CreateRenderTarget			( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor );

	inline Model3DFromFile*			GetModel					( const std::wstring& name ) { return m_fileModel.get( name ); }	///<Zwraca model z pliku o podanej nazwie, je�eli jest wczytany.
	inline RenderTargetObject*		GetRenderTarget				( const std::wstring& name ) { return m_renderTarget.get( name ); }	///<Zwraca RenderTarget o podanej nazwie, je�eli jest wczytany.
	inline VertexShader*			GetVertexShader				( const std::wstring& name ) { return m_vertexShader.get( name ); } ///<Zwraca vertex shader o podanej nazwie, je�eli jest wczytany.
	inline PixelShader*				GetPixelShader				( const std::wstring& name ) { return m_pixelShader.get( name ); }	///<Zwraca pixel shader o podanej nazwie, je�eli jest wczytany.
	inline TextureObject*			GetTexture					( const std::wstring& name ) { return m_texture.get( name ); }		///<Zwraca tekstur� o podanej nazwie, je�eli jest wczytany.
	inline BufferObject*			GetVertexBuffer				( const std::wstring& name ) { return m_vertexBuffer.get( name ); }	///<Zwraca bufor wierzcho�k�w o podanej nazwie, je�eli jest wczytany.
	inline BufferObject*			GetConstantBuffer			( const std::wstring& name ) { return m_constantBuffer.get( name ); }	///<Zwraca bufor sta�ych o podanej nazwie, je�eli jest wczytany.
	inline BufferObject*			GetIndexBuffer				( const std::wstring& name ) { return m_indexBuffer.get( name ); }	///<Zwraca bufor indeks�w o podanej nazwie, je�eli jest wczytany.
	inline MaterialObject*			GetMaterial					( const std::wstring& name ) { return m_material.get( name ); }		///<Zwraca materia� o podanej nazwie, je�eli jest wczytany.

	TextureObject*					AddTexture					( const std::wstring& file_name );
	VertexShader*					AddVertexShader				( const std::wstring& file_name, const std::string& shader_entry );
	VertexShader*					AddVertexShader				( const std::wstring& file_name, const std::string& shader_entry, ShaderInputLayout** layout, InputLayoutDescriptor* layout_desc );
	PixelShader*					AddPixelShader				( const std::wstring& file_name, const std::string& shader_entry );
	BufferObject*					AddVertexBuffer				( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count );
	BufferObject*					AddIndexBuffer				( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count );
	BufferObject*					AddConstantsBuffer			( const std::wstring& name, const void* buffer, unsigned int size );
	
	RenderTargetObject*				AddRenderTarget				( RenderTargetObject* renderTarget, const std::wstring& name );
	MaterialObject*					AddMaterial					( MaterialObject* material, const std::wstring& material_name );

	// Funkcje do listowania asset�w.

	std::vector< ResourcePtr< BufferObject > >			ListVertexBuffers	();
	std::vector< ResourcePtr< BufferObject > >			ListIndexBuffers	();
	std::vector< ResourcePtr< BufferObject > >			ListConstantBuffers	();
	std::vector< ResourcePtr< ShaderInputLayout > >		ListShaderLayouts	();

	std::vector< ResourcePtr< TextureObject > >			ListTextures		();
	std::vector< ResourcePtr< VertexShader > >			ListVertexShaders	();
	std::vector< ResourcePtr< PixelShader > >			ListPixelShaders	();
	
	std::vector< ResourcePtr< MaterialObject > >		ListMaterials		();
	std::vector< ResourcePtr< RenderTargetObject > >	ListRenderTargets	();
	std::vector< ResourcePtr< Model3DFromFile > >		ListMeshes			();

private:
	ILoader*						FindLoader					( const std::wstring& path );
	

#ifdef __TEST
public:
	void test( );
#endif
};


