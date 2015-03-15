#pragma once

/*
includes classes:
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

/**@brief Klasa bazowa dla wszystkich obiektów w grze (aktorów).
*/
class Object
{
private:
	static Engine*	engine;		///< WskaŸnik na g³ówny obiekt silnika.
protected:
	short			class_type;
	short			object_type;

	void inline event(Event*);
	/**@brief Zwraca wskaŸnik na interfejs silnika, który nadaje siê do u¿ywania przez
	programistê gry.
	@attention Nie wolno rzutowaæ obiektu na Engine.
	*/
	inline engine_interface* get_engine_interface(){ return reinterpret_cast<engine_interface*>(engine); }
public:
	virtual ~Object() = default;
	virtual void init(){};

	/**@brief Funkcja ustawia wskaŸnik na g³ówny obiekt silnika.
	@attention Wolno u¿ywaæ tylko klasie Engine w konstruktorze.
	@param[in] engine_ptr WskaŸnik na g³ówny obiekt silnika.
	*/
	static void set_engine( Engine* engine_ptr ) { if( !engine ) engine = engine_ptr; }
};


class Static_object : public Object
{
protected:
	XMFLOAT3		position;
	XMFLOAT4		orientation;			//quaternion
public:
	Static_object();
	Static_object(const XMFLOAT3& pos, const XMFLOAT4& orient);

	inline void set_position(const XMVECTOR& pos) { XMStoreFloat3(&position, pos); }
	inline void set_orientation(const XMVECTOR& quaternion) { XMStoreFloat4(&orientation, quaternion); }
	inline XMVECTOR get_position() const { return XMLoadFloat3( &position ); };
	inline XMVECTOR get_orientation() const { return XMLoadFloat4( &orientation ); };
};


class Collision_object : public Static_object
{

};


class Dynamic_object : public Collision_object
{
protected:
	XMFLOAT3		speed;
	XMFLOAT4		rotation_speed;
	Controller*		controller;
public:
	Dynamic_object();
	Dynamic_object( const XMFLOAT3& move_speed, const XMFLOAT4& rot_speed );

	inline void set_speed( const XMVECTOR& vector ) { XMStoreFloat3( &speed, vector ); }
	inline void set_rotation_speed( const XMVECTOR& quaternion ) { XMStoreFloat4( &rotation_speed, quaternion ); }
	inline void set_rotation_speed( const XMFLOAT4 axis_angle ) { rotation_speed = axis_angle; }
	inline XMVECTOR get_speed() const { return XMLoadFloat3( &speed ); }
	inline XMVECTOR get_rotation_speed() const { return XMLoadFloat4( &rotation_speed ); }

	inline void set_controller( Controller* ctrl ) { controller = ctrl; }
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

/**@brief Klasa bazowa dla obiektów, które bêd¹ renderowane.
*/
class Dynamic_mesh_object : public Physical_object
{
	friend class DisplayEngine;
#ifdef _SCALEABLE_OBJECTS
private:
	float							scale;
public:
	inline void set_scale( float sc ) { scale = sc; };
#endif
protected:
	Model3DFromFile*				model_reference;					//zapisujemy odwo³anie, ¿ebyœmy wiedzieli, ¿e nie mo¿emy kasowaæ tego obiektu
	BufferObject*					vertex_buffer;						//ca³y bufor przechowujemy w jednym obiekcie
	BufferObject*					index_buffer;						//tak samo bufor indeksów
	std::vector<ModelPart>			model_parts;

	bool							model_changed;
public:
	Dynamic_mesh_object();
	virtual ~Dynamic_mesh_object();

	int set_model( Model3DFromFile* model );

private:
	void add_references(const ModelPart* part);
	void delete_all_references();

	/**@brief Funkcja wywo³ywana dla obiektów samo-renderuj¹cych. Ma za zadanie narysowaæ
	obiekt, dla którego zosta³a wywo³ana.

	Je¿eli obiekt ma siê renderowaæ w sposób standardowy, nie ma potrzeby implementowania tej funkcji.
	Zasadniczo nie powinno siê u¿ywaæ tego sposoby renderowania, je¿eli nie ma ku temu wa¿nych powodów,
	poniewa¿ uniemo¿liwia on skuteczn¹ optymalizacjê renderowania.

	@see @ref selfDrawingObjects

	@param[in] device_context WskaŸnik do obiektu directXa s³u¿¹cego do renderowania sceny.
	@param[in] time_interval Czas od ostatniej klatki.
	@param[in] time_lag Czas wzglêdem ostatniego przeliczenia po³o¿eñ.
	*/
	virtual void draw( DeviceContext* device_context, float time_interval, float time_lag ) {}
};

class Animation_object : public Physical_object
{
	friend class DisplayEngine;
protected:


};

/**@brief Klasa bazowa dla wszystkich obiektów kamer w silniku.
*/
class Camera_object : public Dynamic_object
{
	friend class DisplayEngine;
protected:
	DirectX::XMFLOAT4X4		projection_matrix;		///<Macierz projekcji. Dla ka¿dej kamery mo¿e byæ inna. @attention Na razie nieu¿ywane. Macierz projekcji jest ustawiana na sta³e w DisplayEngine.
public:
	void set_projection_matrix( float angle, float X_to_Y, float near_plane, float far_plane );
};


class Controller
{
public:
	virtual ~Controller() = default;
	virtual void control_object( Dynamic_object* ) = 0;
};

class Base_AI_controller : public Controller
{

	virtual void control_object( Dynamic_object* ) = 0;
};

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



/*Klasa obiektu z³o¿onego. Mo¿e zawieraæ w sobie wiele obiektów, których po³o¿enia
s¹ liczone wzglêdem danego obiektu. do tego celu zamiast funkcji move u¿ywa siê complex_move,
której dodatkowymi paramterami s¹ przesuniêcie i obrót rodzica. Objekty z³o¿one mog¹ siê zagnie¿d¿aæ.
Trzeba jednak uwa¿aæ, aby do klasy MovementEngine podaæ tylko obiekt nadrzêdny, w innym wypadku przesuniêcia
bêd¹ siê wlicza³y wielokrotnie. Obiekty bêd¹ce sk³adowymi Complex_obiekt tak¿e mog¹ wykonywaæ w³asne ruchy.*/
class Complex_object : public Dynamic_object
{
protected:
	std::vector<Dynamic_object*>	part;
public:
	void move_complex(float time_interval, const XMFLOAT3& parent_speed, const XMFLOAT4& parent_rotation);
};

