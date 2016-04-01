#include "EngineCore/stdafx.h"

#include "Object.h"
#include "EngineCore/MainEngine/Engine.h"

#include "Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< Object >( "Object" );
}


Engine* Object::engine = nullptr;		//po stworzeniu obiektu klasy Engine, zmienna ta jest uzupe�niana wska�nikiem this



/**Funkcja pozwala wys�a� event, kt�ry b�dzie potem przetworzony przez klase FableEngine.
Eventy s� metod� komunikacji pomiedzy silnikiem graficznym, silnikiem fizycznym, AI i silnikiem kolizji,
a modu�em silnika odpowiedzialnym za fabu��. Istnieje szereg event�w wbudowanych, wysy�anych przez silnik,
mo�na r�wnie� definiowa� w�asne nowe eventy poprzez dziedziczenie z klasy Event. Event mo�e by� wys�any przez dowolny
objekt poprzez wywo�anie funkcji Object::event. Aby wys�a� w�asny event trzeba przeci��y� jedn� z funkcji klas wbudowanych,
kt�ra jest potem wywo�ywana przez silnik i wywo�a� t� funkj�.

Za zwolnienie pami�ci po klasie Event odpowiada klasa FabelEngine (jest to robione automatycznie po wywo�aniu funkcji obs�ugi,
u�ytkownik nie musi si� tym przejmowac).*/
void inline Object::event(Event* new_event)
{
	engine->SendEvent(new_event);
}
