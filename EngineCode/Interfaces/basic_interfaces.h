#pragma once

/**
@file basic_interfaces.h
@brief Zawiera deklaracje klas bazowych dla obiekt�w(aktor�w):

Object
Static_object
Dynamic_object
Collision_object
Physical_object
Dynamic_mesh_object
Animation_object
Controller
Standard_AI_controller
Standard_input_controller
Camera_object
*/
#include "..\stdafx.h"
#include "..\Engine\ModelsManager\meshes_textures_materials.h"




using namespace DirectX;

class engine_interface;
class Engine;
class ControllersEngine;
class Controller;
class Event;
class InputAbstractionLayer_base;

/**@brief Klasa bazowa dla wszystkich obiekt�w w grze (aktor�w).
*/
class Object
{
private:
	static Engine*	engine;		///< Wska�nik na g��wny obiekt silnika.
protected:
	short			class_type;
	short			object_type;

	void inline event(Event*);
	/**@brief Zwraca wska�nik na interfejs silnika, kt�ry nadaje si� do u�ywania przez
	programist� gry.
	@attention Nie wolno rzutowa� obiektu na Engine.
	*/
	inline engine_interface* get_engine_interface(){ return reinterpret_cast<engine_interface*>(engine); }
public:
	virtual ~Object() = default;
	virtual void init(){};

	/**@brief Funkcja ustawia wska�nik na g��wny obiekt silnika.
	@attention Wolno u�ywa� tylko klasie Engine w konstruktorze.
	@param[in] engine_ptr Wska�nik na g��wny obiekt silnika.
	*/
	static void set_engine( Engine* engine_ptr ) { if( !engine ) engine = engine_ptr; }
};


/**@brief Klasa bazowa dla wszystkich obiekt�w statycznych w silniku.

Obiekty posiadaj� po dwie zmienne na orientacj� i pozycj�. Przechowywana jest zawsze pozycja
z poprzedniej klatki, dzi�ki czemu mo�na interpolowa� po�o�enie. (Wy�wietlanie jest op�nione
w czasie o jedn� klatk�, �eby interpolacja nie musia�a wyprzedza� faktycznych po�o�e�).

Swapowanie nast�puj� w funkcji @ref Dynamic_object::move. @todo Trzeba zbada� czy nie ma przypadk�w,
w kt�rych nie b�dzie zachodzi� swapowanie, a powinno (wydaje mi si� �e przy niekt�rych kontrolerach)
i jako� rozwi�za� t� sytuacj�.
*/
class Static_object : public Object
{
private:
	DirectX::XMFLOAT3		position;				///< Pozycja obiektu (lub bufor tylny)
	DirectX::XMFLOAT4		orientation;			///< Orientacja obiektu wyra�ona kwaternionem (lub bufor tylny)
	DirectX::XMFLOAT3		position_back;			///< Pozycja obiektu (lub bufor tylny)
	DirectX::XMFLOAT4		orientation_back;		///< Orientacja obiektu wyra�ona kwaternionem (lub bufor tylny)

	bool					swap_data;	///< Zmienna identyfikuj�ca, kt�re zmienne opisuj�ce po�o�enie s� u�ywane.
protected:

	/** @brief Funkcja przemieszcza obiekt w podane miejsce.
	Warto�ci po�rednie miedzy podanymi po�o�eniami zostan� zinterpolowane.
	Nowa warto�� jest wpisywana do bufora tylnego pozycji.

	@param[in] pos Po�o�enie docelowe obiektu.
	*/
	inline void set_position( const XMVECTOR& pos )
	{
		if ( swap_data )
			XMStoreFloat3( &position, pos );
		else
			XMStoreFloat3( &position_back, pos );
	}

	/** @brief Funkcja ustawia orientacj� obiektu. Warto�ci po�rednie s� interpolowane.
	
	Nowa warto�� jest wpisywana do bufora tylnego orientacji.

	@param[in] quaternion Orientacja docelowa obiektu.
	*/
	inline void set_orientation( const XMVECTOR& quaternion )
	{
		if ( swap_data )
			XMStoreFloat4( &orientation, quaternion );
		else
			XMStoreFloat4( &orientation_back, quaternion );
	}
public:
	Static_object();			///< Kontruktor domy�lny inicjuje obiekt w �rodku uk�adu wsp�rz�dnych.
	Static_object(const XMFLOAT3& pos, const XMFLOAT4& orient);	///< Inicjuje obiekt w podanym po�o�eniu

