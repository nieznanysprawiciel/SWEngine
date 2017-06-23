#pragma once
/**
@file EngineObjectWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

class EngineObject;



namespace sw {
namespace EditorPlugin
{


public ref class EngineObjectWrapper
{
private:
protected:
	EngineObject*		m_actorPtr;		///< WskaŸnik na aktora.

public:
	EngineObjectWrapper( EngineObject* actor );

	System::String^		GetTypeName	();
	System::IntPtr		GetActorPtr	();
	EngineObject*		GetPtr		();


	property int				Type
	{
		int					get ();
	}

	property System::String^	TypeName
	{
		System::String^		get () { return GetTypeName(); }
	}
};



}	// EditorPlugin
}	// sw
