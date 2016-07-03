#pragma once

#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"
#include "EngineCore/Actors/ActorInfo.h"


class Engine;
class EngineObject;
class ActorBase;


namespace EditorPlugin
{



public ref class ActorWrapper
{
private:

	EngineObject*		m_actorPtr;		///< WskaŸnik na aktora.
	unsigned short		m_actorInfo;	///< Informacje o modu³ach, w których znajduje siê aktor.
	System::String^		m_name;			///< Nazwa aktora wyœwietlana w edytorze.

public:
	ActorWrapper		( EngineObject* actor, const ActorInfo* actorInfo );

	System::String^		GetTypeName	();
	System::IntPtr		GetActorPtr	();
	ActorBase*			Ptr			();
	unsigned short		GetActorInfo()		{ return m_actorInfo; }
	
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
		bool		get	()				{ return ActorInfo( m_actorInfo ).EnableDisplay(); }
		void		set ( bool value )	{ SetActorInfo( value, ActorInfoFlag::EnableDisplay ); }
	}

	property bool	EnableMovement
	{
		bool		get ()				{ return ActorInfo( m_actorInfo ).EnableMovement(); }
		void		set ( bool value )	{ SetActorInfo( value, ActorInfoFlag::EnableMovement ); }
	}

	property bool	EnablePhysic
	{
		bool		get ()				{ return ActorInfo( m_actorInfo ).EnablePhysic(); }
		void		set ( bool value )	{ SetActorInfo( value, ActorInfoFlag::EnablePhysic ); }
	}

	property bool	EnableCollisions
	{
		bool		get ()				{ return ActorInfo( m_actorInfo ).EnableCollisions(); }
		void		set ( bool value )	{ SetActorInfo( value, ActorInfoFlag::EnableCollisions ); }
	}

	property bool	EnableShadow
	{
		bool		get ()				{ return ActorInfo( m_actorInfo ).EnableShadow(); }
		void		set ( bool value )	{ SetActorInfo( value, ActorInfoFlag::EnableShadow ); }
	}

	property bool	EnablePreController
	{
		bool		get ()				{ return ActorInfo( m_actorInfo ).EnablePreController(); }
		void		set ( bool value )	{ SetActorInfo( value, ActorInfoFlag::EnablePreController ); }
	}

	property bool	EnablePostController
	{
		bool		get ()				{ return ActorInfo( m_actorInfo ).EnablePostController(); }
		void		set ( bool value )	{ SetActorInfo( value, ActorInfoFlag::EnablePostController ); }
	}

	property bool	IsLight
	{
		bool		get ()				{ return ActorInfo( m_actorInfo ).IsLight(); }
		void		set ( bool value )	{ SetActorInfo( value, ActorInfoFlag::AsLight ); }
	}

	property bool	IsCamera
	{
		bool		get ()				{ return ActorInfo( m_actorInfo ).IsCamera(); }
		void		set ( bool value )	{ SetActorInfo( value, ActorInfoFlag::AsCamera ); }
	}

	property bool	EnableSavingToFile
	{
		bool		get ()				{ return ActorInfo( m_actorInfo ).EnableSavingToFile(); }
		void		set ( bool value )	{ SetActorInfo( value, ActorInfoFlag::EnableSavingToFile ); }
	}

private:
	void			SetActorInfo	( bool value, ActorInfoFlag flag )
	{
		ActorInfo newInfo = m_actorInfo;
		value ? newInfo += ActorInfo( flag ) : newInfo -= ActorInfo( flag );
		m_actorInfo = newInfo.actorFlags;
	}
};

}	//	EditorPlugin
