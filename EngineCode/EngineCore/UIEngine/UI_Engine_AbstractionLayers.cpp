#include "EngineCore/stdafx.h"
#include "UI_Engine.h"
#include "EngineCore/MainEngine/Engine.h"
#include "EngineCore/Actors/ActorObjects.h"


#include "Common/MemoryLeaks.h"


/**@brief Funkcja ustawia domyœlne warstwy abstrakcji w klasie UI_engine.*/
void UI_Engine::InitAbstractionLayers( )
{
	//kolejnoœæ dodawania powinna byæ zgodna z enumem STANDARD_ABSTRACTION_LAYER,
	//poniewa¿ s³u¿y on potem jako indeks do odwo³ywania siê do tablicy
	InputAbstractionLayer* newLayer = new InputAbstractionLayer;
	newLayer->SetupButtonsLayer( std::vector< InputMapping>( STANDARD_LAYERS::PROTOTYPE_BUTTONS_mapping ) );	///<@todo Zastanowiæ siê czy nie zrobiæ std::move na tym wektorze.
	newLayer->SetupAxisLayer( std::vector< InputMapping>( STANDARD_LAYERS::PROTOTYPE_AXES_mapping ) );
	//newLayer->demand_down_event( )
	m_abstractionLayers.push_back( newLayer );

	m_currentAbstractionLayer = newLayer;
	m_currentAbstractionLayer->set_active( true );
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

/**@brief Dodaje do wewnêtrznych tablic UI_Engine now¹ warstwê abstrakcji.
Nie jest to do niczego konieczne, ale warto to robiæ, aby silnik zarz¹dza³
pamiêci¹ zajmowan¹ przez warstwê.
@param[in] newLayer Warstwa do dodania.*/
void UI_Engine::AddAbstractionLayer( InputAbstractionLayer* new_layer )
{
	if ( new_layer != nullptr )
		m_abstractionLayers.push_back( new_layer );
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

/**@brief Ustawia jedn¹ ze standardowych warstw abstrakcji wbudowanych w silnik.

@param[in] layer Jedna z wartoœci @ref STANDARD_ABSTRACTION_LAYER, która indentyfikuje warstwê do ustawienia.
@return
Zwraca 0 w przypadku powodzenia.
Je¿eli podano liczbê wykraczaj¹c¹ poza zakres funkcja zwraca 1.*/
int UI_Engine::SetStandardAbstractionLayer( STANDARD_ABSTRACTION_LAYER layer )
{
	if ( layer >= STANDARD_ABSTRACTION_LAYER_COUNT )
		return 1;

	ChangeAbstractionLayer( m_abstractionLayers[layer] );

	return 0;
}

/**@brief Kasuje i zwalnia pamiêæ po wszystkich dodanych do silnika przez u¿ytkownika
warstwach abstrakcji. Je¿eli jedna z tych warstw abstrakcji bêdzie aktywna
zostanie równie¿ usuniêta, a na jej miejsce zostanie ustawiona jedna z wartstw wbudowanych.*/
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

Funkcja jest wywo³ywana przez UI_engine, dlatego jest prywatna. W ka¿dej klatce nastêpuje
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
