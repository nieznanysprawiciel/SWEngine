#pragma once

#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"
#include "EngineCore/Actors/ActorInfo.h"


class Engine;
class EngineObject;


namespace EditorPlugin
{


public ref class ActorWrapper
{
private:

	EngineObject*		m_actorPtr;		///< WskaŸnik na aktora.
	const ActorInfo*	m_actorInfo;	///< Informacje o modu³ach, w których znajduje siê aktor.
	System::String^		m_name;			///< Nazwa aktora wyœwietlana w edytorze.

public:
	ActorWrapper		( EngineObject* actor, const ActorInfo* actorInfo );

	System::String^		GetTypeName	();
	System::IntPtr		GetActorPtr	();
	
	bool				LoadMesh	( System::String^ meshPath );

public:
	property System::String^	ActorName
	{
		System::String^		get		()							{ return m_name; }
		void				set		( System::String^ name )	{ m_name = name; }
	}

	property int	Type
	{
		int			get ();
	}

	property System::String^	TypeName
	{
		System::String^		get ()		{ return GetTypeName(); }
	}


	property bool	EnableDisplay
	{
		bool		get	()				{ return m_actorInfo->EnableDisplay(); }
		void		set ( bool value )	{ }
	}

	property bool	EnableMovement
	{
		bool		get ()				{ return m_actorInfo->EnableMovement(); }
		void		set ( bool value )	{ }
	}

	property bool	EnablePhysic
	{
		bool		get ()				{ return m_actorInfo->EnablePhysic(); }
		void		set ( bool value )	{ }
	}

	property bool	EnableCollisions
	{
		bool		get ()				{ return m_actorInfo->EnableCollisions(); }
		void		set ( bool value )	{ }
	}

	property bool	EnableShadow
	{
		bool		get ()				{ return m_actorInfo->EnableShadow(); }
		void		set ( bool value )	{ }
	}

	property bool	EnablePreController
	{
		bool		get ()				{ return m_actorInfo->EnablePreController(); }
		void		set ( bool value )	{ }
	}

	property bool	EnablePostController
	{
		bool		get ()				{ return m_actorInfo->EnablePostController(); }
		void		set ( bool value )	{ }
	}

	property bool	IsLight
	{
		bool		get ()				{ return m_actorInfo->IsLight(); }
		void		set ( bool value )	{ }
	}

	property bool	IsCamera
	{
		bool		get ()				{ return m_actorInfo->IsCamera(); }
		void		set ( bool value )	{ }
	}
};

}	//	EditorPlugin
