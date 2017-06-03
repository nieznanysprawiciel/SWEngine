/**
@file ActorBase.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EngineCore/stdafx.h"

#include "ActorBase.h"
#include "EngineCore/MainEngine/Engine.h"

#include "swCommonLib/Serialization/PropertySerialization/Serialization.h"

#include "swCommonLib/Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::ActorBase >( "ActorBase" );
}


namespace sw
{

Engine* ActorBase::engine = nullptr;		//po stworzeniu obiektu klasy Engine, zmienna ta jest uzupe�niana wska�nikiem this



/**@deprecated Eventy wysyla sie przez API w EngineInterface. FableEngine nie odpowiada ju� za
przetwarzanie eventow.

Funkcja pozwala wys�a� event, kt�ry b�dzie potem przetworzony przez klase FableEngine.
Eventy s� metod� komunikacji pomiedzy silnikiem graficznym, silnikiem fizycznym, AI i silnikiem kolizji,
a modu�em silnika odpowiedzialnym za fabu��. Istnieje szereg event�w wbudowanych, wysy�anych przez silnik,
mo�na r�wnie� definiowa� w�asne nowe eventy poprzez dziedziczenie z klasy Event. Event mo�e by� wys�any przez dowolny
objekt poprzez wywo�anie funkcji ActorBase::event. Aby wys�a� w�asny event trzeba przeci��y� jedn� z funkcji klas wbudowanych,
kt�ra jest potem wywo�ywana przez silnik i wywo�a� t� funkj�.

Za zwolnienie pami�ci po klasie Event odpowiada klasa FabelEngine (jest to robione automatycznie po wywo�aniu funkcji obs�ugi,
u�ytkownik nie musi si� tym przejmowac).*/
void inline				ActorBase::event			( Event* new_event )
{
	engine->SendEvent( new_event );
}



/**@brief Domy�lna implementacja serializacji.

Serializuje wszystkie propertiesy oznaczone w metadanych flag� Serialize lub AllowInSaveFile.
Serialize oznacza w�a�ciwo�ci, kt�re zostan� zapisane do pliku z map�. AllowInSaveFile to w�a�ciwo�ci,
kt�re zostan� zapisane w przypadku zapisywania stanu gry.*/
void					ActorBase::Serialize		( ISerializer* ser ) const
{
	Serialization::DefaultSerialize( ser, this );
}

/**@brief Domy�lna implementacja deserializacji.*/
void					ActorBase::Deserialize		( IDeserializer* deser )
{
	GetEngineInterface()->Level.Serialization.DefaultDeserialize( deser, this );
}


}	// sw

