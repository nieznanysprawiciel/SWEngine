#pragma once

#include <string>




class Converters
{
private:
	template< typename EnumType >
	static const char*			EnumToConstChar		( std::pair< EnumType, const char* >* mappingArray, EnumType defaultVal );

	template< typename EnumType >
	static std::string			EnumToString		( std::pair< EnumType, const char* >* mappingArray, EnumType defaultVal );

	template< typename EnumType >
	static EnumType				StringToEnum		( std::pair< EnumType, const char* >* mappingArray, const std::string& value, EnumType defaultValue );

	template< typename EnumType >
	static std::pair< EnumType, const char* >*		GetEnumMappingArray();

public:

	template< typename EnumType >
	static const char*			ToConstChar		( EnumType value);

	template< typename EnumType >
	static std::string			ToString		( EnumType value );

	template< typename EnumType >
	static EnumType				FromString		( const std::string& value, EnumType defaultValue );






};


//====================================================================================//
//			Public	
//====================================================================================//

/**@brief Konwertuje enuma do ³añcucha znaków.*/
template<typename EnumType>
inline const char*			Converters::ToConstChar		( EnumType value )
{
	return EnumToConstChar( GetEnumMappingArray< EnumType >(), value );
}

/**@brief Konwertuje enuma do stringa.*/
template<typename EnumType>
inline std::string			Converters::ToString		( EnumType value )
{
	return EnumToString( GetEnumMappingArray< EnumType >(), value );
}

/**@brief Konwertuje stringa do enuma.*/
template<typename EnumType>
inline EnumType				Converters::FromString		( const std::string& value, EnumType defaultValue )
{
	return StringToEnum( GetEnumMappingArray< EnumType >(), value, defaultValue );
}

//====================================================================================//
//			Private	
//====================================================================================//


template< typename EnumType >
const char*				Converters::EnumToConstChar		( std::pair< EnumType, const char* >* mappingArray, EnumType value )
{
	int idx = 0;
	while( strcmp( mappingArray[ idx ].second, "" ) )
	{
		if( mappingArray[ idx ].first == value )
			return mappingArray[ idx ].second;
		
		idx++;
	}

	return "";
}


template< typename EnumType >
std::string				Converters::EnumToString		( std::pair< EnumType, const char* >* mappingArray, EnumType value )
{
	return std::string( EnumToConstChar( mappingArray, value ) );
}


template< typename EnumType >
EnumType				Converters::StringToEnum		( std::pair< EnumType, const char* >* mappingArray, const std::string& value, EnumType defaultValue )
{
	int idx = 0;
	while( strcmp( mappingArray[ idx ].second, "" ) )
	{
		if( mappingArray[ idx ].second == value.c_str() )
			return mappingArray[ idx ].first;

		idx++;
	}

	return defaultValue;
}


template< typename EnumType >
std::pair< EnumType, const char* >*		Converters::GetEnumMappingArray()
{
	static_assert( false, "You should implement GetEnumMappingArray template spetialization for this type." );
	return nullptr;
}


