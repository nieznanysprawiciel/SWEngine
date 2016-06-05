#pragma once
/**@file ActorBase.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief */

#include "Common/RTTR.h"

class ISerializer;
class IDeserializer;


/**@brief Klasa bazowa dla obiekt�w, kt�re maj� by� dost�pne w edytorze
poprzez rttr. Nale�y po niej odziedziczy� i doda� odpowiednie deklaracje.

@todo Dobrze by by�o, gdyby EngineObject implementowa� domy�ln� serializacj�, ale wtedy jest problem,
�e nie wiadomo, kt�ra z bibliotek powinna t� implementacj� kompilowa�.

@ingroup Reflection*/
class EngineObject
{
	RTTR_ENABLE()

public:
	inline rttr::type	GetType			() const		{ return this->get_type(); }


	virtual void		Serialize		( ISerializer* ser ) const	{}
	virtual void		Deserialize		( IDeserializer* deser )	{}
};
