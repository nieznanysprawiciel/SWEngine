#include "EngineCore/stdafx.h"
#include "UI_Engine.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/ActorObjects.h"


#include "Common/MemoryLeaks.h"


/**@brief Funkcja ustawia domy�lne warstwy abstrakcji w klasie UI_engine.*/
void UI_Engine::InitAbstractionLayers( )
{
	//kolejno�� dodawania powinna by� zgodna z enumem STANDARD_ABSTRACTION_LAYER,
	//poniewa� s�u�y on potem jako indeks do odwo�ywania si� do tablicy
	InputAbstractionLayer* newLayer = new InputAbstractionLayer;
	newLayer->SetupButtonsLayer( std::vector< InputMapping>( STANDARD_LAYERS::PROTOTYPE_BUTTONS_mapping ) );	///<@todo Zastanowi� si� czy nie zrobi� std::move na tym wektorze.
	newLayer->SetupAxisLayer( std::vector< InputMapping>( STANDARD_LAYERS::PROTOTYPE_AXES_mapping ) );
	//newLayer->demand_down_event( )
	m_abstractionLayers.push_back( newLayer );

	m_currentAbstractionLayer = newLayer;
	m_currentAbstractionLayer->set_active( true );
}

/**@brief Zmienia aktualnie ustawion� warstw� abstrakcji na podan� w argumencie.
Warstwa nie musi by� wcze�niej dodana do UI_Engine, ale zaleca si�, aby dodawa�
wszystkie obiekty w celu pilnowania pami�ci.

@param[in] next_layer warstwa abstrakcji, kt�ra ma by� aktywna.
@return
Zwraca warto�� 0 w przypadku powodzenia, a inne warto�ci, je�eli podany argument
by� nieprawid�owy:
- 1	-	je�eli warstwa jest ju� ustawiona
- 2	-	je�eli podano nullptr

Zasadniczo nie ma potrzeby sprawdzania warto�ci zwracanej, w przypadkach innych
ni� debugowanie programu.*/
int UI_Engine::ChangeAbstractionLayer( InputAbstractionLayer* next_layer )
{
	if ( next_layer == nullptr )
		return 2;
	if ( next_layer == m_currentAbstractionLayer )
		return 1;

	m_currentAbstractionLayer->set_active( false );
	m_currentAbstractionLayer = next_layer;
	m_currentAbstractionLayer->set_active( true );

	return 0;
}

/**@brief Dodaje do wewn�trznych tablic UI_Engine now� warstw� abstrakcji.
Nie jest to do niczego konieczne, ale warto to robi�, aby silnik zarz�dza�
pami�ci� zajmowan� przez warstw�.
@param[in] newLayer Warstwa do dodania.*/
void UI_Engine::AddAbstractionLayer( InputAbstractionLayer* new_layer )
{
	if ( new_layer != nullptr )
		m_abstractionLayers.push_back( new_layer );
}

/**@brief Kasuje z tablic UI_Engine wartw� abstrakcji podan� w argumencie.
Wartwa nie mo�e by� w tym czasie aktywna.
@attention Warstwa zostanie usuni�ta, a pami�� zwolniona. Do wska�nika podanego
w argumencie nie wolno si� wi�cej odwo�ywa�.

@param[in] layer Warstwa, kt�ra ma zosta� usuni�ta z tablic silnika.
@return
Funkcja zwraca 0 je�eli wszystko jest w porz�dku. W innym wypadku:
- 1	-	nie by�o takiej wartwy w tablicach,
- 2	-	wartwa jest obecnie aktywna*/
int UI_Engine::DeleteAbstractionLayer( InputAbstractionLayer* layer )
{
	if ( layer == m_currentAbstractionLayer )
		return 2;
	
	for ( unsigned int i = STANDARD_ABSTRACTION_LAYER_COUNT; i < m_abstractionLayers.size(); ++i )
		if ( m_abstractionLayers[i] == layer )
		{
			delete m_abstractionLayers[i];
			m_abstractionLayers.erase( m_abstractionLayers.begin() + i );
			return 0;
		}
	return 1;
}

/**@brief Ustawia jedn� ze standardowych warstw abstrakcji wbudowanych w silnik.

@param[in] layer Jedna z warto�ci @ref STANDARD_ABSTRACTION_LAYER, kt�ra indentyfikuje warstw� do ustawienia.
@return
Zwraca 0 w przypadku powodzenia.
Je�eli podano liczb� wykraczaj�c� poza zakres funkcja zwraca 1.*/
int UI_Engine::SetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER layer )
{
	if ( layer >= STANDARD_ABSTRACTION_LAYER_COUNT )
		return 1;

	ChangeAbstractionLayer( m_abstractionLayers[layer] );

	return 0;
}

/**@brief Kasuje i zwalnia pami�� po wszystkich dodanych do silnika przez u�ytkownika
warstwach abstrakcji. Je�eli jedna z tych warstw abstrakcji b�dzie aktywna
zostanie r�wnie� usuni�ta, a na jej miejsce zostanie ustawiona jedna z wartstw wbudowanych.*/
void UI_Engine::ClearAbstractionLayers( )
{

	for ( unsigned int i = STANDARD_ABSTRACTION_LAYER_COUNT; i < m_abstractionLayers.size(); ++i )
	{
		if ( m_currentAbstractionLayer == m_abstractionLayers[i] )
			SetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER::PROTOTYPE_BUTTONS );
		delete m_abstractionLayers[i];
	}
	m_abstractionLayers.erase( m_abstractionLayers.begin() + STANDARD_ABSTRACTION_LAYER_COUNT,
		m_abstractionLayers.end() );
}

/**@brief Funkcja aktualizuje tablice w aktualnie aktywnej warstwie abstrakcji.

Funkcja jest wywo�ywana przez UI_engine, dlatego jest prywatna. W ka�dej klatce nast�puje
aktualizacja danych w aktualnie aktywnej warstwie abstrakcji.*/
void UI_Engine::UpdateAbstractionLayer( )
{
	if( m_enableInput )
	{
		m_currentAbstractionLayer->begin_event_collection();

		m_currentAbstractionLayer->update_keyboard_device( DEVICE_IDs::KEYBOARD, keyboard_state );
		m_currentAbstractionLayer->update_mouse_device( DEVICE_IDs::MOUSE, &mouse_state, engine->GetWindowWidth(), engine->GetWindowHeight());

		m_currentAbstractionLayer->send_events( engine );
	}
}
