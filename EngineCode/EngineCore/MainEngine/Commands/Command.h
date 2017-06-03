#pragma once

#include "EngineCore/MainEngine/EngineInterface.h"

/**@page EngineInterfaceCommands Komendy silnika

Logika gry operuj¹c na funkcjach API w EngineInterface, nie modyfikuje bezpoœrednio
stanu silnika. Silnik dzia³a wielow¹tkowo i takie podejœcie mog³oby znacznie utrudniæ
zachowanie spójnoœci danych. Z tego powodu aktorzy widz¹ zawsze stan silnika z pocz¹tku danej
klatki i na nim operuj¹.

Funkcje API EngineInterface tworz¹ odpowiednie komendy (dzidziczace po klasie Command) silnika
i dodaj¹ je do kolejki EngineContext::commandQueue. W momencie, gdy wszystkie operacje
zwi¹zane z logik¹ gry zakoñcz¹ siê, wykonywane s¹ komendy, aby ustawiæktóre ustawiaj¹ nowy stan silnika.

@note Silnik gwarantuje, ¿e komendy s¹ wykonywane w jednym w¹tku i wywo³ania funkcji nie wymagaj¹ ¿adnej synchronizacji.

Je¿eli funkcje API nie s¹ wystarczaj¹ce do spe³nienia wszystkich funkcjonalnoœci, istnieje mo¿liwoœæ
napisania w³asnej komendy dziedzicz¹c po klasie Command.
*/


namespace sw
{

/**@brief Klasa bazowa dla komend wewn¹trz silnika.

@see @ref EngineInterfaceCommands*/
class Command
{
private:
protected:
public:
	Command() = default;
	virtual ~Command() = default;


	/**@brief Kod komendy do wykonania.*/
	virtual bool		Execute( EngineContext& engine ) = 0;
};

}	// sw
