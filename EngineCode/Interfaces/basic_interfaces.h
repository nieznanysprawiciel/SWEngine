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

class Object
{
	friend class Engine;
private:
	static Engine*	engine;
protected:
	short			class_type;
	short			object_type;

	void inline event(Event*);
	inline engine_interface* get_interface(){ return reinterpret_cast<engine_interface*>(engine); }
public:
	virtual ~Object() = default;
	virtual void init(){};
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
	inline XMVECTOR get_position() { return XMLoadFloat3( &position ); };
	inline XMVECTOR get_orientation() { return XMLoadFloat4( &orientation ); };
};


class Collision_object : public Static_object
{

};


class Dynamic_object : public Collision_object
{
	friend class ControllersEngine;
	friend class DisplayEngine;
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
	inline XMVECTOR get_speed( ) { return XMLoadFloat3( &speed ); };
	inline XMVECTOR get_rotation_speed( ) { return XMLoadFloat4( &rotation_speed ); };

	inline void set_controller( Controller* ctrl ) { controller = ctrl; };

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
	std::vector<ModelPart>			model_parts;

	bool							model_changed;
public:
	Dynamic_mesh_object();
	virtual ~Dynamic_mesh_object();

	int set_model( Model3DFromFile* model );

private:
	void add_references(const ModelPart* part);
	void delete_all_references();

};

class Animation_object : public Physical_object
{
	friend class DisplayEngine;
protected:


};

class Camera_object : public Dynamic_object
{
	friend class DisplayEngine;

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
	friend class UI_Engine;
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

