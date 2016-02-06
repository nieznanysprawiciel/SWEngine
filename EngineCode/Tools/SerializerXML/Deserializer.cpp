#include "Common/Serialization/Deserializer.h"

#include "RapidXML/rapidxml.hpp"


#include <fstream>
#include <stack>
#include <stdlib.h>

#include "Common/memory_leaks.h"


struct DeserializerImpl
{
	rapidxml::xml_document<>				root;
	std::stack< rapidxml::xml_node<>* >		valuesStack;
	char*									fileContent;

	DeserializerImpl()
	{	fileContent = nullptr;	}
};


IDeserializer::IDeserializer()
{
	impl = new DeserializerImpl;
}

IDeserializer::~IDeserializer()
{	
	delete[] impl->fileContent;
	delete impl;
}

/**@brief Wczytuje i parsuje podany plik.

@param[in] fileName Nazwa pliku.
@param[in] mode Tryb parsowania. Parser XMLowy wspiera tylko parsowanie insitu.
@return Zwraca informacj� czy parsowanie powiod�o si�.*/
bool			IDeserializer::LoadFromFile		( const std::string& fileName, ParsingMode mode )
{
	std::ifstream file( fileName, std::ios::binary | std::ios::ate );

	if( file.fail() )
		return false;
	
	// Szukamy ko�ca pliku, a potem wracamy na pocz�tek.
	std::streambuf* rawBuffer = file.rdbuf();
	unsigned int fileSize = static_cast< unsigned int >( rawBuffer->pubseekoff( 0, file.end ) );
	file.seekg ( 0, file.beg );


	// Alokujemy bufor odpowiedniej d�ugo�ci
	impl->fileContent = new char[ fileSize + 1 ];

	// Wczytujemy plik do bufora
	auto result = rawBuffer->sgetn( impl->fileContent, fileSize );
	file.close();

	// Dodajemy znak ko�ca stringa na ko�cu pliku
	impl->fileContent[ result ] = '\0';

	// Parsujemy w zale�no�ci od wybranego trybu
	if( mode == ParsingMode::ParseInsitu )
		impl->root.parse< rapidxml::parse_default >( impl->fileContent );
	else // ParsingMode::AllocStrings
	{
		// W tym trybie wszystkie stringi s� alokowane.
		// Dlatego po wykonaniu tej operacji kasujemy bufor.
		assert( false );		// rapidXML nie obs�uguje niestety tego trybu

		impl->root.parse< rapidxml::parse_default >( impl->fileContent );
	}
	
	impl->valuesStack.push( &impl->root );
	return true;
}

/**@brief Parsuje XMLa z podanego stringa.
@param[in] contentString String do sparsowania.*/
bool			IDeserializer::LoadFromString	( const std::string& contentString )
{
	return false;
}

/**@brief Wyszukuje obiekt o podanej nazwie i wchodzi w g��b drzewa.

@param[in] name Nazwa obiektu.
@return Zwraca false, je�eli obiekt o danej nazwie nie istnieje.*/
bool			IDeserializer::EnterObject		( const std::string& name )
{
	auto value = impl->valuesStack.top();

	auto enterNode = value->first_node( name.c_str() );
	if( enterNode == nullptr )
		return false;

	//assert( enterNode->type() == rapidxml::node_type:: );

	impl->valuesStack.push( enterNode );
	return true;
}

/**@brief Wyszukuje tablic� o podanej nazwie i wchodzi w g��b drzewa.

@param[in] name Nazwa tablicy.
@return Zwraca false, je�eli tablica o danej nazwie nie istnieje.*/
bool			IDeserializer::EnterArray		( const std::string& name )
{
	//auto value = impl->valuesStack.top();

	//auto iterator = value->FindMember( name.c_str() );
	//if( iterator == value->MemberEnd() )
	//	return false;

	//assert( iterator->value.IsArray() );

	//impl->valuesStack.push( &iterator->value );
	//return true;

	return false;
}

/**@brief */
void			IDeserializer::Exit			()
{
	auto value = impl->valuesStack.top();
	impl->valuesStack.pop();
}

//=========================================================//
//				
//=========================================================//

