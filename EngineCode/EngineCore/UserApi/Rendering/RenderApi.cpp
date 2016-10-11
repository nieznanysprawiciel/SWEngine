#include "EngineCore/stdafx.h"
#include "RenderApi.h"

#include "EngineCore/MainEngine/EngineContext.h"



namespace Api
{

/**@brief Dorzuca do kolejki renderingu nowy przebieg, który zostanie wyrenderowany tylko raz.

Funkcja jest przydatna, je¿eli chcemy wyrenderowaæ teksturê, która bêdzie niezmienna przez ca³¹ grê.

Po wyrenderowaniu wysy³any jest event RenderOnceEndedEvent. Nie ma gwarancji, ¿e renderowanie odbêdzie
siê w tej samej klatce, w której zosta³o wys³ane ¿¹danie. Je¿eli pêtla renderuj¹ca bêdzie za bardzo obci¹¿ona,
to z kolejki bêdzie wybieranych tylko kilka ¿¹dañ, a pozosta³e zostan¹ od³o¿one do kolejnych klatek.

@attention W obecnej wersji event musi zostac odebrany, ¿eby zwolniæ obiekt RenderPassDepracated i wszystkie zasoby,
które zostan¹ w nim umieszczone. W przysz³oœci trzeba zrobiæ jakiœ m¹drzejszy mechanizm.

@param[in] pass Dane przebiegu uzywane przy renderowaniu.*/
void			RenderingApi::RenderOnce		( RenderPassDepracated* pass )
{
	Context->displayEngine->RenderOnce( pass );
}

/**@copydoc DisplayEngine::GetCurrentCamera*/
CameraActor*	RenderingApi::GetCurrentCamera	()
{
	return Context->displayEngine->GetCurrentCamera();
}

/**@copydoc DisplayEngine::SetCurrentCamera*/
void			RenderingApi::SetCurrentCamera	( CameraActor* camera )
{
	Context->displayEngine->SetCurrentCamera( camera );
}

/**@brief Zwraca szerokoœæ g³ównego render targetu.*/
int				RenderingApi::GetWindowWidth()
{
	return Context->windowWidth;
}

/**@brief Zwraca wysokoœæ g³ównego render targetu.*/
int				RenderingApi::GetWindowHeight()
{
	return Context->windowHeight;
}



}	// Api
