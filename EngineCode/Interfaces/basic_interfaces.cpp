#include "..\stdafx.h"
#include "basic_interfaces.h"
#include "..\Engine\Engine.h"


Engine* Object::engine = nullptr;		//po stworzeniu obiektu klasy Engine, zmienna ta jest uzupe³niana wskaxnikiem this



/*Funkcja pozwala wys³aæ event, który bêdzie potem przetworzony przez klase FableEngine.
*Eventy s¹ metod¹ komunikacji pomiedzy silnikiem graficznym, silnikiem fizycznym, AI i silnikiem kolizji,
*a modu³em silnika odpowiedzialnym za fabu³ê. Istnieje szereg eventów wbudowanych, wysy³anych przez silnik,
*mo¿na równie¿ definiowaæ w³asne nowe eventy poprzez dziedziczenie z klasy Event. Event mo¿e byæ wys³any przez dowolny
*objekt poprzez wywo³anie funkcji Object::event. Aby wys³aæ w³asny event trzeba przeci¹¿yæ jedn¹ z funkcji klas wbudowanych,
*która jest potem wywo³ywana przez silnik i wywo³aæ tê funkjê.
*
*Za zwolnienie pamiêci po klasie Event odpowiada klasa FabelEngine (jest to robione automatycznie po wywo³aniu funkcji obs³ugi,
*u¿ytkownik nie musi siê tym przejmowac).*/
void inline Object::event(Event* new_event)
{
	engine->events_queue->push(new_event);
}


/*Funkcja wykonywana w ka¿dym obiegu pêtli renderingu przez obiekt MovementEngine.
 *Dodajemy aktualny wektor prêdkoœci i prêdkoœci obrotowej do wektorów po³o¿enia i orientacji.
 *Poniewa¿ te wektory wyra¿aj¹ przesuniêcie/rotacje na sekundê, to musimy to przemno¿yæ jeszcze przez time_interval.
 *
 *W momencie wykonania zosta³y juz uwzglêdnione oddzia³ywania fizyczne oraz wp³yw kontrolerów.
 *Nale¿y zauwa¿yæ, ¿e o ile najlepiej by by³o, ¿eby kontrolery tak¿e u¿ywa³y tych zmiennych do poruszania
 *obiektami na scenie, to nie jest to obowi¹zek, bo maj¹ te¿ bezpoœredni dostêp do po³o¿enia i orientacji.
 *
 *Sprawdzanie kolizji i ewentualne przesuniêcia z tym zwi¹zane nastêpuj¹ dopiero po zakoñczeniu wywo³ywania tych funkcji.
 *
 *Do przeliczania u¿ywamy biblioteki DirectXmath, która u¿ywa zmiennych wektorowych i wykonuje obliczenia
 *na jednostkach SSE2. Do tego potrzebne jest wyrównanie zmiennych do 16 bitów, czego nie oferuj¹ zmienne 
 *XMFloat, dlatego trzeba wykonywaæ operacjê XMLoadFloat4 i XMStoreFloat4. 
 *Uwaga zmienne XMVECTOR i XMMATRIX nie mog¹ byc alokowane na stercie, poniewa¿ nie jest tam gwarantowane 
 *16bitowe wyrównanie. Po dok³adny opis odsy³am do MSDNu.*/
