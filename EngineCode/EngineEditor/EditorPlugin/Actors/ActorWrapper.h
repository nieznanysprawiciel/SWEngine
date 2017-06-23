#pragma once
/**
@file ActorWrapper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EngineEditor/EditorPlugin/EnginePointerProvider.h"
#include "EngineEditor/EditorPlugin/Actors/EngineObjectWrapper.h"
#include "EngineCore/Actors/ActorInfo.h"


class EngineObject;



namespace sw
{


class Engine;
class ActorBase;


namespace EditorPlugin
{



public ref class ActorWrapper : public EngineObjectWrapper
{
private:

	unsigned short		m_actorInfo;	///< Informacje o modu�ach, w kt�rych znajduje si� aktor.
	System::String^		m_name;			///< Nazwa aktora wy�wietlana w edytorze.

public:
	ActorWrapper		( EngineObject* actor, const ActorInfo* actorInfo );

	ActorBase*			Ptr			();
	unsigned short		GetActorInfo() { return m_actorInfo; }

	bool				LoadMesh	( System::String^ meshPath );

public:
	property System::String^	ActorName
	{
		System::String^		get		() { return m_name; }
		void				set		( System::String^ name ) { m_name = name; }
	}


	property bool	EnableDisplay
	{
		bool		get	() { return ActorInfo( m_actorInfo ).EnableDisplay(); }
		void		set ( bool value ) { SetActorInfo( value, ActorInfoFlag::EnableDisplay ); }
	}

	property bool	EnableMovement
	{
		bool		get () { return ActorInfo( m_actorInfo ).EnableMovement(); }
		void		set ( bool value ) { SetActorInfo( value, ActorInfoFlag::EnableMovement ); }
	}

	property bool	EnablePhysic
	{
		bool		get () { return ActorInfo( m_actorInfo ).EnablePhysic(); }
		void		set ( bool value ) { SetActorInfo( value, ActorInfoFlag::EnablePhysic ); }
	}

	property bool	EnableCollisions
	{
		bool		get () { return ActorInfo( m_actorInfo ).EnableCollisions(); }
		void		set ( bool value ) { SetActorInfo( value, ActorInfoFlag::EnableCollisions ); }
	}

	property bool	EnableShadow
	{
		bool		get () { return ActorInfo( m_actorInfo ).EnableShadow(); }
		void		set ( bool value ) { SetActorInfo( value, ActorInfoFlag::EnableShadow ); }
	}

	property bool	EnablePreController
	{
		bool		get () { return ActorInfo( m_actorInfo ).EnablePreController(); }
		void		set ( bool value ) { SetActorInfo( value, ActorInfoFlag::EnablePreController ); }
	}

	property bool	EnablePostController
	{
		bool		get () { return ActorInfo( m_actorInfo ).EnablePostController(); }
		void		set ( bool value ) { SetActorInfo( value, ActorInfoFlag::EnablePostController ); }
	}

	property bool	IsLight
	{
		bool		get () { return ActorInfo( m_actorInfo ).IsLight(); }
		void		set ( bool value ) { SetActorInfo( value, ActorInfoFlag::AsLight ); }
	}

	property bool	IsCamera
	{
		bool		get () { return ActorInfo( m_actorInfo ).IsCamera(); }
		void		set ( bool value ) { SetActorInfo( value, ActorInfoFlag::AsCamera ); }
	}

	property bool	EnableSavingToFile
	{
		bool		get () { return ActorInfo( m_actorInfo ).EnableSavingToFile(); }
		void		set ( bool value ) { SetActorInfo( value, ActorInfoFlag::EnableSavingToFile ); }
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
}	// sw