/**@brief Wchodzi do pierwszego elementu tablicy lub obiektu.
@return Zwaca false, je�eli nie ma �adnego obiektu w tablicy (lub obiekcie).*/
bool IDeserializer::FirstElement()
{
	//auto value = impl->valuesStack.top();
	//if( value->IsArray() )
	//{
	//	rapidjson::Value::ValueIterator firstElement = value->Begin();
	//	assert( firstElement->IsObject() );			// Tablica zaraz za tablic� nie jest dopuszczalna.

	//	impl->valuesStack.push( firstElement );
	//}
	//else if( value->IsObject() )
	//{
	//	auto firstElement = value->MemberBegin();
	//	impl->valuesStack.push( &firstElement->value );
	//}
	//else
	//{
	//	assert( false );
	//	return false;
	//}

	//return true;

	return false;
}

/**@brief Przechodzi do nast�pnego elementu w tablicy lub w obiekcie.*/
bool IDeserializer::NextElement()
{
	//rapidjson::Value::ValueIterator value = impl->valuesStack.top();
	//impl->valuesStack.pop();

	//auto newValue = ++value;
	//if( newValue->IsNull() )
	//	return false;

	//impl->valuesStack.push( newValue );
	//return true;

	return false;
}

/**@brief */
bool IDeserializer::PrevElement()
{
	return false;
}

/**@brief Wchodzi do ostatniego elementu tablicy lub obiektu.
@return Zwaca false, je�eli nie ma �adnego obiektu w tablicy (lub obiekcie).*/
bool IDeserializer::LastElement()
{
	//auto value = impl->valuesStack.top();
	//if( value->IsArray() )
	//{
	//	rapidjson::Value::ValueIterator firstElement = value->End();
	//	firstElement--;

	//	assert( firstElement->IsObject() );			// Tablica zaraz za tablic� nie jest dopuszczalna.

	//	impl->valuesStack.push( firstElement );
	//}
	//else if( value->IsObject() )
	//{
	//	auto firstElement = value->MemberEnd();
	//	firstElement--;

	//	impl->valuesStack.push( &firstElement->value );
	//}
	//else
	//{
	//	assert( false );
	//	return false;
	//}

	//return true;

	return false;
}

//=========================================================//
//			GetAttribute funkcje pomocnicze
//=========================================================//

namespace
{

/**@brief Konwertuje stringa do warto�ci liczbowych. Wykorzystuje funkcj� strol
i zwraca dok�adnie takie same warto�ci. Zobacz dokumentacj� do biblioteki standardowej c++.*/
template< typename ElementType >
inline ElementType Convert							( const char* valueBegin, char** checkEndPtr )
{
	assert( false );
	return false;
}

const char* convertCharNullTerminator = "\0";

template<>
inline const char*	Convert< const char* >			( const char* valueBegin, char** checkEndPtr )
{
	// @fixme To jest straszny hack. Gdyby kto� z zewn�trz tej funkcji chcia� por�wna�
	// zwracany wska�nik checkEndPtr ze wska�nikiem, kt�ry poda�, to by si� strasznie zdziwi�.
	// Ale jako� mimo wszystko nie chc� sprawdza� d�ugo�ci tego stringa, kiedy to nie jest potrzebne.
	*checkEndPtr = const_cast<char*>( convertCharNullTerminator );
	return valueBegin;
}

template<>
inline uint32		Convert< uint32 >				( const char* valueBegin, char** checkEndPtr )
{	return strtoul( valueBegin, checkEndPtr, 10 );	}

template<>
inline int32		Convert< int32 >				( const char* valueBegin, char** checkEndPtr )
{	return strtol( valueBegin, checkEndPtr, 10 );	}

template<>
inline uint64		Convert< uint64 >				( const char* valueBegin, char** checkEndPtr )
{	return strtoull( valueBegin, checkEndPtr, 10 );	}

template<>
inline int64		Convert< int64 >				( const char* valueBegin, char** checkEndPtr )
{	return strtoll( valueBegin, checkEndPtr, 10 );	}


const char* trueString = "true";
const char* falseString = "false";

template<>
inline bool			Convert< bool >					( const char* valueBegin, char** checkEndPtr )
{
	*checkEndPtr = const_cast<char*>( valueBegin );
	const char* referenceString;
	bool returnValue;

	if( **checkEndPtr == 't' )
	{
		returnValue = true;
		referenceString = trueString;
	}
	else if( **checkEndPtr == 'f' )
	{
		returnValue = false;
		referenceString = falseString;
	}
	else
		return false;

	int i = 0;
	do
	{
		++i;
		*checkEndPtr += 1;
	} while( **checkEndPtr == *( referenceString + i ) && **checkEndPtr != '\0' );

	return returnValue;
}

template<>
inline double		Convert< double >				( const char* valueBegin, char** checkEndPtr )
{	return strtod( valueBegin, checkEndPtr );	}



template<typename Type>
inline Type		GetAttribTemplate( DeserializerImpl* impl, const char* name, Type& defaultValue )
{
	rapidxml::xml_node<>* currentNode = impl->valuesStack.top();

	rapidxml::xml_attribute<>* attribute = currentNode->first_attribute( name );
	if( attribute == nullptr )
		return defaultValue;

	const char* attribValue = attribute->value();
	char* checkEndPtr = nullptr;	// Po konwersji dostaniemy tutaj wska�nik na bajt tu� za ko�cem parsowanego elementu

	auto value = Convert<Type>( attribValue, &checkEndPtr );
	if( *checkEndPtr != '\0' )	// Je�eli przy parsowaniu stringa nie doszli�my do ko�ca, to znaczy, �e pojawi� si� b��d.
		return defaultValue;

	return value;
}

}	// anonymous


