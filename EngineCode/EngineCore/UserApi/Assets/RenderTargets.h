#pragma once
/**@file RenderTargets.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Deklaracja klasy RenderTargets.*/

class RenderTargetObject;
struct RenderTargetDescriptor;


#include "EngineCore/UserApi/ApiGroup.h"


#include <string>

namespace Api { namespace Assets {

	/**@brief Zapewnia dost�p do render target�w w klasie ModelsManager.
	@ingroup UserAPI
	@ingroup AssetsAPI*/
	class RenderTargets : public ApiGroup
	{
	private:
	protected:
	public:
		RenderTargets() = default;
		~RenderTargets() = default;

		RenderTargetObject*		CreateSync	( const std::wstring& name, const RenderTargetDescriptor& renderTargetDescriptor );
	};


}	// Assets
}	// Api
