#pragma once
/**@file ILoader.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Zawiera deklaracj� interfejsu dla loader�w plik�w z modelami.
*/

#include "swCommonLib/Common/Nullable.h"
#include "swCommonLib/System/Path.h"
#include "swGraphicAPI/Resources/MeshResources.h"
#include "EngineCore/ModelsManager/AssetsManager.h"
#include "EngineCore/ModelsManager/Assets/Meshes/MeshAssetInitData.h"


namespace sw
{



/**@defgroup MakingLoaders Pisanie loader�w
@brief Instrukcje jak napisa� loader do wczytywania w�asnego formatu plik�w z modelami 3D.
@ingroup ResourcesManagment

@todo Napisa� dokumentacj�.*/

/**@brief Warto�ci, mo�liwe do zwr�cenia przez klas� Loader
*/
enum LoaderResult
{
	MESH_LOADING_OK,				///<Poprawne wczytywanie mesha
	MESH_LOADING_WRONG,				///<Niepoprawne wczytywanie mesha
	MESH_LOADING_OUT_OF_MEMORY,		///<Brakuje pami��
	MESH_LOADING_WRONG_FILE_FORMAT	///<Niepoprawny format pliku
};

/**
@ingroup MakingLoaders
@brief Interfejs klasy do wczytywania plik�w z modelami.
Aby stworzy� klas� do wczytywania w�asnego formatu plik�w nale�y odziedziczy� po tym interfejsie
i zaimplementowa� metody wirtualne.

Dok�adniejsze informacje o tworzeniu w�asnych loader�w znajduj� si� w paragrafie @ref MakingLoaders.
@note Po wywo�aniu konstruktora klasa ma by� w pe�ni gotowa do dzia�ania. Podobnie po ka�dym wywo�aniu funkcji �aduj�cej
plik, klasa tak�e ma by� gotowa do wczytania kolejnego.
*/
class ILoader
{
protected:
	AssetsManager*		models_manager;
public:
	ILoader( AssetsManager* models_manager ) : models_manager( models_manager ) {};
	virtual ~ILoader() {};


	virtual Nullable< MeshInitData >		LoadMesh	( const filesystem::Path& fileName ) { return "Function should be overriden in derived class."; }

	/**@brief Checks if laoder can load file.
	This function should chack fileName's extension and determine if it can load file.*/
	virtual bool							CanLoad		( const filesystem::Path& fileName ) { return false; }
};


}	// sw

