#include "Common/Serialization/Deserializer.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#ifdef _WIN32
	#define _CRT_SECURE_NO_DEPRECATE
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <fstream>
#include <stack>

#include "Common/memory_leaks.h"
#include "ErrorCodes.h"

struct DeserializerImpl
{
	rapidjson::Document				root;
	std::stack<rapidjson::Value*>	valuesStack;
	char*							fileContent;

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


bool			IDeserializer::LoadFromFile		( const std::string& fileName, ParsingMode mode )
{
	std::ifstream file( fileName, std::ios::binary | std::ios::ate );

	if( file.fail() )
		return false;
	
	// Szukamy koñca pliku, a potem wracamy na pocz¹tek.
	std::streambuf* rawBuffer = file.rdbuf();
	unsigned int fileSize = rawBuffer->pubseekoff( 0, file.end );
	file.seekg ( 0, file.beg );


	// Alokujemy bufor odpowiedniej d³ugoœci
	impl->fileContent = new char[ fileSize + 1 ];

	// Wczytujemy plik do bufora
	auto result = rawBuffer->sgetn( impl->fileContent, fileSize );
	file.close();

	// Dodajemy znak koñca stringa na koñcu pliku
	impl->fileContent[ result ] = '\0';

	// Parsujemy w zale¿noœci od wybranego trybu
	if( mode == ParsingMode::ParseInsitu )
		impl->root.ParseInsitu( impl->fileContent );
	else // ParsingMode::AllocStrings
	{
		// W tym trybie wszystkie stringi s¹ alokowane.
		// Dlatego po wykonaniu tej operacji kasujemy bufor.
		impl->root.Parse( impl->fileContent );
		delete[] impl->fileContent;
		impl->fileContent = nullptr;
	}
	
	if( impl->root.HasParseError() )
		return false;


	impl->valuesStack.push( &impl->root );
	return true;
}

bool			IDeserializer::LoadFromString	( const std::string& contentString )
{
	return false;
}

/**@brief Wyszukuje obiekt o podanej nazwie i wchodzi w g³¹b drzewa.

@param[in] name Nazwa obiektu.
@return Zwraca false, je¿eli obiekt o danej nazwie nie istnieje.*/
bool			IDeserializer::EnterObject		( const std::string& name )
{
	auto value = impl->valuesStack.top();

	auto iterator = value->FindMember( name.c_str() );
	if( iterator == value->MemberEnd() )
		return false;

	assert( iterator->value.IsObject() );

	impl->valuesStack.push( &iterator->value );
	return true;
}

/**@brief Wyszukuje tablicê o podanej nazwie i wchodzi w g³¹b drzewa.

@param[in] name Nazwa tablicy.
@return Zwraca false, je¿eli tablica o danej nazwie nie istnieje.*/
bool			IDeserializer::EnterArray		( const std::string& name )
{
	auto value = impl->valuesStack.top();

	auto iterator = value->FindMember( name.c_str() );
	if( iterator == value->MemberEnd() )
		return false;

	assert( iterator->value.IsArray() );

	impl->valuesStack.push( &iterator->value );
	return true;
}

/**@brief */
void			IDeserializer::Exit			()
{
	auto value = impl->valuesStack.top();
	if( value->IsObject() )
		impl->valuesStack.pop();
	else if( value->IsArray() )
	{
		impl->valuesStack.pop();
		// Coœ jeszcze
	}
	else
		assert( false );
}

/**@brief */
bool IDeserializer::NextElement()
{
	return false;
}

/**@brief */
bool IDeserializer::PrevElement()
{
	return false;
}

/**@brief Pobiera parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
std::string IDeserializer::GetAttribute( const std::string& name, std::string& defaultValue )
{
	rapidjson::Value* currentObject = impl->valuesStack.top();	// Obiekt, w którym szukamy atrybutów

	auto iterator = currentObject->FindMember( name );
	if( iterator == currentObject->MemberEnd() || !iterator->value.IsString() )
		return defaultValue;

	return iterator->value.GetString();
}

/**@brief Zwraca atrybut o podanej nazwie.

@attention Zwracany string przestanie istnieæ w momencie usuniêcia serializatora lub zmiany zawartoœci.

@param[in] name Nazwa atrybutu
@param[in] defaultValue Je¿eli element o podanej nazwie nie istnieje, zostanie zwrócona wartoœæ domyœlna.*/
const char* IDeserializer::GetAttribute( const std::string& name, const char* defaultValue )
{
	rapidjson::Value* currentObject = impl->valuesStack.top();	// Obiekt, w którym szukamy atrybutów

	auto iterator = currentObject->FindMember( name );
	if( iterator == currentObject->MemberEnd() || !iterator->value.IsString() )
		return defaultValue;

	return iterator->value.GetString();
}


/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint32 IDeserializer::GetAttribute( const std::string& name, uint32 defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetUint( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint64 IDeserializer::GetAttribute( const std::string& name, uint64 defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetUint64( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int32 IDeserializer::GetAttribute( const std::string& name, int32 defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetInt( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int64 IDeserializer::GetAttribute( const std::string& name, int64 defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetInt64( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
bool IDeserializer::GetAttribute( const std::string& name, bool defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetBool( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
double IDeserializer::GetAttribute( const std::string& name, double defaultValue )
{
	//rapidjson::Value newObject;
	//newObject.SetDouble( defaultValue );
	//SetValueHelper( impl, name, newObject );
	return defaultValue;
}

std::string IDeserializer::GetError()
{
	rapidjson::ParseErrorCode code = impl->root.GetParseError();
	auto lineNum = impl->root.GetErrorOffset();
	
	std::string errorMessage = "Error: " + std::string( GetStringFromCode( code ) ) + " Offset: " + std::to_string( lineNum );
	return std::move( errorMessage );
}