//=========================================================//
//			GetAttribute string
//=========================================================//


/**@brief Pobiera par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
std::string IDeserializer::GetAttribute( const std::string& name, std::string& defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Zwraca atrybut o podanej nazwie.

@attention Zwracany string przestanie istnie� w momencie usuni�cia serializatora lub zmiany zawarto�ci.

@param[in] name Nazwa atrybutu
@param[in] defaultValue Je�eli element o podanej nazwie nie istnieje, zostanie zwr�cona warto�� domy�lna.*/
const char* IDeserializer::GetAttribute( const std::string& name, const char* defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}


/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
uint32 IDeserializer::GetAttribute( const std::string& name, uint32 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
uint64 IDeserializer::GetAttribute( const std::string& name, uint64 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
int32 IDeserializer::GetAttribute( const std::string& name, int32 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
int64 IDeserializer::GetAttribute( const std::string& name, int64 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
bool IDeserializer::GetAttribute( const std::string& name, bool defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
double IDeserializer::GetAttribute( const std::string& name, double defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}


//=========================================================//
//			GetAttribute const char
//=========================================================//

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
std::string		IDeserializer::GetAttribute		( const char* name, std::string& defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
const char*		IDeserializer::GetAttribute		( const char* name, const char* defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
uint32			IDeserializer::GetAttribute		( const char* name, uint32 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
uint64			IDeserializer::GetAttribute		( const char* name, uint64 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
int32			IDeserializer::GetAttribute		( const char* name, int32 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
int64			IDeserializer::GetAttribute		( const char* name, int64 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
bool			IDeserializer::GetAttribute		( const char* name, bool defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia par� ( nazwa, warto�� ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Warto��, jaka zostanie wpisana do podanej zmiennej.*/
double			IDeserializer::GetAttribute		( const char* name, double defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

//====================================================================================//
//			Obs�uga b��d�w	
//====================================================================================//

/**@brief Zwraca string zawieraj�cy b��d parsowania, je�eli by�.

Aby si� dowiedzie� czy parsowanie powiod�o si�, sprawd� warto�� zwracan� przez
funkcje @ref LoadFromString lub @ref LoadFromFile.*/
std::string IDeserializer::GetError()
{
	//rapidjson::ParseErrorCode code = impl->root.GetParseError();
	//auto lineNum = impl->root.GetErrorOffset();
	//
	//std::string errorMessage = "Error: " + std::string( GetStringFromCode( code ) ) + " Offset: " + std::to_string( lineNum );
	//return std::move( errorMessage );

	return std::string();
}



