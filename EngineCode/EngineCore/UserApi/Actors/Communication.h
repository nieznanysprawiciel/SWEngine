#pragma once
/**
@file Communication.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "EngineCore/UserApi/ApiGroup.h"
#include "swCommonLib/Common/EngineObject.h"
#include "EngineCore/EventsManager/Signal.h"

#include <assert.h>



namespace sw {


class Event;

namespace Api {
namespace Actors {

/**@brief Zawiera funkcje do ustawiania assetów dla aktorów.
@ingroup UserAPI
@ingroup ActorsAPI*/
class Communication : public ApiGroup
{
private:
protected:
public:
	Communication() = default;
	~Communication() = default;

	void			AddListenerDelayed		( rttr::type eventType, const EngineObject* handlerOwner, EventDelegate handler );
	void			RemoveListenerDelayed	( rttr::type eventType, const EngineObject* handlerOwner );
	void			ClearListenersDelayed	();

	void			SendEvent				( Event* evt );
	void			SendSignal				( const Signal& signal );

	template< typename EventType, typename ClassType >
	void			AddListenerDelayed		( ClassType* handlerOwner, void ( ClassType::* handler ) ( const EngineObject*, Event* ) );

	template< typename EventType >
	void			RemoveListenerDelayed	( EngineObject* handlerOwner );
};


/**@copydoc EventManager::AddListener

Wynik wykonania funkcji jest widoczny dopiero w nastêpnej klatce.
@todo Na razie funkcja dodaje delegata od razu. Poprawiæ, gdy pojawi siê wielow¹tkowoœæ.*/
template< typename EventType, typename ClassType >
inline void Communication::AddListenerDelayed( ClassType* handlerOwner, void ( ClassType::* handler )( const EngineObject*, Event* ) )
{
	assert( rttr::type::get< EventType >().is_derived_from< Event >() );

	EventDelegate delegate;
	delegate.bind( handlerOwner, static_cast<void ( EngineObject::* )( const EngineObject*, Event* )>( handler ) );
	AddListenerDelayed( rttr::type::get< EventType >(), handlerOwner, delegate );
}

template< typename EventType >
inline void Communication::RemoveListenerDelayed( EngineObject* handlerOwner )
{
	RemoveListenerDelayed( rttr::type::get< EventType >(), handlerOwner );
}

}	// Actors
}	// Api
}	// sw




