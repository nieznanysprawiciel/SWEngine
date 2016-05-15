#pragma once
/**@file BaseInputController.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Klasa bazowa dla kontroller�w obs�uguj�cych wej�cie od u�ytkownika.*/


#include "EngineCore/ControllersEngine/BaseClasses/IController.h"


class InputAbstractionLayerBase;


/**@brief Klasa bazowa dla wszystkich kontroler�w do sterowania przez u�ytkownika.*/
class BaseInputController : public IController
{
	RTTR_ENABLE( IController )
protected:
	InputAbstractionLayerBase*		m_abstractionLayer;

public:
	explicit BaseInputController		( InputAbstractionLayerBase* layer );
	virtual ~BaseInputController		();

	void				SetAbstractionLayer	( InputAbstractionLayerBase* layer ) { m_abstractionLayer = layer; };
};
