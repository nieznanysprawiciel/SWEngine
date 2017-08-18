#include "stdafx.h"

#include "EngineCore/MainEngine/Engine.h"
#include "swGUI/Native/WinAPI/WinAPIGUI.h"

#include "GamePlayCode/EntryPointGamePlay.h"
//#include "EngineEditor/LightmapsTool/LightmapLogic.h"

#include "swCommonLib/Common/MemoryLeaks.h"


int main( int argc, char** argv )
{
	sw::Engine engine( argc, argv, sw::gui::WinAPIGUI::Create() );
	
	if( engine.Init() )
	{
		EntryPointGamePlay* entryPoint = new EntryPointGamePlay();	// Engine will delete this!!

		engine.SetEntryPoint( entryPoint );
		engine.test();		//to potem zniknie

		return engine.MainLoop();
	}

	return 1;
}