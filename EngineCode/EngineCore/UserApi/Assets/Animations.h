#pragma once
/**@file Animations.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Deklaracja klasy Animations.*/


#include "EngineCore/UserApi/ApiGroup.h"

namespace sw {
namespace Api {
namespace Assets {

/**@brief Zapewnia dostêp do modeli w klasie AssetsManager.
@ingroup UserAPI
@ingroup AssetsAPI*/
class Animations : public ApiGroup
{
private:
protected:
public:
	Animations() = default;
	~Animations() = default;
};


}	// Assets
}	// Api
}	// sw
