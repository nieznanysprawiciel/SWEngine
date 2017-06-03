/**
@file DynamicActor.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EngineCore/stdafx.h"
#include "DynamicActor.h"
#include "EngineCore/ControllersEngine/BaseClasses/IController.h"

#include "swCommonLib/Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< DynamicActor >( "DynamicActor" )
		.property( "Speed", &DynamicActor::m_speed )
		(
			rttr::metadata( MetaDataType::Category, "Movement" ),
			rttr::policy::prop::bind_as_ptr
		)
		.property( "RotationSpeed", &DynamicActor::m_rotationSpeed )
		(
			rttr::metadata( MetaDataType::Category, "Movement" ),
			rttr::policy::prop::bind_as_ptr
		)
		.property( "Mass", &DynamicActor::m_mass )
		(
			rttr::metadata( MetaDataType::Category, "Physical properties" )
		);
}


using namespace DirectX;



// ================================ //
//
DynamicActor::DynamicActor()
	: m_controller( nullptr )
{
	m_speed.x = 0.0;
	m_speed.y = 0.0;
	m_speed.z = 0.0;

#ifdef _QUATERNION_SPEED
	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4( &m_rotationSpeed, quaternion );
#else
	m_rotationSpeed.w = 0.0;
	m_rotationSpeed.x = 0.0;
	m_rotationSpeed.y = 0.0;
	m_rotationSpeed.z = 0.0;
#endif

	m_mass = 1.0f;
}

// ================================ //
//
DynamicActor::DynamicActor( const XMFLOAT3& move_speed, const XMFLOAT4& rot_speed )
	: m_controller( nullptr )
{
	m_speed = move_speed;
	m_rotationSpeed = rot_speed;

	m_mass = 1.0f;
}

/**@brief Usuwa przypisany kontroler.*/
DynamicActor::~DynamicActor()
{
	delete m_controller;
}


/**Funkcja wykonywana w ka�dym obiegu p�tli renderingu przez obiekt MovementEngine.
 Dodajemy aktualny wektor pr�dko�ci i pr�dko�ci obrotowej do wektor�w po�o�enia i orientacji.
 Poniewa� te wektory wyra�aj� przesuni�cie/rotacje na sekund�, to musimy to przemno�y� jeszcze przez time_interval.
 
 W momencie wykonania zosta�y juz uwzgl�dnione oddzia�ywania fizyczne oraz wp�yw kontroler�w.
 Nale�y zauwa�y�, �e o ile najlepiej by by�o, �eby kontrolery tak�e u�ywa�y tych zmiennych do poruszania
 obiektami na scenie, to nie jest to obowi�zek, bo maj� te� bezpo�redni dost�p do po�o�enia i orientacji.
 
 Sprawdzanie kolizji i ewentualne przesuni�cia z tym zwi�zane nast�puj� dopiero po zako�czeniu wywo�ywania tych funkcji.
 
 Do przeliczania u�ywamy biblioteki DirectXmath, kt�ra u�ywa zmiennych wektorowych i wykonuje obliczenia
 na jednostkach SSE2. Do tego potrzebne jest wyr�wnanie zmiennych do 16 bit�w, czego nie oferuj� zmienne 
 XMFloat, dlatego trzeba wykonywa� operacj� XMLoadFloat4 i XMStoreFloat4. 
 Uwaga zmienne XMVECTOR i XMMATRIX nie mog� byc alokowane na stercie, poniewa� nie jest tam gwarantowane 
 16bitowe wyr�wnanie. Po dok�adny opis odsy�am do MSDNu.*/
void DynamicActor::Move(float time_interval)
{
//translacja
	XMVECTOR pos = GetPosition();
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = GetSpeed();
	translate *= time;
	pos = pos + translate;
	SetPosition( pos );

//w docelowej wersji trzeba si� zdecydowa�, co lepsze i reszt� wywali�
#ifdef _QUATERNION_SPEED
//orientacja
	XMVECTOR orient = GetOrientation();
	XMVECTOR rot = GetRotationSpeed();

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni� orientacj� i orientacj� po sekundzie
	//ze wsp�czynnikiem r�wnym czasowi jaki up�yn�� faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du�o oblicze�, mo�e da si� to jako� za�atwi� bez interpolacji...*/

	SetOrientation( orient );
#else
	XMVECTOR orient = GetOrientation();				//pobieramy orientacj�
	XMVECTOR rot = GetRotationSpeed();				//pobieramy o� obrotu (k�t te�, ale on nie ma znaczenia)

	if ( !XMVector3Equal( rot, XMVectorZero() ) )
	{
		float rot_angle = m_rotationSpeed.w * time_interval;	//liczymy k�t obrotu

		rot = XMQuaternionRotationAxis( rot, rot_angle );		//przerabiamy na kwaternion
		orient = XMQuaternionMultiply( orient, rot );			//liczymy nowy kwaternion orientacji
	}
		
	SetOrientation( orient );
#endif
}

/**Funkcja o zastosowaniu tym samym co Move, z t� r�nic�, �e wykonywana dla obiekt�w z�o�onych. Przesuni�cie
jest z�o�eniem ruchu rodzica i danegi obiektu.

Funkcja jest wirtualna i w klasie ComplexActor (implementacja poni�ej) wywo�uje rekurencyjnie t� funkcj�
dla wszystkich swoich dzieci.

UWAGA!! nale�y pami�ta�, �e w klasie MovementEngine nie powinno wyst�pi� �adne dziecko klasy ComplexActor.
W przeciwnym razie niekt�re przesuniecia i obroty zostan� zastosowane wielokrotnie do jednego obiektu.*/
void DynamicActor::MoveComplex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation)
{
	return;
	//DOKO�CZYC !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//translacja
	XMVECTOR pos = GetPosition();
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = XMLoadFloat3(&m_speed);
	XMVECTOR parent_translate = XMLoadFloat3(&parent_speed);
	
	translate += parent_translate;		//g��wna r�nica: dodajemy przesuni�cie rodzica
	translate *= time;
	pos = pos + translate;
	SetPosition( pos );

	//orientacja
	XMVECTOR orient = GetOrientation();
	XMVECTOR rot = XMLoadFloat4(&m_rotationSpeed);
	XMVECTOR parent_rotn = XMLoadFloat4(&parent_rotation);

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni� orientacj� i orientacj� po sekundzie
	//ze wsp�czynnikiem r�wnym czasowi jaki up�yn�� faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du�o oblicze�, mo�e da si� to jako� za�atwi� bez interpolacji...*/

	SetOrientation( orient );
}


// ================================ //
//
void				DynamicActor::SetController		( IController* ctrl )
{
	m_controller = ctrl;
	m_controller->Initialize( this );
}

// ================================ //
//
IController*		DynamicActor::GetController		()
{
	return m_controller;
}

