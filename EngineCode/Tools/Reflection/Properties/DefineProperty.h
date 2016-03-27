#pragma once

#include "Common/RTTR.h"
#include "Common/Properties/IEnableProperty.h"
#include "IMetaProperty.h"

#include <vector>

namespace RTTR
{
	template< typename PropertyType >
	RTTR::TypeInfo::TypeId RegisterProperty( const char* name, RTTR::TypeInfo::TypeId type, PropertyType IEnableProperty::* memberPtr );


	namespace impl
	{

		template< typename ClassName, const char* PropertyName >
		struct PropertyRegisterer
		{
			static RTTR::TypeInfo::TypeId GetPropertyIdx()
			{
				static RTTR::TypeInfo::TypeId idx = RegisterProperty( const char* name, )
			}
		};

	}	//	impl
}	// RTTR



//====================================================================================//
//			Prototyper	
//====================================================================================//

//struct nil {};
//
//template<class T, class U = nil> struct PropertyList
//{
//    typedef T head;
//    typedef U tail;
//};
//
//
//template<class> 
//struct TypeInfoProperties;
//
//template<>
//struct TypeInfoProperties< PropertyList< nil > > 
//{
//	static void fill( std::vector< IMetaProperty* >& ) 
//	{}
//};
//
//template< class T, class U >
//struct TypeInfoProperties< PropertyList< T, U > > 
//{
//	static void fill( std::vector< IMetaProperty* >& v )
//	{
//		//v.push_back( MetaTypeInfo<T>::getTypeInfo() );
//		//TypeInfoFromBaseClassList<typename T::baseClassList>::fill(v);
//		//TypeInfoFromBaseClassList<U>::fill(v);
//	}
//};


//====================================================================================//
//			Prototyper - class to test properties	
//====================================================================================//


//template< typename ClassType >
//class ClassForProperties
//{
//private:
//	// extract the info
//	static void retrieve_impl( std::vector< IMetaProperty* >& v, Traits::true_type )
//	{
//		//TypeInfoFromBaseClassList<typename T::baseClassList>::fill(v);
//	}
//
//	// no type list defined
//	static void retrieve_impl(std::vector< IMetaProperty* >& v, Traits::false_type)
//	{}
//
//public:
//	static std::vector< IMetaProperty* > retrieve()
//	{
//		std::vector< IMetaProperty* > result;
//		// check with SFINAE whether a typedef for baseClassList is defined or not
//		//retrieve_impl( result, typename has_base_class_list<T>::type() );
//		return result;
//	}
//};


/*template< typename ClassName, const char* PropertyName >
struct TypeInfoPropertyTemplate
{
	static IMetaProperty*	CreateProperty()	{}
};

#define Property( classType, name, member )																													\
	template<>																																				\
	struct TypeInfoPropertyTemplate< classType, name >																										\
	{																																						\
		static IMetaProperty*	CreateProperty()																											\
		{																																					\
			static IMetaProperty* newProperty = new MetaProperty< decltype( classType::member ) >( name, RTTR::TypeInfo::get< classType::member >(), &classType::member );\
			return newProperty;																																\
		}																																					\
																																							\
	};			*/																																			\


template< typename ClassType >
class class_
{
public:
	/*!
		* \brief Construct a class_ object with the given name \p name.
		*
		* \param name The name of the class as string literal. Can be retrieved later via type::get_name().
		*
		*/
	class_( const char* name );
	~class_();


	/*!
		* \brief The bracket operator can be used to add additional meta data to the class type.
		*
		*/
	template<typename...Args>
	class_< ClassType >& operator()( Args&&...args );
}


class ClassWithProperties
{
public:
	int		value;

private:
	//Property( ClassWithProperties, "Value", value )
};
