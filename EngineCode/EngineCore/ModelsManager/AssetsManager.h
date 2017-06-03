#pragma once
/**@file AssetsManager.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Plik zawiera deklaracjê klasy AssetsManager.
*/

#include "swGraphicAPI/ResourceManager/ResourceManager.h"

#include "Assets/Meshes/MeshAsset.h"
#include "DefaultAssets.h"

class Engine;
class ILoader;


/**@defgroup ResourcesManagment Resources Managment
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

/**@brief Zawiera wyniki mo¿liwe do zwrócenia przez AssetsManager.
@deprecated*/
enum ModelsManagerResult
{
	MODELS_MANAGER_OK,					///<Poprawne wykonanie funkcji
	MODELS_MANAGER_LOADER_NOT_FOUND,	///<Nie znaleziono loadera do podanego typu pliku
	MODELS_MANAGER_CANNOT_LOAD			///<Nie da siê wczytaæ pliku
};


/**
@ingroup ResourcesManagment
@ingroup EngineCore

@brief Klasa przechowuje wszystkie obiekty zasobów w silniku.

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

	ResourceContainer< MaterialAsset >			m_material;			///< Materials.
	ResourceContainer< MeshAsset >				m_meshes;			///< Meshes.

	/*loadery dla ró¿nych formatów plików z modelami*/
	std::vector<ILoader*>						m_loader;				///<Loadery do plików z modelami 3D.

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


	// Funkcje do listowania assetów.

	std::vector< ResourcePtr< MaterialAsset > >		ListMaterials		();
	std::vector< ResourcePtr< MeshAsset > >			ListMeshes			();

private:
	ILoader*						FindLoader					( const std::wstring& path );

	virtual MemoryChunk				LoadTextureImpl				( const filesystem::Path& filePath, TextureInfo& texInfo ) override;
	
};


