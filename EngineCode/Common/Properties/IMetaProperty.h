#pragma once

#include "Common/RTTR.h"


class IEnableProperty;

typedef RTTR::TypeInfo::TypeId TypeId;


class IMetaProperty
{
private:
	
	const char*		m_name;		///< Nazwa w�a�ciwo�ci, po kt�rej mo�na si� do niej odwo�ywa�.
	TypeId			m_type;		///< Typ w�a�ciwo�ci identyfikowany wska�nikiem na struktur� typeinfo. (mo�e si� zmieni� w przysz�o�ci)

	bool			m_serializable;
	bool			m_showInEditor;

public:
	IMetaProperty( const char* name, TypeId type )
		:	m_name( name )
		,	m_type( type )
	{}


	const char*		GetPropertyName()		{ return m_name; }
	TypeId			GetPropertyType()		{ return m_type; }
};


template< typename PropertyType >
class MetaProperty	:	public IMetaProperty
{
private:

	PropertyType IEnableProperty::*		m_memberPtr;	

public:
	MetaProperty( const char* name, TypeId type, PropertyType IEnableProperty::* memberPtr )
		:	IMetaProperty( name, type )
		,	m_memberPtr( memberPtr )
	{}


	PropertyType IEnableProperty::*	GetPtr()	{ return m_memberPtr; }
};


