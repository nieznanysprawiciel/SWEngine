#pragma once
/**@file IController.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Klasa bazowa dla kontroler�w.*/

#include "Common/EngineObject.h"

class DynamicActor;

/**@brief Klasa bazowa dla wszystkich kontroler�w dla obiekt�w.*/
class IController : public EngineObject
{
	RTTR_ENABLE( EngineObject )
public:
	virtual ~IController() = default;

	virtual void		ControlObjectPre		( DynamicActor* ) = 0;	///< Funkcja wywo�ywana przed 
	virtual void		ControlObjectPost		( DynamicActor* ) = 0;
};


/**
@page Controllers Kontrolery

@section Contents Spis Tre�ci
- @ref mainUse
- @ref PrePostControllers

@subsection mainUse Do czego s�u�� kotrolery

Kontrolery pozwalaj� na sterowanie ruchem Aktor�w. Ka�dy aktor dziedzicz�cy po DynamicActor
mo�e mie� przypisany kontroler przy pomocy funkcji DynamicActor::SetController.
Interfejs IController posiada dwie funkcje @ref IController::ControlObjectPre i @ref IController::ControlObjectPost,
kt�re nale�y zaimplementowa�. Po dodaniu Aktora do ControllersEngine funkcje te s� wywo�ywane w ka�dej klatce.

Kontrolery mog� one dzia�a� jak sztuczna inteligencja, wtedy trzeba odziedziczy� po klasie @ref BaseAIController,
albo odbiera� wej�cie od u�ytkownika, wtedy trzeba odziedziczy� po @ref BaseInputController.

@subsection PrePostControllers Kotrolery pre i post
Istniej� kotrolery, kt�rych ruch uzale�niony jest od po�o�enia innych obiekt�w. Z racji tego nie mog� one zosta� wywo�ane przed policzeniem
przemieszcze�, bo by�yby zawsze op�nione o jedn� klatk�. Pozosta�e kotrolery s�u�� do wp�ywania na ruch obiekt�w, dlatego nie mog� by�
wywo�ane, po obliczeniu po�o�e� obiekt�w. Z tego wzgl�du kotrolery obs�ugiwane s� dwukrotnie, jedne w funkcji ControllersEngine::ProceedControllersPre
tu� przed wywo�aniem MovementEngine::ProceedMovement, a drugie tu� po tym wywo�aniu w funkcji ControllersEngine::ProceedControllersPost.

Do kontroler�w post zaliczamy kotrolery �ledz�ce jakis obiekt, do kotroler�w pre wszystkie kontrolery odpowiedzialne za AI oraz
steruj�ce ruchem na podstawie klawiatury czy myszy.

*/
