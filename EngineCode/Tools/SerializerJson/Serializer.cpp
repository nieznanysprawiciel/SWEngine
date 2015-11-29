#include "Serializer.h"


#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

#include <stack>

struct SerializerImpl
{
	rapidjson::Document				root;
	std::stack<rapidjson::Value>	valuesStack;
};


/**@brief Konstruktor*/
ISerializer::ISerializer()
{
	impl = new SerializerImpl;
	rapidjson::Value newObject( rapidjson::kObjectType );
	impl->valuesStack.push( std::move( newObject ) );
}

/**@brief Destruktor*/
ISerializer::~ISerializer()
{ delete impl; }

/**@brief Tworzy obiekt o podanej nazwie.

@param[in] name Nazwa obiektu.*/
void ISerializer::EnterObject( const std::string& name )
{
	rapidjson::Value newObject( rapidjson::kObjectType );
	rapidjson::Value newName( rapidjson::kStringType );
	newName.SetString( name.c_str(), (rapidjson::SizeType)name.length(), impl->root.GetAllocator() );

	// Wrzucamy nazwê jako pierwsz¹, a potem sam obiekt.
	impl->valuesStack.push( std::move( newName ) );
	impl->valuesStack.push( std::move( newObject ) );
}

/**@brief Koniec tworzenia obiektu.
EnterObject i ExitObject powinny byæ stosowane parami.*/
void ISerializer::ExitObject()
{
	rapidjson::Value& objectValue = impl->valuesStack.top();		// Obiekt, które go konstruowanie skonczyliœmy.
	impl->valuesStack.pop();
	rapidjson::Value& objectName = impl->valuesStack.top();			// Nazwa obiektu.
	impl->valuesStack.pop();

	rapidjson::Value& newCurrentObject = impl->valuesStack.top();
	newCurrentObject.AddMember( std::move( objectName ), std::move( objectValue ), impl->root.GetAllocator() );
}

/**@brief Tworzy tablicê o podanej nazwie.

@param[in] name Nazwa tablicy.*/
void ISerializer::EnterArray( const std::string& name )
{ }

/**@brief Koniec tworzenia tablicy.
Funkcja parzysta do EnterArray.*/
void ISerializer::ExitArray()
{ }

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, const std::string& value )
{
	rapidjson::Value& currentObject = impl->valuesStack.top();	// Obiekt, do którego przyczepiamy atrybut.
	rapidjson::Value newObject;
	rapidjson::Value valueName;
	valueName.SetString( name.c_str(), (rapidjson::SizeType)name.length(), impl->root.GetAllocator() );
	newObject.SetString( value.c_str(), (rapidjson::SizeType)value.length(), impl->root.GetAllocator() );
	currentObject.AddMember( std::move( valueName ), std::move( newObject ), impl->root.GetAllocator() );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, uint32 value )
{ }

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, uint64 value )
{ }

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, int32 value )
{ }

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, int64 value )
{ }

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, bool value )
{ }

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, double value )
{ }


