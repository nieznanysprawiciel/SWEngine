#include "Common/Serialization/Serializer.h"

#include "RapidXML/rapidxml.hpp"
#include "RapidXML/rapidxml_print.hpp"

#include <fstream>
#include <stack>

struct SerializerImpl
{
	rapidxml::xml_document<>			root;
	std::stack< rapidxml::xml_node<>* >	valuesStack;
};


/**@brief Konstruktor*/
ISerializer::ISerializer()
{
	impl = new SerializerImpl;
	impl->valuesStack.push( &impl->root );
}

/**@brief Destruktor*/
ISerializer::~ISerializer()
{ delete impl; }



/**@brief Zwraca stringa zawieraj¹cego zserializowanego Jsona.

@attention Po wykonaniu funkcji serializator wraca na zerowy poziom
zagnie¿d¿enia node'ów.

@param[in] mode Formatowanie stringa.*/
std::string	ISerializer::SaveString( WritingMode mode )
{
	int printingFlag = 0;
	if( mode == WritingMode::Sparing )
		printingFlag = rapidxml::print_no_indenting;

	while( impl->valuesStack.size() > 1 )
		this->Exit();

	std::string xmlString;
	rapidxml::print( std::back_inserter( xmlString ), impl->root, printingFlag );
	return std::move( xmlString );
}


/**@brief Zapisuje zserializowane dane do pliku.

@attention Po wykonaniu funkcji serializator wraca na zerowy poziom
zagnie¿d¿enia node'ów.

@param[in] fileName Nazwa pliku docelowego.
@return Zwraca true, je¿eli zapisywanie powiedzie siê.*/
bool ISerializer::SaveFile( const std::string& fileName, WritingMode mode )
{
	std::ofstream file;
	file.open( fileName );
	if( !file.fail() )
	{
		file << SaveString( mode );

		file.close();
		return true;
	}
	return false;
}

/**@brief Tworzy obiekt o podanej nazwie.

@param[in] name Nazwa obiektu.*/
void ISerializer::EnterObject( const std::string& name )
{
	char* nodeName = impl->root.allocate_string( name.c_str() );
	rapidxml::xml_node<>* node = impl->root.allocate_node( rapidxml::node_type::node_element, nodeName );
	impl->valuesStack.push( node );
}

/**@brief Koniec tworzenia obiektu.
EnterObject i ExitObject powinny byæ stosowane parami.*/
void ISerializer::Exit()
{
	rapidxml::xml_node<>* attachNode = impl->valuesStack.top();		// Obiekt, którego konstruowanie skonczyliœmy.
	impl->valuesStack.pop();
	rapidxml::xml_node<>* attachTo = impl->valuesStack.top();		// Nazwa obiektu.

	attachTo->append_node( attachNode );
}

/**@brief Tworzy tablicê o podanej nazwie.

@param[in] name Nazwa tablicy.*/
void ISerializer::EnterArray( const std::string& name )
{
	//rapidjson::Value newArray( rapidjson::kArrayType );

	//// Nazwa jest potrzebna tylko, je¿eli przyczepiamy siê do obiektu.
	//// W przypadku tablic wszystkie obiekty s¹ bez nazwy, wiêc unikamy alokacji pamiêci i wrzucamy pustego stringa.
	//rapidjson::Value newName( rapidjson::kStringType );
	//if( impl->valuesStack.top().IsObject() )
	//	newName.SetString( name.c_str(), (rapidjson::SizeType)name.length(), impl->root.GetAllocator() );

	//// Wrzucamy nazwê jako pierwsz¹, a potem sam obiekt.
	//impl->valuesStack.push( std::move( newName ) );
	//impl->valuesStack.push( std::move( newArray ) );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, const std::string& value )
{
	Size attribNameSize = name.length();
	Size attribValueSize = value.length();

	char* attribName = impl->root.allocate_string( name.c_str(), attribNameSize );
	char* attribValue = impl->root.allocate_string( value.c_str(), attribValueSize );

	rapidxml::xml_attribute<>* attribute = impl->root.allocate_attribute( attribName, attribValue, attribNameSize, attribValueSize );
	auto currentNode = impl->valuesStack.top();
	currentNode->append_attribute( attribute );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, const char* value )
{
	Size attribNameSize = name.length();
	Size attribValueSize = strlen( value );

	char* attribName = impl->root.allocate_string( name.c_str(), attribNameSize );
	char* attribValue = impl->root.allocate_string( value, attribValueSize );

	rapidxml::xml_attribute<>* attribute = impl->root.allocate_attribute( attribName, attribValue, attribNameSize, attribValueSize );
	auto currentNode = impl->valuesStack.top();
	currentNode->append_attribute( attribute );
}


/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, uint32 value )
{
	//rapidjson::Value newObject;
	//newObject.SetUint( value );
	//SetValueHelper( impl, name, newObject );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, uint64 value )
{
	//rapidjson::Value newObject;
	//newObject.SetUint64( value );
	//SetValueHelper( impl, name, newObject );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, int32 value )
{
	//rapidjson::Value newObject;
	//newObject.SetInt( value );
	//SetValueHelper( impl, name, newObject );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, int64 value )
{
	//rapidjson::Value newObject;
	//newObject.SetInt64( value );
	//SetValueHelper( impl, name, newObject );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, bool value )
{
	//rapidjson::Value newObject;
	//newObject.SetBool( value );
	//SetValueHelper( impl, name, newObject );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] value Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
void ISerializer::SetValue( const std::string& name, double value )
{
	//rapidjson::Value newObject;
	//newObject.SetDouble( value );
	//SetValueHelper( impl, name, newObject );
}

