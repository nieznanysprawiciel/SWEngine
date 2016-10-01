#pragma once
/**@file AssetsManager.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy AssetsManager.
*/

#include "GraphicAPI/ResourceManager.h"

#include "Model3DFromFile.h"		///< @deprecated
#include "Assets/Meshes/MeshAsset.h"
#include "DefaultAssets.h"

class Engine;
class ILoader;


/**@defgroup ResourcesManagment Zarz�dzenie zasobami
@ingroup EngineCore
@brief Zarz�dzanie zasobami jak vertex i pixel shadery, bufory, tekstury, materia�y, render targety i modele z plik�w.

Do zarz�dzania zasobami s�u�y klasa @ref AssetsManager. Przechowuje ona wszystkie zasoby i jako jedyna ma prawo
do kasowania posiadanych przez siebie obiekt�w. Wszystkie operacje typu wczytywanie zasob�w odbywaj� si�
poprzez wywo�anie odpowiednich funkcji tej klasy.

@ref AssetsManager posiada tablic� loader�w (@ref ILoader), kt�re s�u�� do wczytywania plik�w z modelami.
Modele wczytane z plik�w umieszczane i przechowywane s� w klasie @ref Model3DFromFile.
*/


//-------------------------------------------------------------------------------//
//							AssetsManager
//-------------------------------------------------------------------------------//

/**@brief Zawiera wyniki mo�liwe do zwr�cenia przez AssetsManager.*/
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
class AssetsManager : public ResourceManager
{
	friend Model3DFromFile;
private:
	Engine* m_engine;			///<Wska�nik na obiekt g��wny silnika

	ResourceContainer<MaterialObject>			m_material;			///<Materia�y.
	ResourceContainer<Model3DFromFile>			m_fileModel;		///<Obiekty modeli 3D z plik�w

	/*loadery dla r�nych format�w plik�w z modelami*/
	std::vector<ILoader*>						m_loader;				///<Loadery do plik�w z modelami 3D

public:
	explicit						AssetsManager( Engine* engine );
									~AssetsManager();

	// Funkcje pomocnicze
	VertexShader*					FindBestVertexShader		( TextureObject** textures );
	PixelShader*					FindBestPixelShader			( TextureObject** textures );


	// Funkcje do zarz�dzania assetami
	ModelsManagerResult				LoadModelFromFile			( const std::wstring& file );
	ResourcePtr< MeshAsset >		CreateMesh					( const std::wstring& name, MeshInitData&& initData );
	ResourcePtr< MeshAsset >		CreateMesh					( const std::wstring& name, MeshCreateData&& initData );
	ResourcePtr< MaterialAsset >	CreateMaterial				( const std::wstring& name, MaterialInitData&& initData );

	inline Model3DFromFile*			GetModel					( const std::wstring& name ) { return m_fileModel.get( name ); }	///<Zwraca model z pliku o podanej nazwie, je�eli jest wczytany.
	inline MaterialObject*			GetMaterial					( const std::wstring& name ) { return m_material.get( name ); }		///<Zwraca materia� o podanej nazwie, je�eli jest wczytany.

	MaterialObject*					AddMaterial					( MaterialObject* material, const std::wstring& material_name );

	// Funkcje do listowania asset�w.

	std::vector< ResourcePtr< MaterialObject > >		ListMaterials		();
	std::vector< ResourcePtr< Model3DFromFile > >		ListMeshes			();

private:
	ILoader*						FindLoader					( const std::wstring& path );

	virtual MemoryChunk				LoadTextureImpl				( const filesystem::Path& filePath, TextureInfo& texInfo ) override;
	
};


