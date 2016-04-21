#pragma once


#include "Common/EngineObject.h"


class EngineInterface;
class Engine;
class ControllersEngine;
class Controller;
class Event;
class InputAbstractionLayerBase;
class IRenderer;
class Model3DFromFile;



/**@brief Klasa bazowa dla wszystkich obiektów w grze (aktorów).
*/
class ActorBase	:	public EngineObject
{
	RTTR_ENABLE( EngineObject )
private:
	static Engine*	engine;		///< WskaŸnik na g³ówny obiekt silnika.
protected:

	void event( Event* );
	/**@brief Zwraca wskaŸnik na interfejs silnika, który nadaje siê do u¿ywania przez
	programistê gry.
	@attention Nie wolno rzutowaæ obiektu na Engine.
	*/
	EngineInterface*	GetEngineInterface(){ return reinterpret_cast<EngineInterface*>(engine); }
public:
	virtual ~ActorBase() = default;
	virtual void		Init(){};

	/**@brief Funkcja ustawia wskaŸnik na g³ówny obiekt silnika.
	@attention Wolno u¿ywaæ tylko klasie Engine w konstruktorze.
	@param[in] engine_ptr WskaŸnik na g³ówny obiekt silnika.
	*/
	static void			SetEngine( Engine* engine_ptr ) { if( !engine ) engine = engine_ptr; }

	static ActorBase*		Create()	{ return new ActorBase; }
};