void Dynamic_object::move(float time_interval)
{
//translacja
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = XMLoadFloat3(&speed);
	translate *= time;
	pos = pos + translate;
	XMStoreFloat3(&position, pos);

//w docelowej wersji trzeba siê zdecydowaæ, co lepsze i resztê wywaliæ
#ifdef _QUATERNION_SPEED
//orientacja
	XMVECTOR orient = XMLoadFloat4(&orientation);
	XMVECTOR rot = XMLoadFloat4(&rotation_speed);

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni¹ orientacjê i orientacjê po sekundzie
	//ze wspóczynnikiem równym czasowi jaki up³yn¹³ faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du¿o obliczeñ, mo¿e da siê to jakoœ za³atwiæ bez interpolacji...*/

	XMStoreFloat4(&orientation, orient);
#else
	XMVECTOR orient = XMLoadFloat4( &orientation );		//pobieramy orientacjê
	XMVECTOR rot = XMLoadFloat4( &rotation_speed );		//pobieramy oœ obrotu (k¹t te¿, ale on nie ma znaczenia)

	if ( !XMVector3Equal( rot, XMVectorZero( ) ) )
	{
		float rot_angle = rotation_speed.w * time_interval;	//liczymy k¹t obrotu

		rot = XMQuaternionRotationAxis( rot, rot_angle );		//przerabiamy na kwaternion
		orient = XMQuaternionMultiply( orient, rot );			//liczymy nowy kwaternion orientacji

		XMStoreFloat4( &orientation, orient );
	}
	//else: wtedy orientacja siê nie zmienia
#endif
}

/*Funkcja o zastosowaniu tym samym co move, z t¹ ró¿nic¹, ¿e wykonywana dla obiektów z³o¿onych. Przesuniêcie
jest z³o¿eniem ruchu rodzica i danegi obiektu.

Funkcja jest wirtualna i w klasie Complex_object (implementacja poni¿ej) wywo³uje rekurencyjnie tê funkcjê
dla wszystkich swoich dzieci.

UWAGA!! nale¿y pamiêtaæ, ¿e w klasie MovementEngine nie powinno wyst¹piæ ¿adne dziecko klasy Complex_object.
W przeciwnym razie niektóre przesuniecia i obroty zostan¹ zastosowane wielokrotnie do jednego obiektu.*/
void Dynamic_object::move_complex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation)
{
	return;
	//DOKOÑCZYC !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//translacja
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR time = XMVectorReplicate(time_interval);
	XMVECTOR translate = XMLoadFloat3(&speed);
	XMVECTOR parent_translate = XMLoadFloat3(&parent_speed);
	
	translate += parent_translate;		//g³ówna ró¿nica: dodajemy przesuniêcie rodzica
	translate *= time;
	pos = pos + translate;
	XMStoreFloat3(&position, pos);

	//orientacja
	XMVECTOR orient = XMLoadFloat4(&orientation);
	XMVECTOR rot = XMLoadFloat4(&rotation_speed);
	XMVECTOR parent_rotn = XMLoadFloat4(&parent_rotation);

	//najpierw liczymy nowy kwaternion dla obrotu w czasie sekundy
	rot = XMQuaternionMultiply(orient, rot);
	//teraz interpolujemy poprzedni¹ orientacjê i orientacjê po sekundzie
	//ze wspóczynnikiem równym czasowi jaki up³yn¹³ faktycznie
	orient = XMQuaternionSlerp(orient, rot, time_interval);

	/*Du¿o obliczeñ, mo¿e da siê to jakoœ za³atwiæ bez interpolacji...*/

	XMStoreFloat4(&orientation, orient);
}

void Complex_object::move_complex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation)
{

}

//----------------------------------------------------------------------------------------------//
//									Static_object
//----------------------------------------------------------------------------------------------//

Static_object::Static_object()
{//inicjujemy obiekt w punkcie ( 0.0, 0.0, 0.0 ) i zorientowany tak jak jego mesh
	position.x = 0.0;
	position.y = 0.0;
	position.z = 0.0;

	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4(&orientation,quaternion);
}

Static_object::Static_object(const XMFLOAT3& pos, const XMFLOAT4& orient)
{
	position = pos;
	orientation = orient;
}

//----------------------------------------------------------------------------------------------//
//									Dynamic_object
//----------------------------------------------------------------------------------------------//

Dynamic_object::Dynamic_object()
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

Dynamic_object::Dynamic_object( const XMFLOAT3& move_speed, const XMFLOAT4& rot_speed )
{
	speed = move_speed;
	rotation_speed = rot_speed;
}

