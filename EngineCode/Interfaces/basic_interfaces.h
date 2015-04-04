#pragma once

/**
@file basic_interfaces.h
@brief Zawiera deklaracje klas bazowych dla obiektów(aktorów):

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


/**@brief Klasa bazowa dla wszystkich obiektów statycznych w silniku.

Obiekty posiadaj¹ po dwie zmienne na orientacjê i pozycjê. Przechowywana jest zawsze pozycja
z poprzedniej klatki, dziêki czemu mo¿na interpolowaæ po³o¿enie. (Wyœwietlanie jest opóŸnione
w czasie o jedn¹ klatkê, ¿eby interpolacja nie musia³a wyprzedzaæ faktycznych po³o¿eñ).

Swapowanie nastêpujê w funkcji @ref Dynamic_object::move. @todo Trzeba zbadaæ czy nie ma przypadków,
w których nie bêdzie zachodziæ swapowanie, a powinno (wydaje mi siê ¿e przy niektórych kontrolerach)
i jakoœ rozwi¹zaæ tê sytuacjê.
*/
class Static_object : public Object
{
private:
	DirectX::XMFLOAT3		position;				///< Pozycja obiektu (lub bufor tylny)
	DirectX::XMFLOAT4		orientation;			///< Orientacja obiektu wyra¿ona kwaternionem (lub bufor tylny)
	DirectX::XMFLOAT3		position_back;			///< Pozycja obiektu (lub bufor tylny)
	DirectX::XMFLOAT4		orientation_back;		///< Orientacja obiektu wyra¿ona kwaternionem (lub bufor tylny)

	bool					swap_data;	///< Zmienna identyfikuj¹ca, które zmienne opisuj¹ce po³o¿enie s¹ u¿ywane.
protected:

	/** @brief Funkcja przemieszcza obiekt w podane miejsce.
	Wartoœci poœrednie miedzy podanymi po³o¿eniami zostan¹ zinterpolowane.
	Nowa wartoœæ jest wpisywana do bufora tylnego pozycji.

	@param[in] pos Po³o¿enie docelowe obiektu.
	*/
	inline void set_position( const XMVECTOR& pos )
	{
		if ( swap_data )
			XMStoreFloat3( &position, pos );
		else
			XMStoreFloat3( &position_back, pos );
	}

	/** @brief Funkcja ustawia orientacjê obiektu. Wartoœci poœrednie s¹ interpolowane.
	
	Nowa wartoœæ jest wpisywana do bufora tylnego orientacji.

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
	Static_object();			///< Kontruktor domyœlny inicjuje obiekt w œrodku uk³adu wspó³rzêdnych.
	Static_object(const XMFLOAT3& pos, const XMFLOAT4& orient);	///< Inicjuje obiekt w podanym po³o¿eniu

	/**@brief Przemieszcza obiekt w podane miejsce.
	
	Funkcja powinna zostaæ u¿yta, je¿eli obiekt ma zostaæ przemiszczony skokowo w jakieœ miejsce.
	Wartoœci poœrednie nie bêd¹ interpolowane. Funkcjê nale¿y tak¿e wywo³aæ przy inicjacji obiektu.
	
	@param[in] pos Nowe po³o¿enie obiektu.*/
	inline void teleport( const XMVECTOR& pos ) { XMStoreFloat3( &position, pos ); XMStoreFloat3( &position_back, pos ); }

	/**@brief Zmienia orientacjê obiektu na podan¹.

	Funkcja powinna zostaæ u¿yta, je¿eli obiekt ma zostaæ obrócony skokowo.
	Wartoœci poœrednie nie bêd¹ interpolowane. Funkcjê nale¿y tak¿e wywo³aæ przy inicjacji obiektu.

	@param[in] quaternion Kwaternion nowego obrotu.*/
	inline void teleport_orientation( const XMVECTOR& quaternion ) { XMStoreFloat4( &orientation, quaternion ); XMStoreFloat4( &orientation_back, quaternion ); }


	/**@brief Pobiera aktualn¹ pozycjê obiektu.
	
	@return Wektor zawieraj¹cy pozycjê obiektu.*/
	inline XMVECTOR get_position() const
	{
		if ( swap_data )
			return XMLoadFloat3( &position_back );
		else
			return XMLoadFloat3( &position );
	}

