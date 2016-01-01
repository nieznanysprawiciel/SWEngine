#include "Common/Serialization/Deserializer.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include <fstream>
#include <stack>


struct DeserializerImpl
{

};


IDeserializer::IDeserializer()
{ }

IDeserializer::~IDeserializer()
{ }




/**@brief Pobiera parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
std::string& IDeserializer::GetValue( const std::string& name, std::string& defaultValue )
{
	//rapidjson::Value& currentObject = impl->valuesStack.top();	// Obiekt, do którego przyczepiamy atrybut.
	//rapidjson::Value newObject;
	//rapidjson::Value valueName;
	//valueName.SetString( name.c_str(), (rapidjson::SizeType)name.length(), impl->root.GetAllocator() );
	//newObject.SetString( defaultValue.c_str(), (rapidjson::SizeType)defaultValue.length(), impl->root.GetAllocator() );
	//currentObject.AddMember( std::move( valueName ), std::move( newObject ), impl->root.GetAllocator() );
	return defaultValue;
}


/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint32 IDeserializer::GetValue( const std::string& name, uint32 defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetUint( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint64 IDeserializer::GetValue( const std::string& name, uint64 defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetUint64( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int32 IDeserializer::GetValue( const std::string& name, int32 defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetInt( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int64 IDeserializer::GetValue( const std::string& name, int64 defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetInt64( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
bool IDeserializer::GetValue( const std::string& name, bool defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetBool( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
double IDeserializer::GetValue( const std::string& name, double defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetDouble( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

