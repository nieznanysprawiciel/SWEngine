#include "stdafx.h"
#include "UI_Engine.h"
#include "Engine.h"
#include "Interfaces\basic_interfaces.h"


#include "memory_leaks.h"


/**@brief Funkcja ustawia domyœlne warstwy abstrakcji w klasie UI_engine.*/
void UI_Engine::init_abstraction_layers( )
{
	//kolejnoœæ dodawania powinna byæ zgodna z enumem STANDARD_ABSTRACTION_LAYER,
	//poniewa¿ s³u¿y on potem jako indeks do odwo³ywania siê do tablicy
	InputAbstractionLayer* new_layer = new InputAbstractionLayer;
	new_layer->setup_buttons_layer( STANDARD_LAYERS::PROTOTYPE_BUTTONS_mapping, PROTOTYPE_BUTTONS_MAPPING_COUNT );
	new_layer->setup_axis_layer(STANDARD_LAYERS::PROTOTYPE_AXES_mapping, PROTOTYPE_AXES_MAPPING_COUNT);
	abstraction_layers.push_back( new_layer );

	current_abstraction_layer = new_layer;
	current_abstraction_layer->set_active( true );
}

/**@brief Zmienia aktualnie ustawion¹ warstwê abstrakcji na podan¹ w argumencie.
Warstwa nie musi byæ wczeœniej dodana do UI_Engine, ale zaleca siê, aby dodawaæ
wszystkie obiekty w celu pilnowania pamiêci.

@param[in] next_layer warstwa abstrakcji, która ma byæ aktywna.
@return
Zwraca wartoœæ 0 w przypadku powodzenia, a inne wartoœci, je¿eli podany argument
by³ nieprawid³owy:
- 1	-	je¿eli warstwa jest ju¿ ustawiona
- 2	-	je¿eli podano nullptr

Zasadniczo nie ma potrzeby sprawdzania wartoœci zwracanej, w przypadkach innych
ni¿ debugowanie programu.*/
int UI_Engine::change_abstraction_layer( InputAbstractionLayer* next_layer )
{
	if ( next_layer == nullptr )
		return 2;
	if ( next_layer == current_abstraction_layer )
		return 1;

	current_abstraction_layer->set_active( false );
	current_abstraction_layer = next_layer;
	current_abstraction_layer->set_active( true );

	return 0;
}

/**@brief Dodaje do wewnêtrznych tablic UI_Engine now¹ warstwê abstrakcji.
Nie jest to do niczego konieczne, ale warto to robiæ, aby silnik zarz¹dza³
pamiêci¹ zajmowan¹ przez warstwê.
@param[in] new_layer Warstwa do dodania.*/
void UI_Engine::add_abstraction_layer( InputAbstractionLayer* new_layer )
{
	if ( new_layer != nullptr )
		abstraction_layers.push_back( new_layer );
}

/**@brief Kasuje z tablic UI_Engine wartwê abstrakcji podan¹ w argumencie.
Wartwa nie mo¿e byæ w tym czasie aktywna.
@attention Warstwa zostanie usuniêta, a pamiêæ zwolniona. Do wskaŸnika podanego
w argumencie nie wolno siê wiêcej odwo³ywaæ.

@param[in] layer Warstwa, która ma zostaæ usuniêta z tablic silnika.
@return
Funkcja zwraca 0 je¿eli wszystko jest w porz¹dku. W innym wypadku:
- 1	-	nie by³o takiej wartwy w tablicach,
- 2	-	wartwa jest obecnie aktywna*/
int UI_Engine::delete_abstraction_layer( InputAbstractionLayer* layer )
{
	if ( layer == current_abstraction_layer )
		return 2;
	
	for ( unsigned int i = STANDARD_ABSTRACTION_LAYER_COUNT; i < abstraction_layers.size(); ++i )
		if ( abstraction_layers[i] == layer )
		{
			delete abstraction_layers[i];
			abstraction_layers.erase( abstraction_layers.begin() + i );
			return 0;
		}
	return 1;
}

/**@brief Ustawia jedn¹ ze standardowych warstw abstrakcji wbudowanych w silnik.

@param[in] layer Jedna z wartoœci @ref STANDARD_ABSTRACTION_LAYER, która indentyfikuje warstwê do ustawienia.
@return
Zwraca 0 w przypadku powodzenia.
Je¿eli podano liczbê wykraczaj¹c¹ poza zakres funkcja zwraca 1.*/
int UI_Engine::set_standard_abstraction_layer( STANDARD_ABSTRACTION_LAYER layer )
{
	if ( layer >= STANDARD_ABSTRACTION_LAYER_COUNT )
		return 1;

	change_abstraction_layer( abstraction_layers[layer] );

	return 0;
}

/**@brief Kasuje i zwalnia pamiêæ po wszystkich dodanych do silnika przez u¿ytkownika
warstwach abstrakcji. Je¿eli jedna z tych warstw abstrakcji bêdzie aktywna
zostanie równie¿ usuniêta, a na jej miejsce zostanie ustawiona jedna z wartstw wbudowanych.*/
void UI_Engine::clear_abstraction_layers( )
{

	for ( unsigned int i = STANDARD_ABSTRACTION_LAYER_COUNT; i < abstraction_layers.size(); ++i )
	{
		if ( current_abstraction_layer == abstraction_layers[i] )
			set_standard_abstraction_layer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS );
		delete abstraction_layers[i];
	}
	abstraction_layers.erase( abstraction_layers.begin() + STANDARD_ABSTRACTION_LAYER_COUNT,
		abstraction_layers.end() );
}

/**@brief Funkcja aktualizuje tablice w aktualnie aktywnej warstwie abstrakcji.

Funkcja jest wywo³ywana przez UI_engine, dlatego jest prywatna. W ka¿dej klatce nastêpuje
aktualizacja danych w aktualnie aktywnej warstwie abstrakcji.*/
void UI_Engine::update_abstraction_layer( )
{
	current_abstraction_layer->begin_event_collection();

	current_abstraction_layer->update_keyboard_device( DEVICE_IDs::KEYBOARD, keyboard_state );
	current_abstraction_layer->update_mouse_device( DEVICE_IDs::MOUSE, &mouse_state, engine->get_window_widht(), engine->get_window_height());

	current_abstraction_layer->send_events( engine );
}
