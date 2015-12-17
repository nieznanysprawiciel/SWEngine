#pragma once

#include "Common/TypesDefinitions.h"

#include <string>

struct SerializerImpl;

/**@brief Tryb Zapisywania JSONa.*/
enum class WritingMode
{
	Sparing,				///< Tryb oszczędny bez spacji i tabulacji.
	Readable				///< Tryb przeznaczony do czytania przez ludzi.
};

/**@brief Interfejs dla serializatorów.*/
class ISerializer
{
private:
	SerializerImpl*		impl;
protected:
public:
	ISerializer();
	virtual ~ISerializer();

	void		EnterObject		( const std::string& name );
	void		EnterArray		( const std::string& name );

	void		Exit			();

	void		SetValue		( const std::string& name, const std::string& value );
	void		SetValue		( const std::string& name, const char* value );
	void		SetValue		( const std::string& name, uint32 value );
	void		SetValue		( const std::string& name, uint64 value );
	void		SetValue		( const std::string& name, int32 value );
	void		SetValue		( const std::string& name, int64 value );
	void		SetValue		( const std::string& name, bool value );
	void		SetValue		( const std::string& name, double value );


	bool		SaveFile		( const std::string& fileName, WritingMode mode = WritingMode::Sparing );
	std::string	SaveString		( WritingMode mode = WritingMode::Sparing );
};


