#pragma once
/**
@file IDragable.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


//========================================//
// windows include
#include "EngineEditor/EditorPlugin/IncludeWindows.h"



namespace EditorApp { namespace GUI
{
	/// To jest straszny hack, kt�ry powsta�, �eby klasa ActorClassMetaInfo
	/// mog�a dziedziczy� po IDragable.

	public interface class IDragable
	{
	public:
		property System::Type^		DataType
		{
			virtual System::Type^	get () = 0;
		}
	
		virtual void	Remove( System::Object^ i ) = 0;
	};


}	// GUI
}	// EditorApp
