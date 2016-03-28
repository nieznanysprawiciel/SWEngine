#pragma once

#include "RTTR.h"


class IEnableProperty;

typedef RTTR::TypeInfo TypeInfo;


class IMetaProperty
{
private:
	
	const char*		m_name;		///< Nazwa w�a�ciwo�ci, po kt�rej mo�na si� do niej odwo�ywa�.
	TypeInfo		m_type;		///< Typ w�a�ciwo�ci identyfikowany wska�nikiem na struktur� typeinfo. (mo�e si� zmieni� w przysz�o�ci)

	bool			m_serializable;
	bool			m_showInEditor;

public:
	IMetaProperty( const char* name, TypeInfo type )
		:	m_name( name )
		,	m_type( type )
	{}

	virtual ~IMetaProperty() {}


	const char*		GetPropertyName()		{ return m_name; }
	TypeInfo		GetPropertyType()		{ return m_type; }
};


template< typename PropertyType >
class MetaProperty	:	public IMetaProperty
{
private:

	PropertyType IEnableProperty::*		m_memberPtr;	

public:
	MetaProperty( const char* name, TypeInfo type, PropertyType IEnableProperty::* memberPtr )
		:	IMetaProperty( name, type )
		,	m_memberPtr( memberPtr )
	{}


	PropertyType IEnableProperty::*	GetPtr()	{ return m_memberPtr; }
};


