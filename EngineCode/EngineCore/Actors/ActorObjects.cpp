/**@file basic_interfaces.cpp
@brief Plik zawiera definicje metod klas z pliku basic_interfaces.h
*/

#include "EngineCore/stdafx.h"
#include "ActorObjects.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/ModelsManager/Model3DFromFile.h"
#include "GraphicAPI/IRenderer.h"
#include "GraphicAPI/MeshResources.h"


#include "Common/memory_leaks.h"

using namespace DirectX;

Engine* Object::engine = nullptr;		//po stworzeniu obiektu klasy Engine, zmienna ta jest uzupe�niana wskaxnikiem this



/**Funkcja pozwala wys�a� event, kt�ry b�dzie potem przetworzony przez klase FableEngine.
Eventy s� metod� komunikacji pomiedzy silnikiem graficznym, silnikiem fizycznym, AI i silnikiem kolizji,
a modu�em silnika odpowiedzialnym za fabu��. Istnieje szereg event�w wbudowanych, wysy�anych przez silnik,
mo�na r�wnie� definiowa� w�asne nowe eventy poprzez dziedziczenie z klasy Event. Event mo�e by� wys�any przez dowolny
objekt poprzez wywo�anie funkcji Object::event. Aby wys�a� w�asny event trzeba przeci��y� jedn� z funkcji klas wbudowanych,
kt�ra jest potem wywo�ywana przez silnik i wywo�a� t� funkj�.

Za zwolnienie pami�ci po klasie Event odpowiada klasa FabelEngine (jest to robione automatycznie po wywo�aniu funkcji obs�ugi,
u�ytkownik nie musi si� tym przejmowac).*/
void inline Object::event(Event* new_event)
{
	engine->send_event(new_event);
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
void DynamicObject::Move(float time_interval)
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
		float rot_angle = rotation_speed.w * time_interval;	//liczymy k�t obrotu

		rot = XMQuaternionRotationAxis( rot, rot_angle );		//przerabiamy na kwaternion
		orient = XMQuaternionMultiply( orient, rot );			//liczymy nowy kwaternion orientacji
	}
		
	SetOrientation( orient );
#endif
}

/**Funkcja o zastosowaniu tym samym co Move, z t� r�nic�, �e wykonywana dla obiekt�w z�o�onych. Przesuni�cie
jest z�o�eniem ruchu rodzica i danegi obiektu.

Funkcja jest wirtualna i w klasie ComplexObject (implementacja poni�ej) wywo�uje rekurencyjnie t� funkcj�
dla wszystkich swoich dzieci.

UWAGA!! nale�y pami�ta�, �e w klasie MovementEngine nie powinno wyst�pi� �adne dziecko klasy ComplexObject.
W przeciwnym razie niekt�re przesuniecia i obroty zostan� zastosowane wielokrotnie do jednego obiektu.*/
void DynamicObject::MoveComplex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation)
{
	return;
	//DOKO�CZYC !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//translacja
	XMVECTOR pos = GetPosition();
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = XMLoadFloat3(&speed);
	XMVECTOR parent_translate = XMLoadFloat3(&parent_speed);
	
	translate += parent_translate;		//g��wna r�nica: dodajemy przesuni�cie rodzica
	translate *= time;
	pos = pos + translate;
	SetPosition( pos );

	//orientacja
	XMVECTOR orient = GetOrientation();
	XMVECTOR rot = XMLoadFloat4(&rotation_speed);
	XMVECTOR parent_rotn = XMLoadFloat4(&parent_rotation);

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni� orientacj� i orientacj� po sekundzie
	//ze wsp�czynnikiem r�wnym czasowi jaki up�yn�� faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du�o oblicze�, mo�e da si� to jako� za�atwi� bez interpolacji...*/

	SetOrientation( orient );
}

void ComplexObject::MoveComplex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation)
{

}

//----------------------------------------------------------------------------------------------//
//									Static_object
//----------------------------------------------------------------------------------------------//

StaticObject::StaticObject()
{//inicjujemy obiekt w punkcie ( 0.0, 0.0, 0.0 ) i zorientowany tak jak jego mesh
	position.x = 0.0;
	position.y = 0.0;
	position.z = 0.0;

	position_back = position;

	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4(&orientation,quaternion);

	orientation_back = orientation;

	swap_data = false;
}

