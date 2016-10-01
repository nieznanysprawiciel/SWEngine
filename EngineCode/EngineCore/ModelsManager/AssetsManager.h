#pragma once
/**@file AssetsManager.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy AssetsManager.
*/

#include "GraphicAPI/ResourceManager.h"

#include "Model3DFromFile.h"		///< @deprecated
#include "Assets/Meshes/MeshAsset.h"
#include "DefaultAssets.h"

class Engine;
class ILoader;


/**@defgroup ResourcesManagment Zarz¹dzenie zasobami
@ingroup EngineCore
@brief Zarz¹dzanie zasobami jak vertex i pixel shadery, bufory, tekstury, materia³y, render targety i modele z plików.

Do zarz¹dzania zasobami s³u¿y klasa @ref AssetsManager. Przechowuje ona wszystkie zasoby i jako jedyna ma prawo
do kasowania posiadanych przez siebie obiektów. Wszystkie operacje typu wczytywanie zasobów odbywaj¹ siê
poprzez wywo³anie odpowiednich funkcji tej klasy.

@ref AssetsManager posiada tablicê loaderów (@ref ILoader), które s³u¿¹ do wczytywania plików z modelami.
Modele wczytane z plików umieszczane i przechowywane s¹ w klasie @ref Model3DFromFile.
*/


//-------------------------------------------------------------------------------//
//							AssetsManager
//-------------------------------------------------------------------------------//

/**@brief Zawiera wyniki mo¿liwe do zwrócenia przez AssetsManager.*/
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
class AssetsManager : public ResourceManager
{
	friend Model3DFromFile;
private:
	Engine* m_engine;			///<WskaŸnik na obiekt g³ówny silnika

	ResourceContainer<MaterialObject>			m_material;			///<Materia³y.
	ResourceContainer<Model3DFromFile>			m_fileModel;		///<Obiekty modeli 3D z plików

	/*loadery dla ró¿nych formatów plików z modelami*/
	std::vector<ILoader*>						m_loader;				///<Loadery do plików z modelami 3D

public:
	explicit						AssetsManager( Engine* engine );
									~AssetsManager();

	// Funkcje pomocnicze
	VertexShader*					FindBestVertexShader		( TextureObject** textures );
	PixelShader*					FindBestPixelShader			( TextureObject** textures );


	// Funkcje do zarz¹dzania assetami
	ModelsManagerResult				LoadModelFromFile			( const std::wstring& file );
	ResourcePtr< MeshAsset >		CreateMesh					( const std::wstring& name, MeshInitData&& initData );
	ResourcePtr< MeshAsset >		CreateMesh					( const std::wstring& name, MeshCreateData&& initData );
	ResourcePtr< MaterialAsset >	CreateMaterial				( const std::wstring& name, MaterialInitData&& initData );

	inline Model3DFromFile*			GetModel					( const std::wstring& name ) { return m_fileModel.get( name ); }	///<Zwraca model z pliku o podanej nazwie, je¿eli jest wczytany.
	inline MaterialObject*			GetMaterial					( const std::wstring& name ) { return m_material.get( name ); }		///<Zwraca materia³ o podanej nazwie, je¿eli jest wczytany.

	MaterialObject*					AddMaterial					( MaterialObject* material, const std::wstring& material_name );

	// Funkcje do listowania assetów.

	std::vector< ResourcePtr< MaterialObject > >		ListMaterials		();
	std::vector< ResourcePtr< Model3DFromFile > >		ListMeshes			();

private:
	ILoader*						FindLoader					( const std::wstring& path );

	virtual MemoryChunk				LoadTextureImpl				( const filesystem::Path& filePath, TextureInfo& texInfo ) override;
	
};


