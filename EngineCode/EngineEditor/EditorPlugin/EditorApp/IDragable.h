#pragma once

//========================================//
// windows include
#define WIN32_LEAN_AND_MEAN
#include <windows.h>



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
