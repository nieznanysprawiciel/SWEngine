#pragma once

#include <map>

template <class TYPE>
class ResourceContainer
{
private:
	unsigned int count;

protected:
	std::map<std::wstring, TYPE> container;

public:
	ResourceContainer();
	~ResourceContainer();


	TYPE get( unsigned int id );
	inline unsigned int get_next_id() { return count; }
	inline TYPE get( const std::wstring& name )
	{
		auto iter = container.find( name );
		if ( iter != container.end() )
			return iter->second;
		return nullptr;
	}
};

template <class TYPE>
ResourceContainer<TYPE>::ResourceContainer()
{
	count = 1;
}

template <class TYPE>
ResourceContainer<TYPE>::~ResourceContainer( )
{
	for ( auto iter = container.begin( ); iter != container.end( ); iter++ )
	{
		delete iter.second;
	}
	container.clear();
}

template <class TYPE>
TYPE ResourceContainer<TYPE>::get( unsigned int id )
{
	for ( auto iter = container.begin(); iter != container.end(); iter++ )
	{
		if ( iter->second->get_id() == id )
			return iter->second;
	}
	return nullptr;
}

