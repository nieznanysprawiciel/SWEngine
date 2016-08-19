#pragma once
/**@file ModelsManager.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy ModelsManager.
*/

#include "GraphicAPI/MeshResources.h"
#include "Model3DFromFile.h"
#include "ResourceContainer.h"
#include "DefaultAssets.h"

class Engine;
class ILoader;


/**@defgroup ResourcesManagment Zarz¹dzenie zasobami
@ingroup EngineCore
@brief Zarz¹dzanie zasobami jak vertex i pixel shadery, bufory, tekstury, materia³y, render targety i modele z plików.

Do zarz¹dzania zasobami s³u¿y klasa @ref ModelsManager. Przechowuje ona wszystkie zasoby i jako jedyna ma prawo
do kasowania posiadanych przez siebie obiektów. Wszystkie operacje typu wczytywanie zasobów odbywaj¹ siê
poprzez wywo³anie odpowiednich funkcji tej klasy.

@ref ModelsManager posiada tablicê loaderów (@ref ILoader), które s³u¿¹ do wczytywania plików z modelami.
Modele wczytane z plików umieszczane i przechowywane s¹ w klasie @ref Model3DFromFile.
*/


//-------------------------------------------------------------------------------//
//							ModelsManager
//-------------------------------------------------------------------------------//

/**@brief Zawiera wyniki mo¿liwe do zwrócenia przez ModelsManager.*/
typedef enum ModelsManagerResult
{
	MODELS_MANAGER_OK,					///<Poprawne wykonanie funkcji
	MODELS_MANAGER_LOADER_NOT_FOUND,	///<Nie znaleziono loadera do podanego typu pliku
	MODELS_MANAGER_CANNOT_LOAD			///<Nie da siê wczytaæ pliku
};


/**
@ingroup ResourcesManagment
@ingroup EngineCore

@brief Klasa przechowuje wszystkie obiekty zasobów w silniku (oprócz dŸwiêków, które raczej
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
	Engine* m_engine;			///<WskaŸnik na obiekt g³ówny silnika

	ResourceContainer<VertexShader>				m_vertexShader;		///<Vertex shadery.
	ResourceContainer<PixelShader>				m_pixelShader;		///<Pixel shadery.
	ResourceContainer<TextureObject>			m_texture;			///<Tekstury.
	ResourceContainer<BufferObject>				m_vertexBuffer;		///<Bufory wierzcho³ków.
	ResourceContainer<BufferObject>				m_indexBuffer;		///<Bufory indeksów.
	ResourceContainer<BufferObject>				m_constantBuffer;	///<Bufory sta³ych dla shaderów.
	ResourceContainer<MaterialObject>			m_material;			///<Materia³y.
	ResourceContainer<ShaderInputLayout>		m_vertexLayout;		///<Layouty dla róznych formatów wierzcho³ków.
	// UWAGA! m_fileModel musi byæ na koñcu. Jego destruktor kasuje odwo³ania do obiektów powy¿ej. Podobnie RenderTargetObject odwo³uje siê do tekstur.
	// Musz¹ one w tym czasie istnieæ, a destruktory s¹ wywo³ywane w kolejnoœci odwrotnej do zadeklarowanej w klasie.
	ResourceContainer<RenderTargetObject>		m_renderTarget;		///<Obiekty mog¹ce s³u¿yæ za render target.
	ResourceContainer<Model3DFromFile>			m_fileModel;		///<Obiekty modeli 3D z plików

	/*loadery dla ró¿nych formatów plików z modelami*/
	std::vector<ILoader*>						m_loader;				///<Loadery do plików z modelami 3D

public:
	ModelsManager( Engine* engine );
	~ModelsManager();

	// Funkcje pomocnicze
	VertexShader*					FindBestVertexShader		( TextureObject** textures );
	PixelShader*					FindBestPixelShader			( TextureObject** textures );


	// Funkcje do zarz¹dzania assetami
	ModelsManagerResult				LoadModelFromFile			( const std::wstring& file );
	RenderTargetObject*				CreateRenderTarget			( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor );

	inline Model3DFromFile*			GetModel					( const std::wstring& name ) { return m_fileModel.get( name ); }	///<Zwraca model z pliku o podanej nazwie, je¿eli jest wczytany.
	inline RenderTargetObject*		GetRenderTarget				( const std::wstring& name ) { return m_renderTarget.get( name ); }	///<Zwraca RenderTarget o podanej nazwie, je¿eli jest wczytany.
	inline VertexShader*			GetVertexShader				( const std::wstring& name ) { return m_vertexShader.get( name ); } ///<Zwraca vertex shader o podanej nazwie, je¿eli jest wczytany.
	inline PixelShader*				GetPixelShader				( const std::wstring& name ) { return m_pixelShader.get( name ); }	///<Zwraca pixel shader o podanej nazwie, je¿eli jest wczytany.
	inline TextureObject*			GetTexture					( const std::wstring& name ) { return m_texture.get( name ); }		///<Zwraca teksturê o podanej nazwie, je¿eli jest wczytany.
	inline BufferObject*			GetVertexBuffer				( const std::wstring& name ) { return m_vertexBuffer.get( name ); }	///<Zwraca bufor wierzcho³ków o podanej nazwie, je¿eli jest wczytany.
	inline BufferObject*			GetConstantBuffer			( const std::wstring& name ) { return m_constantBuffer.get( name ); }	///<Zwraca bufor sta³ych o podanej nazwie, je¿eli jest wczytany.
	inline BufferObject*			GetIndexBuffer				( const std::wstring& name ) { return m_indexBuffer.get( name ); }	///<Zwraca bufor indeksów o podanej nazwie, je¿eli jest wczytany.
	inline MaterialObject*			GetMaterial					( const std::wstring& name ) { return m_material.get( name ); }		///<Zwraca materia³ o podanej nazwie, je¿eli jest wczytany.

	TextureObject*					AddTexture					( const std::wstring& file_name );
	VertexShader*					AddVertexShader				( const std::wstring& file_name, const std::string& shader_entry );
	VertexShader*					AddVertexShader				( const std::wstring& file_name, const std::string& shader_entry, ShaderInputLayout** layout, InputLayoutDescriptor* layout_desc );
	PixelShader*					AddPixelShader				( const std::wstring& file_name, const std::string& shader_entry );
	BufferObject*					AddVertexBuffer				( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count );
	BufferObject*					AddIndexBuffer				( const std::wstring& name, const void* buffer, unsigned int element_size, unsigned int vert_count );
	BufferObject*					AddConstantsBuffer			( const std::wstring& name, const void* buffer, unsigned int size );
	
	RenderTargetObject*				AddRenderTarget				( RenderTargetObject* renderTarget, const std::wstring& name );
	MaterialObject*					AddMaterial					( MaterialObject* material, const std::wstring& material_name );

	// Funkcje do listowania assetów.

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


