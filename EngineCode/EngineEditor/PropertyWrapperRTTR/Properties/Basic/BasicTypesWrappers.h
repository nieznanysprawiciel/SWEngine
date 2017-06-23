#pragma once
/**
@file BasicTypesWrappers.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineEditor/PropertyWrapperRTTR/Properties/Base/PropertyWrapper.h"



namespace sw {
namespace EditorPlugin
{


//====================================================================================//
//				IntPropertyWrapper
//====================================================================================//

/**@brief Property typy in.*/
public ref class IntPropertyWrapper : PropertyWrapper
{
public:
	IntPropertyWrapper( PropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyInt, prop, prop.get_name().to_string().c_str() )
	{}


	property int		Value
	{
		int			get ()
		{
			if( m_actorPtr )
				return GetValue( m_actorPtr );
			return std::numeric_limits< int >::max();
		}

		void		set	( int value )
		{
			if( m_actorPtr )
				SetValue( m_actorPtr, value );
		}
	}

private:
	int						GetValue( void* refObject );
	void					SetValue( void* refObject, int newValue );
};


//====================================================================================//
//				UIntPropertyWrapper
//====================================================================================//


/**@brief Property typy unsinged int.*/
public ref class UIntPropertyWrapper : PropertyWrapper
{
public:
	UIntPropertyWrapper( PropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyUInt, prop, prop.get_name().to_string().c_str() )
	{}


	property uint32		Value
	{
		uint32		get ()
		{
			if( m_actorPtr )
				return GetValue( m_actorPtr );
			return std::numeric_limits< uint32 >::max();
		}

		void		set	( uint32 value )
		{
			if( m_actorPtr )
				SetValue( m_actorPtr, value );
		}
	}

private:
	uint32					GetValue( void* refObject );
	void					SetValue( void* refObject, uint32 newValue );
};


//====================================================================================//
//				BoolPropertyWrapper
//====================================================================================//



/**@brief Property typu bool.*/
public ref class BoolPropertyWrapper : PropertyWrapper
{
public:
	BoolPropertyWrapper( PropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyBool, prop, prop.get_name().to_string().c_str() )
	{}

	property bool		Value
	{
		bool		get ()
		{
			if( m_actorPtr )
				return GetValue( m_actorPtr );
			return std::numeric_limits< bool >::max();
		}

		void		set ( bool value )
		{
			if( m_actorPtr )
				SetValue( m_actorPtr, value );
		}
	}

private:
	bool		GetValue( void* refObject );
	void		SetValue( void* refObject, bool newValue );
};

//====================================================================================//
//				FloatPropertyWrapper
//====================================================================================//


/**@brief Property typu float.*/
public ref class FloatPropertyWrapper : PropertyWrapper
{
public:
	FloatPropertyWrapper( PropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyFloat, prop, prop.get_name().to_string().c_str() )
	{}

	property float		Value
	{
		float		get ()
		{
			if( m_actorPtr )
				return GetValue( m_actorPtr );
			return std::numeric_limits< float >::max();
		}

		void		set ( float value )
		{
			if( m_actorPtr )
				SetValue( m_actorPtr, value );
		}
	}

private:
	float		GetValue( void* refObject );
	void		SetValue( void* refObject, float newValue );
};


//====================================================================================//
//				DoublePropertyWrapper
//====================================================================================//


/**@brief Property typu double.*/
public ref class DoublePropertyWrapper : PropertyWrapper
{
public:
	DoublePropertyWrapper( PropertyWrapper^ parent, rttr::property prop )
		: PropertyWrapper( parent, PropertyType::PropertyDouble, prop, prop.get_name().to_string().c_str() )
	{}

public:
	property double		Value
	{
		double		get	()
		{
			if( m_actorPtr )
				return GetValue( m_actorPtr );
			return std::numeric_limits< double >::max();
		}

		void		set	( double value )
		{
			if( m_actorPtr )
				SetValue( m_actorPtr, value );
		}
	}

private:
	double		GetValue( void* refObject );
	void		SetValue( void* refObject, double newValue );
};



}
}	// sw

