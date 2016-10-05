#pragma once

#include "Common/EngineObject.h"
#include "Common/TypesDefinitions.h"
#include "Common/RTTR.h"


/**@brief Klasa bazowa dla obiekt�w przechowuj�cych dane materia�u.

Klasa @ref MaterialAsset przechowuje bufor z danymi na karcie graficznej, ale r�wnie�
trzyma lokaln� kopi� tych danych na CPU. W silniku mo�na u�ywa� r�nych modeli cieniowania
st�d potrzebna jest klasa bazowa.

@ingroup Materials*/
class ShadingModelBase : public EngineObject
{
	RTTR_ENABLE( EngineObject )
private:
public:
	ShadingModelBase();

	virtual Size		GetShadingModelSize		() = 0;
	virtual uint8*		GetShadingModelData		() = 0;
	virtual TypeID		GetShadingModelType		() = 0;
	virtual TypeID		GetShadingModelPtrType	() = 0;
	

	std::string			GetShadingModelTypeName ();
};


/**@brief Klasa opakowuj�ca dane materia�u.

Same dane materia�u powinny by� przechowywane w klasach b�d�cych POD (Plain old data).
Musz� si� one nadawa� do bezpo�redniego updatowania bufora sta�ych dla shadera, dlatego
dane musz� by� wyr�wnane oraz nie mog� zawiera� wska�nika na vtable.

Klasa ShadingModelData pozwala na przechowywanie informacji o typie struktury z danymi.

@ingroup Materials*/
template< typename ShadingModelStruct >
class ShadingModelData : public ShadingModelBase
{
	RTTR_ENABLE( ShadingModelBase )
private:
public:
	ShadingModelStruct		Data;

public:
	explicit			ShadingModelData		();
	explicit			ShadingModelData		( ShadingModelStruct model );
	void				StaticValidate			();


	virtual Size		GetShadingModelSize		() override		{ return sizeof( ShadingModelStruct ); }
	virtual uint8*		GetShadingModelData		() override		{ return reinterpret_cast< uint8* >( &Data ); }
	virtual TypeID		GetShadingModelType		() override		{ return rttr::type::get< ShadingModelStruct >(); }
	virtual TypeID		GetShadingModelPtrType	() override		{ return rttr::type::get< ShadingModelStruct* >(); }

	ShadingModelStruct&	GetStruct	()		{ return Data; }
};


//====================================================================================//
//			Inline definitions	
//====================================================================================//

template< typename ShadingModelStruct >
inline void			ShadingModelData< ShadingModelStruct >::StaticValidate		()
{
	static_assert( std::is_default_constructible< ShadingModelStruct >::value, "Material data must be default constructible." );

	// Constructor must be registered in rttr with AsRawPtr policy.
	assert( TypeID::get< ShadingModelData< ShadingModelStruct > >().get_constructor().is_valid() );
	assert( TypeID::get< ShadingModelData< ShadingModelStruct > >().get_constructor().get_instanciated_type().is_pointer() );
	assert( TypeID::get< ShadingModelData< ShadingModelStruct > >().get_property( "Data" ).is_valid() );
}

template< typename ShadingModelStruct >
inline				ShadingModelData< ShadingModelStruct >::ShadingModelData	()
{
	StaticValidate();
}


template< typename ShadingModelStruct >
inline				ShadingModelData< ShadingModelStruct >::ShadingModelData	( ShadingModelStruct model )
	:	Data( model )
{
	StaticValidate();
}
