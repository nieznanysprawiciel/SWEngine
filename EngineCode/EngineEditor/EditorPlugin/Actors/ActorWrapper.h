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
	const ActorInfo*	m_actorInfo;	///< Informacje o modu³¹ch, w których znajduje siê aktor.
	System::String^		m_name;			///< Nazwa aktora wyœwietlana w edytorze.

public:
	ActorWrapper		( EngineObject* actor, const ActorInfo* actorInfo );

	System::String^		GetTypeName();
	System::IntPtr		GetActorPtr();

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


	property bool	EnableDisplay
	{
		bool		get	()				{ return m_actorInfo->enableDisplay; }
	}

	property bool	EnableMovement
	{
		bool		get ()				{ return m_actorInfo->enableMovement; }
	}

	property bool	EnablePhysic
	{
		bool		get ()				{ return m_actorInfo->enablePhysic; }
	}

	property bool	EnableCollisions
	{
		bool		get ()				{ return m_actorInfo->enableCollisions; }
	}

	property bool	EnableShadow
	{
		bool		get ()				{ return m_actorInfo->enableShadow; }
	}

	property bool	EnablePreController
	{
		bool		get ()				{ return m_actorInfo->enablePreController; }
	}

	property bool	EnablePostController
	{
		bool		get ()				{ return m_actorInfo->enablePostController; }
	}

	property bool	IsLight
	{
		bool		get ()				{ return m_actorInfo->isLight; }
	}

	property bool	IsCamera
	{
		bool		get ()				{ return m_actorInfo->isCamera; }
	}
};

}	//	EditorPlugin
