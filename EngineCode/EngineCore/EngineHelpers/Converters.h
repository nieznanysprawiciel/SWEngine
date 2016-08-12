#pragma once

#include <string>
#include <codecvt>



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
	static const char*			ToConstChar		( const EnumType& value);

	template< typename EnumType >
	static std::string			ToString		( const EnumType& value );

	template< typename EnumType >
	static EnumType				FromString		( const std::string& value, EnumType defaultValue );





	template<>
	static std::string			ToString< std::wstring >		( const std::wstring& value );

	template<>
	static std::wstring			FromString< std::wstring >		( const std::string& value, std::wstring defaultValue );
};


//====================================================================================//
//			Public	
//====================================================================================//

/**@brief Konwertuje enuma do ³añcucha znaków.*/
template<typename EnumType>
inline const char*			Converters::ToConstChar		( const EnumType& value )
{
	return EnumToConstChar( GetEnumMappingArray< EnumType >(), value );
}

/**@brief Konwertuje enuma do stringa.*/
template<typename EnumType>
inline std::string			Converters::ToString		( const EnumType& value )
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
//			Wstring to string	
//====================================================================================//

template<>
inline std::string			Converters::ToString		( const std::wstring& value )
{
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes( value );
}

template<>
inline std::wstring			Converters::FromString		( const std::string& value, std::wstring defaultValue )
{
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.from_bytes( value );
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