StaticObject::StaticObject(const XMFLOAT3& pos, const XMFLOAT4& orient)
{
	position = pos;
	position_back = pos;
	orientation = orient;
	orientation_back = orient;

	swap_data = false;
}

/**@brief Zwraca interpolowan� pozycj� obiektu mi�dzy przechowywanymi po�o�eniami.

@param[in] time_lag Procent czasu jaki up�yn�� od ostaniej klatki do nast�pnej
Zakres [0,1].*/
XMVECTOR StaticObject::GetInterpolatedPosition( float time_lag ) const
{
	XMVECTOR pos2 = XMLoadFloat3( &position );
	XMVECTOR pos1 = XMLoadFloat3( &position_back );
	if ( XMVector3Equal( pos1, pos2 ) )
		return pos1;

	if ( swap_data )
		pos1 = XMVectorLerp( pos2, pos1, time_lag );
	else
		pos1 = XMVectorLerp( pos1, pos2, time_lag );

	pos1 = XMVectorSetW( pos1, 1 );
	return pos1;
}

/**@brief Zwraca interpolowan� orientacj� obiektu mi�dzy przechowywanymi po�o�eniami.

@param[in] time_lag Procent czasu jaki up�yn�� od ostaniej klatki do nast�pnej
Zakres [0,1].*/
XMVECTOR StaticObject::GetInterpolatedOrientation( float time_lag ) const
{
	XMVECTOR orient2 = XMLoadFloat4( &orientation );
	XMVECTOR orient1 = XMLoadFloat4( &orientation_back );
	if ( XMVector3Equal( orient1, orient2 ) )	// Wa�ne! Oblcizenia na floatach s� niedok�adne i troszk� wszystko lata.
		return orient1;

	if ( swap_data )
		return XMQuaternionSlerp( orient2, orient1, time_lag );
	else
		return XMQuaternionSlerp( orient1, orient2, time_lag );
}

//----------------------------------------------------------------------------------------------//
//									DynamicObject
//----------------------------------------------------------------------------------------------//

DynamicObject::DynamicObject()
{
	speed.x = 0.0;
	speed.y = 0.0;
	speed.z = 0.0;

#ifdef _QUATERNION_SPEED
	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4( &rotation_speed, quaternion );
#else
	rotation_speed.w = 0.0;
	rotation_speed.x = 0.0;
	rotation_speed.y = 0.0;
	rotation_speed.z = 0.0;
#endif
}

DynamicObject::DynamicObject( const XMFLOAT3& move_speed, const XMFLOAT4& rot_speed )
{
	speed = move_speed;
	rotation_speed = rot_speed;
}

//----------------------------------------------------------------------------------------------//
//									PhysicalObject
//----------------------------------------------------------------------------------------------//

PhysicalObject::PhysicalObject()
{
	mass = 1;
}

//----------------------------------------------------------------------------------------------//
//									DynamicMeshObject
//----------------------------------------------------------------------------------------------//

DynamicMeshObject::DynamicMeshObject()
{
	model_reference = nullptr;
	model_changed = false;
	vertex_buffer = nullptr;
	index_buffer = nullptr;
#ifdef _SCALEABLE_OBJECTS
	scale = 1.0;
#endif
}

DynamicMeshObject::~DynamicMeshObject()
{
	//Kasujac obiekt nie wolno nam niczego usuwa�, bo nic nie nale�y do nas
	//jedynie kasujemy referencje

	//kasujemy referencje bezpo�rednie
	DeleteAllReferences();
}

