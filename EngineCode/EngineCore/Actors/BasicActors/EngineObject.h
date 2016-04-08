#pragma once


#include "Common/RTTR.h"


class EngineInterface;
class Engine;
class ControllersEngine;
class Controller;
class Event;
class InputAbstractionLayer_base;
class IRenderer;
class Model3DFromFile;

/**@brief Klasa bazowa dla wszystkich obiekt�w w grze (aktor�w).
*/
class EngineObject
{
	RTTR_ENABLE()
private:
	static Engine*	engine;		///< Wska�nik na g��wny obiekt silnika.
protected:

	void event( Event* );
	/**@brief Zwraca wska�nik na interfejs silnika, kt�ry nadaje si� do u�ywania przez
	programist� gry.
	@attention Nie wolno rzutowa� obiektu na Engine.
	*/
	EngineInterface*	GetEngineInterface(){ return reinterpret_cast<EngineInterface*>(engine); }
public:
	virtual ~EngineObject() = default;
	virtual void		Init(){};

	/**@brief Funkcja ustawia wska�nik na g��wny obiekt silnika.
	@attention Wolno u�ywa� tylko klasie Engine w konstruktorze.
	@param[in] engine_ptr Wska�nik na g��wny obiekt silnika.
	*/
	static void			SetEngine( Engine* engine_ptr ) { if( !engine ) engine = engine_ptr; }

	static EngineObject*		Create()	{ return new EngineObject; }
};
