#include "EngineCore/stdafx.h"
#include "RenderApi.h"

#include "EngineCore/MainEngine/EngineContext.h"



namespace Api
{

/**@brief Dorzuca do kolejki renderingu nowy przebieg, kt�ry zostanie wyrenderowany tylko raz.

Funkcja jest przydatna, je�eli chcemy wyrenderowa� tekstur�, kt�ra b�dzie niezmienna przez ca�� gr�.

Po wyrenderowaniu wysy�any jest event RenderOnceEndedEvent. Nie ma gwarancji, �e renderowanie odb�dzie
si� w tej samej klatce, w kt�rej zosta�o wys�ane ��danie. Je�eli p�tla renderuj�ca b�dzie za bardzo obci��ona,
to z kolejki b�dzie wybieranych tylko kilka ��da�, a pozosta�e zostan� od�o�one do kolejnych klatek.

@attention W obecnej wersji event musi zostac odebrany, �eby zwolni� obiekt RenderPassDepracated i wszystkie zasoby,
kt�re zostan� w nim umieszczone. W przysz�o�ci trzeba zrobi� jaki� m�drzejszy mechanizm.

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

/**@brief Zwraca szeroko�� g��wnego render targetu.*/
int				RenderingApi::GetWindowWidth()
{
	return Context->windowWidth;
}

/**@brief Zwraca wysoko�� g��wnego render targetu.*/
int				RenderingApi::GetWindowHeight()
{
	return Context->windowHeight;
}



}	// Api
