#pragma once

#include "ResourcePropertyWrapper.h"


namespace EditorPlugin
{




public ref class TexturePropertyWrapper : ResourcePropertyWrapper
{
private:
public:
	TexturePropertyWrapper( rttr::property prop );


	virtual void			ResetActor		( System::IntPtr objectPtr ) override;
	virtual void			BuildHierarchy	( rttr::type classType ) override;
};


}	// EditorPlugin
