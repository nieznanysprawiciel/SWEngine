#pragma once

#include "Common/EngineObject.h"
#include "Common/TypesDefinitions.h"
#include "Common/RTTR.h"


/**@brief Klasa bazowa dla obiektów przechowuj¹cych dane materia³u.

Klasa @ref MaterialAsset przechowuje bufor z danymi na karcie graficznej, ale równie¿
trzyma lokaln¹ kopiê tych danych na CPU. W silniku mo¿na u¿ywaæ ró¿nych modeli cieniowania
st¹d potrzebna jest klasa bazowa.

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


/**@brief Klasa opakowuj¹ca dane materia³u.

Same dane materia³u powinny byæ przechowywane w klasach bêd¹cych POD (Plain old data).
Musz¹ siê one nadawaæ do bezpoœredniego updatowania bufora sta³ych dla shadera, dlatego
dane musz¹ byæ wyrównane oraz nie mog¹ zawieraæ wskaŸnika na vtable.

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
