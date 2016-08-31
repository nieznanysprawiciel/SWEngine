#pragma once

#include "Common/TypesDefinitions.h"
#include "Common/RTTR.h"


/**@brief Klasa bazowa dla obiektów przechowuj¹cych dane materia³u.

Klasa @ref MaterialAsset przechowuje bufor z danymi na karcie graficznej, ale równie¿
trzyma lokaln¹ kopiê tych danych na CPU. W silniku mo¿na u¿ywaæ ró¿nych modeli cieniowania
st¹d potrzebna jest klasa bazowa.

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

	virtual Size		GetSize		()		{ return sizeof( ShadingModelStruct ); }
	virtual char*		GetData		()		{ return static_cast< void* >( &Data ); }
	virtual rttr::type	GetType		()		{ return rttr::type::get< ShadingModelStruct >(); }
};

