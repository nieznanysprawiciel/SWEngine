#pragma once


class EngineObject;


namespace EditorPlugin
{


public ref class EngineObjectWrapper
{
private:
protected:
	EngineObject*		m_actorPtr;		///< Wska�nik na aktora.

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
		System::String^		get ()		{ return GetTypeName(); }
	}
};



}	// EditorPlugin

