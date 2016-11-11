#pragma once
/**
@file BufferUpdaterBase.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "Common/EngineObject.h"
#include "Common/TypesDefinitions.h"
#include "Common/RTTR.h"

#include "GraphicAPI/MeshResources.h"


class BufferUpdaterBase : public EngineObject
{
	RTTR_ENABLE( EngineObject )
private:
protected:

	ResourcePtr< BufferObject >		m_buffer;

public:
	BufferUpdaterBase();

	virtual Size		GetBufferSize		() = 0;
	virtual uint8*		GetBufferData		() = 0;
	virtual TypeID		GetBufferType		() = 0;
	virtual TypeID		GetBufferPtrType	() = 0;
	
	std::string			GetBufferTypeName	();
};



template< typename BufferType >
class BufferUpdater : public BufferUpdaterBase, public BufferType
{
	RTTR_ENABLE( BufferUpdaterBase )
private:
public:
	explicit			BufferUpdater		();
	void				StaticValidate		();


	virtual Size		GetBufferSize		() override		{ return sizeof( BufferType ); }
	virtual uint8*		GetBufferData		() override		{ return reinterpret_cast< uint8* >( static_cast< BufferType* >( this ) ); }
	virtual TypeID		GetBufferType		() override		{ return rttr::type::get< BufferType >(); }
	virtual TypeID		GetBufferPtrType	() override		{ return rttr::type::get< BufferType* >(); }

	BufferType&			GetBufferData		()		{ return static_cast< BufferType& >( *this ); }
};



//====================================================================================//
//			Inline definitions	
//====================================================================================//

template< typename BufferType >
inline void			BufferUpdater< BufferType >::StaticValidate		()
{
	static_assert( std::is_default_constructible< BufferType >::value, "Buffer struct must be default constructible." );

	// Constructor must be registered in rttr with AsRawPtr policy.
	assert( TypeID::get< BufferUpdater< BufferType > >().get_constructor().is_valid() );
	assert( TypeID::get< BufferUpdater< BufferType > >().get_constructor().get_instanciated_type().is_pointer() );
}

template< typename BufferType >
inline				BufferUpdater< BufferType >::BufferUpdater	()
{
	StaticValidate();
}


