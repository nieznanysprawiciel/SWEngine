#include "EngineCore/stdafx.h"

#include "Object.h"
#include "EngineCore/MainEngine/Engine.h"

#include "Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< Object >( "Object" );
}


Engine* Object::engine = nullptr;		//po stworzeniu obiektu klasy Engine, zmienna ta jest uzupe³niana wskaŸnikiem this



/**Funkcja pozwala wys³aæ event, który bêdzie potem przetworzony przez klase FableEngine.
Eventy s¹ metod¹ komunikacji pomiedzy silnikiem graficznym, silnikiem fizycznym, AI i silnikiem kolizji,
a modu³em silnika odpowiedzialnym za fabu³ê. Istnieje szereg eventów wbudowanych, wysy³anych przez silnik,
mo¿na równie¿ definiowaæ w³asne nowe eventy poprzez dziedziczenie z klasy Event. Event mo¿e byæ wys³any przez dowolny
objekt poprzez wywo³anie funkcji Object::event. Aby wys³aæ w³asny event trzeba przeci¹¿yæ jedn¹ z funkcji klas wbudowanych,
która jest potem wywo³ywana przez silnik i wywo³aæ tê funkjê.

Za zwolnienie pamiêci po klasie Event odpowiada klasa FabelEngine (jest to robione automatycznie po wywo³aniu funkcji obs³ugi,
u¿ytkownik nie musi siê tym przejmowac).*/
void inline Object::event(Event* new_event)
{
	engine->SendEvent(new_event);
}
