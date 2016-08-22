#pragma once

#include <type_traits>


class ResourceObject;


/**@brief Wrapper na wskaŸnik na zasoby silnika (assety i resourcy).
Automatycznie inkrementuje i dekrementuje liczbê referencji.

@ingroup Resources*/
template< typename ResourceType >
class ResourcePtr
{
private:
	ResourceType*		m_resource;

public:
	ResourcePtr()
	{
		static_assert( std::is_base_of< ResourceObject, ResourceType >::value, "Template parameter type must inherit from ResourceObject" );
		m_resource = nullptr;
	}

	ResourcePtr( ResourceType* ptr )
	{
		static_assert( std::is_base_of< ResourceObject, ResourceType >::value, "Template parameter type must inherit from ResourceObject" );
		m_resource = ptr;
	}

	~ResourcePtr()
	{
		ReleaseResource();
	}

	ResourcePtr( const ResourcePtr& other )
	{
		m_resource = other.m_resource;
		m_resource->AddObjectReference();
	}

	ResourcePtr( ResourcePtr&& other )
	{
		if( this != &other)
		{
			m_resource = other.m_resource;
			other.m_resource = nullptr;
		}
	}

	void operator=( ResourceType* ptr )
	{
		ReleaseResource();

		m_resource = ptr;
		m_resource->AddObjectReference();
	}

	void operator=( const ResourcePtr& ptr )
	{
		this = ptr.m_resource;	
	}

	ResourceType* operator*()
	{
		return m_resource;
	}

	ResourceType* operator->()
	{
		return m_resource;
	}

	const ResourceType* operator*() const
	{
		return m_resource;
	}

	const ResourceType* operator->() const
	{
		return m_resource;
	}

	void ReleaseResource()
	{
		if( m_resource )
			m_resource->DeleteObjectReference();
	}

	ResourceType*	Ptr	() const
	{ return m_resource; }

};

