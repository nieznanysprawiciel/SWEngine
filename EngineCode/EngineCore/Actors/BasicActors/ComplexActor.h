#pragma once


#include "EngineCore/Actors/ActorPointer.h"
#include "DynamicActor.h"


struct ChildActor
{
	ActorPtr< StaticActor >		Actor;
	DirectX::XMFLOAT4			Rotation;
	DirectX::XMFLOAT3			Translation;
};



/*Klasa obiektu z³o¿onego. Mo¿e zawieraæ w sobie wiele obiektów, których po³o¿enia
s¹ liczone wzglêdem danego obiektu. do tego celu zamiast funkcji Move u¿ywa siê complex_move,
której dodatkowymi paramterami s¹ przesuniêcie i obrót rodzica. Objekty z³o¿one mog¹ siê zagnie¿d¿aæ.
Trzeba jednak uwa¿aæ, aby do klasy MovementEngine podaæ tylko obiekt nadrzêdny, w innym wypadku przesuniêcia
bêd¹ siê wlicza³y wielokrotnie. Obiekty bêd¹ce sk³adowymi Complex_obiekt tak¿e mog¹ wykonywaæ w³asne ruchy.

@ingroup Actors*/
class ComplexActor : public DynamicActor
{
	RTTR_ENABLE( DynamicActor )
protected:

	std::vector< ChildActor >	m_components;


public:
	void			MoveComplex		( float time_interval, const DirectX::XMFLOAT3& parent_speed, const DirectX::XMFLOAT4& parent_rotation );


	static ActorBase*			Create()	{ return new ComplexActor; }
};