	/**@brief Przemieszcza obiekt w podane miejsce.
	
	Funkcja powinna zosta� u�yta, je�eli obiekt ma zosta� przemiszczony skokowo w jakie� miejsce.
	Warto�ci po�rednie nie b�d� interpolowane. Funkcj� nale�y tak�e wywo�a� przy inicjacji obiektu.
	
	@param[in] pos Nowe po�o�enie obiektu.*/
	inline void teleport( const XMVECTOR& pos ) { XMStoreFloat3( &position, pos ); XMStoreFloat3( &position_back, pos ); }

	/**@brief Zmienia orientacj� obiektu na podan�.

	Funkcja powinna zosta� u�yta, je�eli obiekt ma zosta� obr�cony skokowo.
	Warto�ci po�rednie nie b�d� interpolowane. Funkcj� nale�y tak�e wywo�a� przy inicjacji obiektu.

	@param[in] quaternion Kwaternion nowego obrotu.*/
	inline void teleport_orientation( const XMVECTOR& quaternion ) { XMStoreFloat4( &orientation, quaternion ); XMStoreFloat4( &orientation_back, quaternion ); }


	/**@brief Pobiera aktualn� pozycj� obiektu.
	
	@return Wektor zawieraj�cy pozycj� obiektu.*/
	inline XMVECTOR get_position() const
	{
		if ( swap_data )
			return XMLoadFloat3( &position_back );
		else
			return XMLoadFloat3( &position );
	}

	/**@brief Pobiera aktualn� orientacj� obiektu.

	@return Wektor zawieraj�cy orientacj� obiektu.*/
	inline XMVECTOR get_orientation() const
	{
		if ( swap_data )
			return XMLoadFloat4( &orientation_back );
		else
			return XMLoadFloat4( &orientation );
	}

	/**@brief Funkcja zamienia aktualne bufory na pozycj� i orientacj�.*/
	inline void swap() { if ( swap_data ) swap_data = false; else swap_data = true; }

	XMVECTOR get_interpolated_position( float frame_percent ) const;
	XMVECTOR get_interpolated_orientation( float frame_percent ) const;
};

/**@brief Klasa bazowa dla obiekt�w zdolnych do kolizji.*/
class Collision_object : public Static_object
{

};

/**@brief Klasa bazowa dla obiekt�w dynamicznych.

Je�eli jest zdefiniowana sta�a _QUATERNION_SPEED, to pr�dko�ci k�towe s� wyra�one
kwaternionem w przeciwnym razie jest to wektor, w kt�rym sk�adowa w jest k�tem obrotu.
W docelowej wersji b�dzie najprawdopodobniej wybrana opcja z wetorem a nie kwaternionem.

@note Niezaleznie od tego jak jest wyra�ona pr�dko��, orientacja zawsze jest kwaternionem.*/
class Dynamic_object : public Collision_object
{
protected:
	XMFLOAT3		speed;				///< Pr�dko�� postepowa obiektu.
	XMFLOAT4		rotation_speed;		///< Pr�dko�� k�towa obiektu (wyra�ona wektorem i k�tem obrotu w sk�adowej w).
	Controller*		controller;			///< Wska�nik na kontroler, poruszaj�cy obiektem.
public:
	Dynamic_object();	///< Kontruktor ustawi�j�cy zerow� pr�dko�� k�tow� i post�pow�.
	Dynamic_object( const XMFLOAT3& move_speed, const XMFLOAT4& rot_speed );	///< Kontruktor ustawia podan� w parametrach pr�dko��.

	inline void set_speed( const XMVECTOR& vector ) { XMStoreFloat3( &speed, vector ); }	///<Ustawia pr�dko�� obiektu @param[in] vector Wektor pr�dko�ci.
	inline void set_rotation_speed( const XMVECTOR& quaternion ) { XMStoreFloat4( &rotation_speed, quaternion ); }	///<Ustawia pr�dko�� obrotow� @param[in] quaternion Wektor pr�dko�ci.
	inline void set_rotation_speed( const XMFLOAT4 axis_angle ) { rotation_speed = axis_angle; }		///<Ustawia pr�dko�� obrotow� @param[in] quaternion Wektor pr�dko�ci.
	inline XMVECTOR get_speed() const { return XMLoadFloat3( &speed ); }	///< Zwraca pr�dko�� post�pow� obiektu.
	inline XMVECTOR get_rotation_speed() const { return XMLoadFloat4( &rotation_speed ); }	///< Zwraca pr�dko�� obrotow� obiektu.

	inline void set_controller( Controller* ctrl ) { controller = ctrl; }	///< Ustawia podany w parametrze kontroler
	inline Controller* get_controller() { return controller; }

