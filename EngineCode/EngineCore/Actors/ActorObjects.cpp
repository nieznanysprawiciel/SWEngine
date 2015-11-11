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

Engine* Object::engine = nullptr;		//po stworzeniu obiektu klasy Engine, zmienna ta jest uzupe³niana wskaxnikiem this



/**Funkcja pozwala wys³aæ event, który bêdzie potem przetworzony przez klase FableEngine.
Eventy s¹ metod¹ komunikacji pomiedzy silnikiem graficznym, silnikiem fizycznym, AI i silnikiem kolizji,
a modu³em silnika odpowiedzialnym za fabu³ê. Istnieje szereg eventów wbudowanych, wysy³anych przez silnik,
mo¿na równie¿ definiowaæ w³asne nowe eventy poprzez dziedziczenie z klasy Event. Event mo¿e byæ wys³any przez dowolny
objekt poprzez wywo³anie funkcji Object::event. Aby wys³aæ w³asny event trzeba przeci¹¿yæ jedn¹ z funkcji klas wbudowanych,
która jest potem wywo³ywana przez silnik i wywo³aæ tê funkjê.

Za zwolnienie pamiêci po klasie Event odpowiada klasa FabelEngine (jest to robione automatycznie po wywo³aniu funkcji obs³ugi,
u¿ytkownik nie musi siê tym przejmowac).*/
void inline Object::event(Event* new_event)
{
	engine->send_event(new_event);
}


/**Funkcja wykonywana w ka¿dym obiegu pêtli renderingu przez obiekt MovementEngine.
 Dodajemy aktualny wektor prêdkoœci i prêdkoœci obrotowej do wektorów po³o¿enia i orientacji.
 Poniewa¿ te wektory wyra¿aj¹ przesuniêcie/rotacje na sekundê, to musimy to przemno¿yæ jeszcze przez time_interval.
 
 W momencie wykonania zosta³y juz uwzglêdnione oddzia³ywania fizyczne oraz wp³yw kontrolerów.
 Nale¿y zauwa¿yæ, ¿e o ile najlepiej by by³o, ¿eby kontrolery tak¿e u¿ywa³y tych zmiennych do poruszania
 obiektami na scenie, to nie jest to obowi¹zek, bo maj¹ te¿ bezpoœredni dostêp do po³o¿enia i orientacji.
 
 Sprawdzanie kolizji i ewentualne przesuniêcia z tym zwi¹zane nastêpuj¹ dopiero po zakoñczeniu wywo³ywania tych funkcji.
 
 Do przeliczania u¿ywamy biblioteki DirectXmath, która u¿ywa zmiennych wektorowych i wykonuje obliczenia
 na jednostkach SSE2. Do tego potrzebne jest wyrównanie zmiennych do 16 bitów, czego nie oferuj¹ zmienne 
 XMFloat, dlatego trzeba wykonywaæ operacjê XMLoadFloat4 i XMStoreFloat4. 
 Uwaga zmienne XMVECTOR i XMMATRIX nie mog¹ byc alokowane na stercie, poniewa¿ nie jest tam gwarantowane 
 16bitowe wyrównanie. Po dok³adny opis odsy³am do MSDNu.*/
void DynamicObject::Move(float time_interval)
{
//translacja
	XMVECTOR pos = GetPosition();
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = GetSpeed();
	translate *= time;
	pos = pos + translate;
	SetPosition( pos );

//w docelowej wersji trzeba siê zdecydowaæ, co lepsze i resztê wywaliæ
#ifdef _QUATERNION_SPEED
//orientacja
	XMVECTOR orient = GetOrientation();
	XMVECTOR rot = GetRotationSpeed();

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni¹ orientacjê i orientacjê po sekundzie
	//ze wspóczynnikiem równym czasowi jaki up³yn¹³ faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du¿o obliczeñ, mo¿e da siê to jakoœ za³atwiæ bez interpolacji...*/

	SetOrientation( orient );
#else
	XMVECTOR orient = GetOrientation();				//pobieramy orientacjê
	XMVECTOR rot = GetRotationSpeed();				//pobieramy oœ obrotu (k¹t te¿, ale on nie ma znaczenia)

	if ( !XMVector3Equal( rot, XMVectorZero() ) )
	{
		float rot_angle = rotation_speed.w * time_interval;	//liczymy k¹t obrotu

		rot = XMQuaternionRotationAxis( rot, rot_angle );		//przerabiamy na kwaternion
		orient = XMQuaternionMultiply( orient, rot );			//liczymy nowy kwaternion orientacji
	}
		
	SetOrientation( orient );
#endif
}

