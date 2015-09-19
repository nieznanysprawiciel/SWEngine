#pragma once
/**
@file IGraphicAPIInitializer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja interfejsu IGraphicAPIInitializer.
*/

#include "GraphicAPI/IRenderer.h"
#include "Common/TypesDefinitions.h"

#include <string>

/**@defgroup GraphicAPI
@ingroup ModulesStructure
@brief Interfejsy dla obiektów zasobów, renderera i initializera.

@EngineCore jest niezale¿ny od konkretnej implementacji API graficznego, która
zosta³a u¿yta. W tym celu w tym projekcie zgromadzone s¹ wszystkie interfejsy,
które powinny zostaæ zaimplementowane przez dziedzicz¹ce API oraz zestawy sta³ych silnikowych
niezale¿nych od platformy.

Pliki w tym projekcie powinny zostaæ w³¹czone do ka¿dego projektu, który u¿ywa b¹dŸ
implementuje API graficzne. Oprócz interfejsów do zaimplementowania, projekt zawiera te¿
pliki .cpp, które powinny byæ skompilowane do biblioteki statycznej razem z poszczególnymi API graficznymi.*/


/**@brief Przechowuje informacje potrzebne do inicjalizacji
API graficznego. Parametr dla funkcji @ref IGraphicAPIInitializer::InitAPI.
@ingroup GraphicAPI*/
struct GraphicAPIInitData
{
	uint16 windowWidth;
	uint16 windowHeight;
	uint32 windowHandle;
	bool singleThreaded;
	bool fullScreen;
	ResourceFormat depthStencilFormat;

	void DefaultSettings()
	{
		windowWidth = 1024;
		windowHeight = 768;
		singleThreaded = true;
		fullScreen = false;
		depthStencilFormat = ResourceFormat::RESOURCE_FORMAT_D24_UNORM_S8_UINT;
	}
};

/**@brief Interfejs klasy do inicjowania API graficznego.

Klasa powinna zostaæ zainicjowana przez dziedziczace API graficzne.
Podstawowymi funkcjonalnoœciami klasy jest:
- zainicjowanie i zwolnienie API
- stworzenie rendererów*/
class IGraphicAPIInitializer
{
private:
protected:
public:
	virtual ~IGraphicAPIInitializer() = default;

	virtual IRenderer*		CreateRenderer			( RendererUsage usage ) = 0;
	virtual bool			InitAPI					( GraphicAPIInitData& initData ) = 0;
	virtual void			ReleaseAPI				() = 0;

	// Future
	// virtual std::wstring	GetErrorString() = 0;
	// virtual void			Resize( uint16 newWidth, uint16 newHeight ) = 0;
};
