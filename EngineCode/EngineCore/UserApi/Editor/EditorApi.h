#pragma once
/**
@file EditorApi.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/UserApi/ApiGroup.h"

// Use only includes which compile under clr.
#include "swInputLibrary/InputCore/IInput.h"
#include "swGUI/Native/INativeGUI.h"


namespace sw
{

class EngineInterface;
struct CameraData;

}	// sw



namespace sw {
namespace Api
{



/**@brief This class is interface for editor.

All functions that shouldn't be reachable by engine user can be placed here. This class won't be instaciated in @ref EngineInterface.*/
class EditorApi : public ApiGroup
{
private:
protected:
public:

	/**@brief Creates engine object.*/
	static EngineInterface*		CreateEngine				( gui::INativeGUI* gui, input::IInput* input, uint16 width, uint16 height );

	/**@brief Destroy engine object.*/
	static void					ReleaseEngine				();

	/**@brief Gets editor preview render target handle.*/
	static void*				GetMainRenderTargetHandle	();

	/**@brief Gets currently set main camera info.*/
	static CameraData&			GetMainCamera				();

	/**@brief Returns internal engine object.*/
	static EngineInterface*		GetEngine					();

	///@name Input
	///@brief Allows to change module in runtime. Usufull only when editor starts diplaying game in window instead of preview.
	///@{
	static void					EnableInput			( bool val );
	static input::IInput*		ChangeInputModule	( input::IInput* newModule );
	///@}

	///@name Main loop functions
	///@{
	static void					UpdateScene			( float& lag, float timeInterval );
	static void					RenderScene			( float lag, float timeInterval );

	static void					Flush				();
	///@}

	///@name Test functions
	///@brief Test functions to create default scene and so on.
	///@{
	static void					Test				();
	static void					SetSkydomeAndCamera	();
	static void					CreateDefaultScene	();
	///@}
};



}	// Api
}	// sw


