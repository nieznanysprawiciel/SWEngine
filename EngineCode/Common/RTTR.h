#pragma once

// Windows.h header often defines theese macros adn causes conflicts
#undef max
#undef min

#include "Tools/Reflection/RTTR.h"


/**@defgroup Reflection Mechanizm refleksji.
@ingroup Tools
@brief Biblioteka zapewnia dynamiczn� kontrol� typ�w obiekt�w oraz mechanizmy rejestrowania
metainformacji.

Silnik wykorzystuje bibliotek� RTTR autorstwa Axela Menzela udost�pnian� na licencji
MIT z ma�ymi przer�bkami.
Dokumentacja i przyk�ady znajduj� si� na stronie:
http://www.rttr.org/ */
