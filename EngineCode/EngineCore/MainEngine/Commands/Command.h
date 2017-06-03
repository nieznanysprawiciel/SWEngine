#pragma once

#include "EngineCore/MainEngine/EngineInterface.h"

/**@page EngineInterfaceCommands Komendy silnika

Logika gry operuj�c na funkcjach API w EngineInterface, nie modyfikuje bezpo�rednio
stanu silnika. Silnik dzia�a wielow�tkowo i takie podej�cie mog�oby znacznie utrudni�
zachowanie sp�jno�ci danych. Z tego powodu aktorzy widz� zawsze stan silnika z pocz�tku danej
klatki i na nim operuj�.

Funkcje API EngineInterface tworz� odpowiednie komendy (dzidziczace po klasie Command) silnika
i dodaj� je do kolejki EngineContext::commandQueue. W momencie, gdy wszystkie operacje
zwi�zane z logik� gry zako�cz� si�, wykonywane s� komendy, aby ustawi�kt�re ustawiaj� nowy stan silnika.

@note Silnik gwarantuje, �e komendy s� wykonywane w jednym w�tku i wywo�ania funkcji nie wymagaj� �adnej synchronizacji.

Je�eli funkcje API nie s� wystarczaj�ce do spe�nienia wszystkich funkcjonalno�ci, istnieje mo�liwo��
napisania w�asnej komendy dziedzicz�c po klasie Command.
*/


namespace sw
{

/**@brief Klasa bazowa dla komend wewn�trz silnika.

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
