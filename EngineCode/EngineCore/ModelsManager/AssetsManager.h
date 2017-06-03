#pragma once
/**@file AssetsManager.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy AssetsManager.
*/

#include "swGraphicAPI/ResourceManager/ResourceManager.h"

#include "Assets/Meshes/MeshAsset.h"
#include "DefaultAssets.h"

class Engine;
class ILoader;


/**@defgroup ResourcesManagment Resources Managment
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

/**@brief Zawiera wyniki mo�liwe do zwr�cenia przez AssetsManager.
@deprecated*/
enum ModelsManagerResult
{
	MODELS_MANAGER_OK,					///<Poprawne wykonanie funkcji
	MODELS_MANAGER_LOADER_NOT_FOUND,	///<Nie znaleziono loadera do podanego typu pliku
	MODELS_MANAGER_CANNOT_LOAD			///<Nie da si� wczyta� pliku
};


/**
@ingroup ResourcesManagment
@ingroup EngineCore

@brief Klasa przechowuje wszystkie obiekty zasob�w w silniku.

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

	ResourceContainer< MaterialAsset >			m_material;			///< Materials.
	ResourceContainer< MeshAsset >				m_meshes;			///< Meshes.

	/*loadery dla r�nych format�w plik�w z modelami*/
	std::vector<ILoader*>						m_loader;				///<Loadery do plik�w z modelami 3D.

public:
	explicit						AssetsManager( Engine* engine );
									~AssetsManager();

	///@name Shader matching
	///@detail Functions look for best matching default shaders for texture array.
	///Textures have same meaning as @ref TextureUse enumaration. If your textures have other meaning
	///provide your own shaders instead.
	///@{
	VertexShader*					FindBestVertexShader		( TextureObject** textures );
	PixelShader*					FindBestPixelShader			( TextureObject** textures );
	GeometryShader*					FindBestGeometryhader		( TextureObject** textures );
	EvaluationShader*				FindBestEvaluationShader	( TextureObject** textures );
	ControlShader*					FindBestControlShader		( TextureObject** textures );

	void							FillBestShaders				( MaterialInitData& initData );
	///@}

	///@name Assets loading
	///@detail Load assets from specified file. Functions protect from loading assets multiple times.
	///@{
	ResourcePtr< MeshAsset >		LoadMesh					( const filesystem::Path& file );
	ResourcePtr< MaterialAsset >	LoadMaterial				( const filesystem::Path& file );
	///@}


	///@name Assets creation
	///@detail You can create assets in code using these functions. Remember to give unique names for your assets.
	///Engine uses convention, that all generated resources have :: before name, to distinguish them from assets loaded from files.
	///@{
	ResourcePtr< MeshAsset >		CreateMesh					( const std::wstring& name, MeshInitData&& initData );
	ResourcePtr< MeshAsset >		CreateMesh					( const std::wstring& name, MeshCreateData&& initData );
	ResourcePtr< MaterialAsset >	CreateMaterial				( const std::wstring& name, MaterialInitData&& initData );
	ResourcePtr< MaterialAsset >	CreateMaterial				( const std::wstring& name, MaterialCreateData&& initData );
	///@}

	inline ResourcePtr< MeshAsset >		GetMesh					( const std::wstring& name )	{ return m_meshes.get( filesystem::Path( name ).WString() ); }
	inline ResourcePtr< MaterialAsset >	GetMaterial				( const std::wstring& name )	{ return m_material.get( filesystem::Path( name ).WString() ); }


	// Funkcje do listowania asset�w.

	std::vector< ResourcePtr< MaterialAsset > >		ListMaterials		();
	std::vector< ResourcePtr< MeshAsset > >			ListMeshes			();

private:
	ILoader*						FindLoader					( const std::wstring& path );

	virtual MemoryChunk				LoadTextureImpl				( const filesystem::Path& filePath, TextureInfo& texInfo ) override;
	
};


