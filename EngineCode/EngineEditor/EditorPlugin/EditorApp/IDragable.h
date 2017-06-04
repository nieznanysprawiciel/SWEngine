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
	/// To jest straszny hack, który powsta³, ¿eby klasa ActorClassMetaInfo
	/// mog³a dziedziczyæ po IDragable.

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
