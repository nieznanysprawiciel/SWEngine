#pragma once

#include "Common/TypesDefinitions.h"
#include "Common/RTTR.h"


/**@brief Klasa bazowa dla obiekt�w przechowuj�cych dane materia�u.

Klasa @ref MaterialAsset przechowuje bufor z danymi na karcie graficznej, ale r�wnie�
trzyma lokaln� kopi� tych danych na CPU. W silniku mo�na u�ywa� r�nych modeli cieniowania
st�d potrzebna jest klasa bazowa.

@ingroup Materials*/
class ShadingModelBase
{
	RTTR_ENABLE()
private:
public:

	virtual Size		GetSize		() = 0;
	virtual char*		GetData		() = 0;
	virtual rttr::type	GetType		() = 0;
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

	virtual Size		GetSize		()		{ return sizeof( ShadingModelStruct ); }
	virtual char*		GetData		()		{ return static_cast< void* >( &Data ); }
	virtual rttr::type	GetType		()		{ return rttr::type::get< ShadingModelStruct >(); }
};

