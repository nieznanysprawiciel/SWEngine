#pragma once


#include "DynamicActor.h"


/*Klasa obiektu z�o�onego. Mo�e zawiera� w sobie wiele obiekt�w, kt�rych po�o�enia
s� liczone wzgl�dem danego obiektu. do tego celu zamiast funkcji Move u�ywa si� complex_move,
kt�rej dodatkowymi paramterami s� przesuni�cie i obr�t rodzica. Objekty z�o�one mog� si� zagnie�d�a�.
Trzeba jednak uwa�a�, aby do klasy MovementEngine poda� tylko obiekt nadrz�dny, w innym wypadku przesuni�cia
b�d� si� wlicza�y wielokrotnie. Obiekty b�d�ce sk�adowymi Complex_obiekt tak�e mog� wykonywa� w�asne ruchy.*/
class ComplexActor : public DynamicActor
{
	RTTR_ENABLE( DynamicActor )
protected:
	std::vector<DynamicActor*>	part;
public:
	void MoveComplex( float time_interval, const DirectX::XMFLOAT3& parent_speed, const DirectX::XMFLOAT4& parent_rotation );


	static EngineObject*			Create()	{ return new ComplexActor; }
};



