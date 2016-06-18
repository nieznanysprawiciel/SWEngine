#pragma once
/**@file ActorBase.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief */

#include "Common/RTTR.h"
#include "Common/TypesDefinitions.h"

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

	/// Zwraca ilo�� pami�ci zajmowanej przez obiekt. Implementacje w klasach potomnych powinny te�
	/// dolicza� rekurencyjnie pami�� zajmowan� przez obiekty, kt�rych w�a�cicielem jest ten obiekt.
	virtual Size		MemorySize		()							{ return this->get_type().get_sizeof(); }
};
