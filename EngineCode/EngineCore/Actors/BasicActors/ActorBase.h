#pragma once


#include "swCommonLib/Common/EngineObject.h"


/**@defgroup Actors Actors
@ingroup EngineCore
@brief Information about engine actors.*/


class EngineInterface;
class Engine;
class ControllersEngine;
class IController;
class Event;
class InputAbstractionLayerBase;
class IRenderer;
class Model3DFromFile;



/**@brief Base class for all engine actors.

@ingroup Actors*/
class ActorBase	:	public EngineObject
{
	RTTR_ENABLE( EngineObject )
private:
	static Engine*	engine;		///< Wska�nik na g��wny obiekt silnika.
protected:

	void event( Event* );

public:
	virtual ~ActorBase() = default;
	virtual void		Init(){};

	/**@brief Zwraca wska�nik na interfejs silnika, kt�ry nadaje si� do u�ywania przez
	programist� gry.
	@attention Nie wolno rzutowa� obiektu na Engine.
	*/
	static EngineInterface*			GetEngineInterface()	{ return reinterpret_cast< EngineInterface* >( engine ); }

	/**@brief Funkcja ustawia wska�nik na g��wny obiekt silnika.
	@attention Wolno u�ywa� tylko klasie Engine w konstruktorze.
	@param[in] engine_ptr Wska�nik na g��wny obiekt silnika.
	*/
	static void			SetEngine		( Engine* enginePtr )	{ if( !engine ) engine = enginePtr; }

	static ActorBase*	Create			()						{ return new ActorBase; }

	virtual void		Serialize		( ISerializer* ser ) const override;
	virtual void		Deserialize		( IDeserializer* deser ) override;
};
