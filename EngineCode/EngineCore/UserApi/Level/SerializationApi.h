#pragma once
/**@file SerializationApi.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Deklaracja klasy SerializationApi.*/


#include "EngineCore/UserApi/ApiGroup.h"
#include "Common/EngineObject.h"

class IDeserializer;



namespace Api
{

/**@defgroup SerializationApi Serializacja levelu
@ingroup UserAPI
@brief Funkcje pomocnicze do serializacji.*/

	/**@brief Zawiera funkcje pomocnicze do serializacji.
	U�ytkownik raczej nie powinien korzysta� z tych funkcji, poniewa� zapisywanie i wczytywanie
	dzieje si� automatycznie.

	@ingroup UserAPI
	@ingroup SerializationApi*/
	class SerializationApi : public ApiGroup
	{
	private:
	protected:
	public:
		SerializationApi() = default;
		~SerializationApi() = default;
	public:
	
		void		DefaultDeserialize			( IDeserializer* deser, EngineObject* object );
		bool		DeserializeSingleGeneric	( IDeserializer* deser, rttr::property prop, const EngineObject* object );
		
	};



}	// Api