	/**@brief Pobiera aktualn¹ orientacjê obiektu.

	@return Wektor zawieraj¹cy orientacjê obiektu.*/
	inline XMVECTOR get_orientation() const
	{
		if ( swap_data )
			return XMLoadFloat4( &orientation_back );
		else
			return XMLoadFloat4( &orientation );
	}

	/**@brief Funkcja zamienia aktualne bufory na pozycjê i orientacjê.*/
	inline void swap() { if ( swap_data ) swap_data = false; else swap_data = true; }

	XMVECTOR get_interpolated_position( float frame_percent ) const;
	XMVECTOR get_interpolated_orientation( float frame_percent ) const;
};

/**@brief Klasa bazowa dla obiektów zdolnych do kolizji.*/
class Collision_object : public Static_object
{

};

/**@brief Klasa bazowa dla obiektów dynamicznych.

Je¿eli jest zdefiniowana sta³a _QUATERNION_SPEED, to prêdkoœci k¹towe s¹ wyra¿one
kwaternionem w przeciwnym razie jest to wektor, w którym sk³adowa w jest k¹tem obrotu.
W docelowej wersji bêdzie najprawdopodobniej wybrana opcja z wetorem a nie kwaternionem.

@note Niezaleznie od tego jak jest wyra¿ona prêdkoœæ, orientacja zawsze jest kwaternionem.*/
class Dynamic_object : public Collision_object
{
protected:
	XMFLOAT3		speed;				///< Prêdkoœæ postepowa obiektu.
	XMFLOAT4		rotation_speed;		///< Prêdkoœæ k¹towa obiektu (wyra¿ona wektorem i k¹tem obrotu w sk³adowej w).
	Controller*		controller;			///< WskaŸnik na kontroler, poruszaj¹cy obiektem.
public:
	Dynamic_object();	///< Kontruktor ustawi¹j¹cy zerow¹ prêdkoœæ k¹tow¹ i postêpow¹.
	Dynamic_object( const XMFLOAT3& move_speed, const XMFLOAT4& rot_speed );	///< Kontruktor ustawia podan¹ w parametrach prêdkoœæ.

	inline void set_speed( const XMVECTOR& vector ) { XMStoreFloat3( &speed, vector ); }	///<Ustawia prêdkoœæ obiektu @param[in] vector Wektor prêdkoœci.
	inline void set_rotation_speed( const XMVECTOR& quaternion ) { XMStoreFloat4( &rotation_speed, quaternion ); }	///<Ustawia prêdkoœæ obrotow¹ @param[in] quaternion Wektor prêdkoœci.
	inline void set_rotation_speed( const XMFLOAT4 axis_angle ) { rotation_speed = axis_angle; }		///<Ustawia prêdkoœæ obrotow¹ @param[in] quaternion Wektor prêdkoœci.
	inline XMVECTOR get_speed() const { return XMLoadFloat3( &speed ); }	///< Zwraca prêdkoœæ postêpow¹ obiektu.
	inline XMVECTOR get_rotation_speed() const { return XMLoadFloat4( &rotation_speed ); }	///< Zwraca prêdkoœæ obrotow¹ obiektu.

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

/**@brief Klasa bazowa dla obiektów, które bêd¹ renderowane.
*/
class Dynamic_mesh_object : public Physical_object
{
	friend class DisplayEngine;
#ifdef _SCALEABLE_OBJECTS
private:
	float							scale;		///<Skalowanie wzglêdem wszystkich osi.
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

/**@brief Klasa bazowa dla wszystkich kontrolerów dla obiektów.*/
class Controller
{
public:
	virtual ~Controller() = default;
	virtual void control_object( Dynamic_object* ) = 0;
};

/**@brief Klasa bazowa dla wszystkich kontrolerów sztucznej inteligencji.*/
class Base_AI_controller : public Controller
{

	virtual void control_object( Dynamic_object* ) = 0;
};

/**@brief Klasa bazowa dla wszystkich kontrolerów do sterowania przez u¿ytkownika.*/
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