int DynamicMeshObject::SetModel(Model3DFromFile* model)
{
	if( model == nullptr )
		return 1;

	//najpierw czy�cimy poprzedni� zawarto��
	//kasujemy referencje bezpo�rednie
	DeleteAllReferences();

	//dodajemy now� zawarto��
	model_reference = model;
	model->add_object_reference();

	vertex_buffer = model->get_vertex_buffer();
	if ( !vertex_buffer )
		return 1;	// Nie ma bufora wierzcho�k�w, to nie ma dalej czego szuka�
	vertex_buffer->add_object_reference();


	index_buffer = model->get_index_buffer();
	if ( index_buffer )		// Jak nie ma bufora indeks�w to w zasadzie krzywda si� nikomu nie dzieje
		index_buffer->add_object_reference();		// Ale trzeba pilnowa�, �eby nie dodawa� odwo�a� do obiektu, kt�rego nie ma

	unsigned int count = model->get_parts_count();
	model_parts.reserve( count );

	for (unsigned int i = 0; i < count; ++i)
	{//przepisujemy sobie wska�niki
		register const ModelPart* part = model->get_part( i );

		model_parts.push_back( *part );
		AddReferences(part);			//Dodajemy odwo�ania
	}

	//Pytanie czy to ma sens. Funkcja reserve ustawi�a wielko�� wektora na przynajmniej count.
	//Je�eli da�a wi�cej, to warto by by�o zmniejszy�, bo zawarto�� tych wektor�w si� ju� nie zmieni.
	//Problemem jest, �e obie te funkcje wed�ug dokumentacji nie s� wi���ce.
	model_parts.shrink_to_fit( );

	model_changed = false;		//Zawarto�� tablic odpowiada modelowi

	return 0;
}

/**
Dodajemy odwo�ania do wszystkich istniej�cych element�w w przekazanym wska�niku.

@param[in] part Struktura ModelPart opisuj�ce cz�� mesha, w kt�rej dodajemy referencje.*/
void DynamicMeshObject::AddReferences( const ModelPart* part )
{
	if ( part == nullptr )
		return;

	if ( part->material )
		part->material->add_object_reference();
	if ( part->mesh )
		part->mesh->add_object_reference();
	if ( part->pixel_shader )
		part->pixel_shader->add_object_reference();
	for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
		if ( part->texture[i] )
			part->texture[i]->add_object_reference( );
	if ( part->vertex_shader )
		part->vertex_shader->add_object_reference();

}

/**
Kasuje odwo�ania do obiekt�w, kt�rych w�asno�ci� jest ModelsManager albo Model3DFromFile
w tablicy model_parts oraz wska�niku model_reference i vertex_buffer.

�adne obiekty nie s� kasowane, poniewa� nie nale�� one do nas.
Wszystkie zmienne s� za to czyszczone.*/
void DynamicMeshObject::DeleteAllReferences( )
{
	if ( model_reference != nullptr )
		model_reference->delete_object_reference( );
	model_reference = nullptr;

	if ( vertex_buffer )
		vertex_buffer->delete_object_reference();
	vertex_buffer = nullptr;

	if ( index_buffer )
		index_buffer->delete_object_reference( );
	index_buffer = nullptr;

	for ( unsigned int k = 0; k < model_parts.size( ); ++k )
	{//ka�dy element mo�e by� nullptrem
		register ModelPart* part = &model_parts[k];

		//ka�dy element mo�e by� nullptrem
		if ( part->material )
			part->material->delete_object_reference();
		if ( part->mesh )
			part->mesh->delete_object_reference( );
		if ( part->pixel_shader )
			part->pixel_shader->delete_object_reference( );
		for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
			if ( part->texture[i] )
				part->texture[i]->delete_object_reference( );
		if ( part->vertex_shader )
			part->vertex_shader->delete_object_reference( );
	}
	model_parts.clear();
}

//----------------------------------------------------------------------------------------------//
//									BaseInputController
//----------------------------------------------------------------------------------------------//

BaseInputController::BaseInputController( InputAbstractionLayer_base* layer )
: abstraction_layer( layer )
{}

BaseInputController::~BaseInputController(){};


//----------------------------------------------------------------------------------------------//
//									CameraObject
//----------------------------------------------------------------------------------------------//


/**@brief Tworzy macierz projekcji dla danej kamery i umieszcza w polu projection_matrix

@param[in] angle K�t widzenia w pionie
@param[in] X_to_Y Stosunek Szeroko�ci do wysoko�ci ekranu
@param[in] near_plane Bli�sza p�aszczyzna obcinania
@param[in] far_plane Dalsza p�aszczyzna obcinania*/
void CameraObject::SetProjectionMatrix( float angle, float X_to_Y, float near_plane, float far_plane )
{
	XMMATRIX proj_matrix = XMMatrixPerspectiveFovLH( angle, X_to_Y, near_plane, far_plane );
	proj_matrix = XMMatrixTranspose( proj_matrix );
	XMStoreFloat4x4( &projection_matrix, proj_matrix );
}