	void move(float time_interval);
	virtual void move_complex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation);
};


class Physical_object : public Dynamic_object
{
protected:
	float			mass;
public:
	Physical_object();

	void pulse();
};

/**@brief Klasa bazowa dla obiekt�w, kt�re b�d� renderowane.
*/
class Dynamic_mesh_object : public Physical_object
{
	friend class DisplayEngine;
#ifdef _SCALEABLE_OBJECTS
private:
	float							scale;		///<Skalowanie wzgl�dem wszystkich osi.
public:
	inline void set_scale( float sc ) { scale = sc; };
#endif
protected:
	Model3DFromFile*				model_reference;					//zapisujemy odwo�anie, �eby�my wiedzieli, �e nie mo�emy kasowa� tego obiektu
	BufferObject*					vertex_buffer;						//ca�y bufor przechowujemy w jednym obiekcie
	BufferObject*					index_buffer;						//tak samo bufor indeks�w
	std::vector<ModelPart>			model_parts;

	bool							model_changed;
public:
	Dynamic_mesh_object();
	virtual ~Dynamic_mesh_object();

	int set_model( Model3DFromFile* model );

private:
	void add_references(const ModelPart* part);
	void delete_all_references();

	/**@brief Funkcja wywo�ywana dla obiekt�w samo-renderuj�cych. Ma za zadanie narysowa�
	obiekt, dla kt�rego zosta�a wywo�ana.

	Je�eli obiekt ma si� renderowa� w spos�b standardowy, nie ma potrzeby implementowania tej funkcji.
	Zasadniczo nie powinno si� u�ywa� tego sposoby renderowania, je�eli nie ma ku temu wa�nych powod�w,
	poniewa� uniemo�liwia on skuteczn� optymalizacj� renderowania.

	@see @ref selfDrawingObjects

	@param[in] device_context Wska�nik do obiektu directXa s�u��cego do renderowania sceny.
	@param[in] time_interval Czas od ostatniej klatki.
	@param[in] time_lag Czas wzgl�dem ostatniego przeliczenia po�o�e�.
	*/
	virtual void draw( DeviceContext* device_context, float time_interval, float time_lag ) {}
};

class Animation_object : public Physical_object
{
	friend class DisplayEngine;
protected:


};

/**@brief Klasa bazowa dla wszystkich obiekt�w kamer w silniku.
*/
class Camera_object : public Dynamic_object
{
	friend class DisplayEngine;
protected:
	DirectX::XMFLOAT4X4		projection_matrix;		///<Macierz projekcji. Dla ka�dej kamery mo�e by� inna. @attention Na razie nieu�ywane. Macierz projekcji jest ustawiana na sta�e w DisplayEngine.
public:
	void set_projection_matrix( float angle, float X_to_Y, float near_plane, float far_plane );
};

/**@brief Klasa bazowa dla wszystkich kontroler�w dla obiekt�w.*/
class Controller
{
public:
	virtual ~Controller() = default;
	virtual void control_object( Dynamic_object* ) = 0;
};

/**@brief Klasa bazowa dla wszystkich kontroler�w sztucznej inteligencji.*/
class Base_AI_controller : public Controller
{

	virtual void control_object( Dynamic_object* ) = 0;
};

/**@brief Klasa bazowa dla wszystkich kontroler�w do sterowania przez u�ytkownika.*/
class Base_input_controller : public Controller
{
protected:
	InputAbstractionLayer_base*		abstraction_layer;

public:
	Base_input_controller( InputAbstractionLayer_base* layer );
	virtual ~Base_input_controller();

	inline void set_abstraction_layer( InputAbstractionLayer_base* layer ) { abstraction_layer = layer; };

	virtual void control_object( Dynamic_object* ) = 0;
};



/*Klasa obiektu z�o�onego. Mo�e zawiera� w sobie wiele obiekt�w, kt�rych po�o�enia
s� liczone wzgl�dem danego obiektu. do tego celu zamiast funkcji move u�ywa si� complex_move,
kt�rej dodatkowymi paramterami s� przesuni�cie i obr�t rodzica. Objekty z�o�one mog� si� zagnie�d�a�.
Trzeba jednak uwa�a�, aby do klasy MovementEngine poda� tylko obiekt nadrz�dny, w innym wypadku przesuni�cia
b�d� si� wlicza�y wielokrotnie. Obiekty b�d�ce sk�adowymi Complex_obiekt tak�e mog� wykonywa� w�asne ruchy.*/
class Complex_object : public Dynamic_object
{
protected:
	std::vector<Dynamic_object*>	part;
public:
	void move_complex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation);
};