//----------------------------------------------------------------------------------------------//
//									Physical_object
//----------------------------------------------------------------------------------------------//

Physical_object::Physical_object()
{
	mass = 1;
}

//----------------------------------------------------------------------------------------------//
//									Dynamic_mesh_object
//----------------------------------------------------------------------------------------------//

Dynamic_mesh_object::Dynamic_mesh_object()
{
	model_reference = nullptr;
#ifdef _SCALEABLE_OBJECTS
	scale = 1.0;
#endif
}

Dynamic_mesh_object::~Dynamic_mesh_object()
{
	//Kasujac obiekt nie wolno nam niczego usuwaæ, bo nic nie nale¿y do nas
	//jedynie ksujemy referencje

	if( model_reference != nullptr )
		model_reference->delete_reference();

	//kasujemy referencje bezpoœrednie
	//zak³adamy, ¿e tablice s¹ takie same, bo z takim warunkiem je stworzyliœmy
	for (unsigned int k = 0; k < materials.size(); ++k)
	{//ka¿dy element mo¿e byæ nullptrem
		if( mesh_parts[k] != nullptr )
			mesh_parts[k]->mesh_object->delete_object_reference();
		if( materials[k] != nullptr )
			materials[k]->delete_object_reference();
		if( textures[k] != nullptr )
			textures[k]->delete_object_reference();
	}
}

int Dynamic_mesh_object::set_model(Model3DFromFile* model)
{
	if( model == nullptr )
		return 1;

	//najpierw czyœcimy poprzedni¹ zawartoœæ
	//kasujemy odwo³anie plikowe
	if( model_reference != nullptr )
		model_reference->delete_reference();

	//kasujemy referencje bezpoœrednie
	//zak³adamy, ¿e tablice s¹ takie same, bo z takim warunkiem je stworzyliœmy
	for (unsigned int k = 0; k < materials.size(); ++k)
	{//ka¿dy element mo¿e byæ nullptrem
		if( mesh_parts[k] != nullptr )
			mesh_parts[k]->mesh_object->delete_object_reference();
		if( materials[k] != nullptr )
			materials[k]->delete_object_reference();
		if( textures[k] != nullptr )
			textures[k]->delete_object_reference();
	}

	mesh_parts.clear();
	materials.clear();
	textures.clear();


	model_reference = model;
	model->add_reference();
	unsigned int count = model->get_parts_count();

	mesh_parts.reserve( count );
	materials.reserve( count );
	textures.reserve( count );

	for (unsigned int i = 0; i < count; ++i)
	{//przepisujemy sobie wskaŸniki
		MeshPart* mesh = model->get_mesh_part(i);
		MaterialObject* material = model->get_material(i);
		TextureObject* texture = model->get_texture(i);

		mesh_parts.push_back(mesh);
		materials.push_back(material);
		textures.push_back(texture);

	//ka¿dy element mo¿e byæ nullptrem
		if( mesh != nullptr )
			mesh->mesh_object->add_object_reference();
		if( material != nullptr )
			material->add_object_reference();
		if( texture != nullptr )
			texture->add_object_reference();
	}

	//Pytanie czy to ma sens. Funkcja reserve ustawi³a wielkoœæ wektora na przynajmniej count.
	//Je¿eli da³a wiêcej, to warto by by³o zmniejszyæ, bo zawartoœæ tych wektorów siê ju¿ nie zmieni.
	//Problemem jest, ¿e obie te funkcje wed³ug dokumentacji nie s¹ wi¹¿¹ce.
	mesh_parts.shrink_to_fit();
	materials.shrink_to_fit();
	textures.shrink_to_fit();

	return 0;
}

//----------------------------------------------------------------------------------------------//
//									Base_input_controller
//----------------------------------------------------------------------------------------------//

Base_input_controller::Base_input_controller( InputAbstractionLayer_base* layer )
: abstraction_layer( layer )
{}

Base_input_controller::~Base_input_controller(){};

