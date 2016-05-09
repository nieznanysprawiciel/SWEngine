#pragma once

//========================================//
// windows include
#define WIN32_LEAN_AND_MEAN
#include <windows.h>



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