/**Funkcja o zastosowaniu tym samym co Move, z t¹ ró¿nic¹, ¿e wykonywana dla obiektów z³o¿onych. Przesuniêcie
jest z³o¿eniem ruchu rodzica i danegi obiektu.

Funkcja jest wirtualna i w klasie ComplexObject (implementacja poni¿ej) wywo³uje rekurencyjnie tê funkcjê
dla wszystkich swoich dzieci.

UWAGA!! nale¿y pamiêtaæ, ¿e w klasie MovementEngine nie powinno wyst¹piæ ¿adne dziecko klasy ComplexObject.
W przeciwnym razie niektóre przesuniecia i obroty zostan¹ zastosowane wielokrotnie do jednego obiektu.*/
void DynamicObject::MoveComplex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation)
{
	return;
	//DOKOÑCZYC !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//translacja
	XMVECTOR pos = GetPosition();
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = XMLoadFloat3(&speed);
	XMVECTOR parent_translate = XMLoadFloat3(&parent_speed);
	
	translate += parent_translate;		//g³ówna ró¿nica: dodajemy przesuniêcie rodzica
	translate *= time;
	pos = pos + translate;
	SetPosition( pos );

	//orientacja
	XMVECTOR orient = GetOrientation();
	XMVECTOR rot = XMLoadFloat4(&rotation_speed);
	XMVECTOR parent_rotn = XMLoadFloat4(&parent_rotation);

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni¹ orientacjê i orientacjê po sekundzie
	//ze wspóczynnikiem równym czasowi jaki up³yn¹³ faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du¿o obliczeñ, mo¿e da siê to jakoœ za³atwiæ bez interpolacji...*/

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

/**@brief Zwraca interpolowan¹ pozycjê obiektu miêdzy przechowywanymi po³o¿eniami.

@param[in] time_lag Procent czasu jaki up³yn¹³ od ostaniej klatki do nastêpnej
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

/**@brief Zwraca interpolowan¹ orientacjê obiektu miêdzy przechowywanymi po³o¿eniami.

@param[in] time_lag Procent czasu jaki up³yn¹³ od ostaniej klatki do nastêpnej
Zakres [0,1].*/
XMVECTOR StaticObject::GetInterpolatedOrientation( float time_lag ) const
{
	XMVECTOR orient2 = XMLoadFloat4( &orientation );
	XMVECTOR orient1 = XMLoadFloat4( &orientation_back );
	if ( XMVector3Equal( orient1, orient2 ) )	// Wa¿ne! Oblcizenia na floatach s¹ niedok³adne i troszkê wszystko lata.
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
	//Kasujac obiekt nie wolno nam niczego usuwaæ, bo nic nie nale¿y do nas
	//jedynie kasujemy referencje

	//kasujemy referencje bezpoœrednie
	DeleteAllReferences();
}

int DynamicMeshObject::SetModel(Model3DFromFile* model)
{
	if( model == nullptr )
		return 1;

	//najpierw czyœcimy poprzedni¹ zawartoœæ
	//kasujemy referencje bezpoœrednie
	DeleteAllReferences();

	//dodajemy now¹ zawartoœæ
	model_reference = model;
	model->add_object_reference();

	vertex_buffer = model->get_vertex_buffer();
	if ( !vertex_buffer )
		return 1;	// Nie ma bufora wierzcho³ków, to nie ma dalej czego szukaæ
	vertex_buffer->add_object_reference();


	index_buffer = model->get_index_buffer();
	if ( index_buffer )		// Jak nie ma bufora indeksów to w zasadzie krzywda siê nikomu nie dzieje
		index_buffer->add_object_reference();		// Ale trzeba pilnowaæ, ¿eby nie dodawaæ odwo³añ do obiektu, którego nie ma

	unsigned int count = model->get_parts_count();
	model_parts.reserve( count );

	for (unsigned int i = 0; i < count; ++i)
	{//przepisujemy sobie wskaŸniki
		register const ModelPart* part = model->get_part( i );

		model_parts.push_back( *part );
		AddReferences(part);			//Dodajemy odwo³ania
	}

	//Pytanie czy to ma sens. Funkcja reserve ustawi³a wielkoœæ wektora na przynajmniej count.
	//Je¿eli da³a wiêcej, to warto by by³o zmniejszyæ, bo zawartoœæ tych wektorów siê ju¿ nie zmieni.
	//Problemem jest, ¿e obie te funkcje wed³ug dokumentacji nie s¹ wi¹¿¹ce.
	model_parts.shrink_to_fit( );

	model_changed = false;		//Zawartoœæ tablic odpowiada modelowi

	return 0;
}

/**
Dodajemy odwo³ania do wszystkich istniej¹cych elementów w przekazanym wskaŸniku.

@param[in] part Struktura ModelPart opisuj¹ce czêœæ mesha, w której dodajemy referencje.*/
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
Kasuje odwo³ania do obiektów, których w³asnoœci¹ jest ModelsManager albo Model3DFromFile
w tablicy model_parts oraz wskaŸniku model_reference i vertex_buffer.

¯adne obiekty nie s¹ kasowane, poniewa¿ nie nale¿¹ one do nas.
Wszystkie zmienne s¹ za to czyszczone.*/
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
	{//ka¿dy element mo¿e byæ nullptrem
		register ModelPart* part = &model_parts[k];

		//ka¿dy element mo¿e byæ nullptrem
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

@param[in] angle K¹t widzenia w pionie
@param[in] X_to_Y Stosunek Szerokoœci do wysokoœci ekranu
@param[in] near_plane Bli¿sza p³aszczyzna obcinania
@param[in] far_plane Dalsza p³aszczyzna obcinania*/
void CameraObject::SetProjectionMatrix( float angle, float X_to_Y, float near_plane, float far_plane )
{
	XMMATRIX proj_matrix = XMMatrixPerspectiveFovLH( angle, X_to_Y, near_plane, far_plane );
	proj_matrix = XMMatrixTranspose( proj_matrix );
	XMStoreFloat4x4( &projection_matrix, proj_matrix );
}
