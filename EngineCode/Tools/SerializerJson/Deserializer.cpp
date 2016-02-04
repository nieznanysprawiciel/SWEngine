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

//=========================================================//
//				
//=========================================================//

/**@brief Wchodzi do pierwszego elementu tablicy lub obiektu.
@return Zwaca false, je¿eli nie ma ¿adnego obiektu w tablicy (lub obiekcie).*/
bool IDeserializer::FirstElement()
{
	auto value = impl->valuesStack.top();
	if( value->IsArray() )
	{
		rapidjson::Value::ValueIterator firstElement = value->Begin();
		assert( firstElement->IsObject() );			// Tablica zaraz za tablic¹ nie jest dopuszczalna.

		impl->valuesStack.push( firstElement );
	}
	else if( value->IsObject() )
	{
		auto firstElement = value->MemberBegin();
		impl->valuesStack.push( &firstElement->value );
	}
	else
	{
		assert( false );
		return false;
	}

	return true;
}

/**@brief Przechodzi do nastêpnego elementu w tablicy lub w obiekcie.*/
bool IDeserializer::NextElement()
{
	rapidjson::Value::ValueIterator value = impl->valuesStack.top();
	impl->valuesStack.pop();

	++value;
	if( value == impl->valuesStack.top()->End() )
		return false;

	impl->valuesStack.push( value );
	return true;
}

/**@brief */
bool IDeserializer::PrevElement()
{
	return false;
}

/**@brief Wchodzi do ostatniego elementu tablicy lub obiektu.
@return Zwaca false, je¿eli nie ma ¿adnego obiektu w tablicy (lub obiekcie).*/
bool IDeserializer::LastElement()
{
	auto value = impl->valuesStack.top();
	if( value->IsArray() )
	{
		rapidjson::Value::ValueIterator firstElement = value->End();
		firstElement--;

		assert( firstElement->IsObject() );			// Tablica zaraz za tablic¹ nie jest dopuszczalna.

		impl->valuesStack.push( firstElement );
	}
	else if( value->IsObject() )
	{
		auto firstElement = value->MemberEnd();
		firstElement--;

		impl->valuesStack.push( &firstElement->value );
	}
	else
	{
		assert( false );
		return false;
	}

	return true;
}

//=========================================================//
//			GetAttribute funkcje pomocnicze
//=========================================================//

namespace
{

template< typename ElementType >
inline bool Is					( rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{
	assert( false );
	return false;
}

template<>
inline bool Is< const char* >	(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.IsString();	}

template<>
inline bool Is< uint32 >	(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.IsUint();	}

template<>
inline bool Is< int32 >		(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.IsInt();	}

template<>
inline bool Is< uint64 >	(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.IsUint64();	}

template<>
inline bool Is< int64 >		(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.IsInt64();	}

template<>
inline bool Is< bool >		(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.IsBool();	}

template<>
inline bool Is< double >	(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.IsDouble();	}

//====
//====
template< typename ElementType >
inline ElementType Get					( rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{
	assert( false );
	return false;
}

template<>
inline const char* Get< const char* >	(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.GetString();	}

template<>
inline uint32 Get< uint32 >	(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.GetUint();	}

template<>
inline int32 Get< int32 >		(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.GetInt();	}

template<>
inline uint64 Get< uint64 >	(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.GetUint64();	}

template<>
inline int64 Get< int64 >		(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.GetInt64();	}

template<>
inline bool Get< bool >		(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.GetBool();	}

template<>
inline double Get< double >	(  rapidjson::GenericMemberIterator<false, rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<> >& iterator )
{	return iterator->value.GetDouble();	}



template<typename Type>
inline Type		GetAttribTemplate( DeserializerImpl* impl, const char* name, Type& defaultValue )
{
	rapidjson::Value* currentObject = impl->valuesStack.top();	// Obiekt, w którym szukamy atrybutów

	auto iterator = currentObject->FindMember( name );
	if( iterator == currentObject->MemberEnd() || !Is< Type >( iterator ) )
		return defaultValue;

	return Get< Type >( iterator );
}

}	// anonymous


//=========================================================//
//			GetAttribute string
//=========================================================//


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
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}


/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint32 IDeserializer::GetAttribute( const std::string& name, uint32 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint64 IDeserializer::GetAttribute( const std::string& name, uint64 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int32 IDeserializer::GetAttribute( const std::string& name, int32 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int64 IDeserializer::GetAttribute( const std::string& name, int64 defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
bool IDeserializer::GetAttribute( const std::string& name, bool defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
double IDeserializer::GetAttribute( const std::string& name, double defaultValue )
{
	return GetAttribTemplate( impl, name.c_str(), defaultValue );
}


//=========================================================//
//			GetAttribute const char
//=========================================================//

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
std::string		IDeserializer::GetAttribute		( const char* name, std::string& defaultValue )
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
const char*		IDeserializer::GetAttribute		( const char* name, const char* defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint32			IDeserializer::GetAttribute		( const char* name, uint32 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
uint64			IDeserializer::GetAttribute		( const char* name, uint64 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int32			IDeserializer::GetAttribute		( const char* name, int32 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
int64			IDeserializer::GetAttribute		( const char* name, int64 defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
bool			IDeserializer::GetAttribute		( const char* name, bool defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

/**@brief Ustawia parê ( nazwa, wartoœæ ) w aktualnym obiekcie.

@param[in] name Nazwa ustawianej zmiennej.
@param[in] defaultValue Wartoœæ, jaka zostanie wpisana do podanej zmiennej.*/
double			IDeserializer::GetAttribute		( const char* name, double defaultValue )
{
	return GetAttribTemplate( impl, name, defaultValue );
}

//====================================================================================//
//			Obs³uga b³êdów	
//====================================================================================//

/**@brief Zwraca string zawieraj¹cy b³¹d parsowania, je¿eli by³.

Aby siê dowiedzieæ czy parsowanie powiod³o siê, sprawdŸ wartoœæ zwracan¹ przez
funkcje @ref LoadFromString lub @ref LoadFromFile.*/
std::string IDeserializer::GetError()
{
	rapidjson::ParseErrorCode code = impl->root.GetParseError();
	auto lineNum = impl->root.GetErrorOffset();
	
	std::string errorMessage = "Error: " + std::string( GetStringFromCode( code ) ) + " Offset: " + std::to_string( lineNum );
	return std::move( errorMessage );
}

