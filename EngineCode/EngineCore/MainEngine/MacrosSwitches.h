#pragma once
/**
@file macros_switches.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Ten plik zawiera makrodefinicje b�d�ce prze��cznikami mi�dzy r�nymi opcjami kompilacji.
Wi�kszo�� z tych makrodefinicji pozwala na wyb�r jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba b�dzie si� zdecydowa� na kt�r�� z nich.*/

#include "Common/TypesDefinitions.h"

#include "Common/RTTR.h"


///Co tyle sekund aktualizujemy m_framesPerSec.
static const unsigned int FRAMES_PER_SEC_UPDATE = 10;
static const float FIXED_MOVE_UPDATE_INTERVAL = ((float)1 / (float)56);	///<Interwa�, po kt�rym nast�puje kolejne przeliczenie po�o�e� obiekt�w (w sekundach).

///@def _INTERPOLATE_POSITIONS
///@brief W��cza do kodu funkcje interpoluj�ca po�o�enia obiekt�w.
#define _INTERPOLATE_POSITIONS

//#define _QUATERNION_SPEED			//@def _QUATERNION_SPEED
/*	Predko�� obrotowa jest wyra�ona kwaternionem
Je�eli to makro nie jest zdefiniowane, to pr�dko�� obrotowa jest wyra�ona jako wektor
4-elementowy, w kt�rym pierwsze 3 sk�adowe opisuj� wektor obrotu, a sk�adowa w k�t obrotu*/

///@def __TEST
///@brief W��cza funkcje testuj�ce silnika.
///Funkcje testuj�ce maj� na celu umo�liwienie wczytywania modeli i ustawiania stan�w obiekt�w,
///mimo �e nie s� stworzone docelowe mechanizmy, kt�rych ma u�ywa� silnik.
#define __TEST

#define _SCALEABLE_OBJECTS			///<Umo�liwia ustawienie skalowania dla meshy.


/**@def ENGINE_MAX_LIGHTS
@brief Maksymalna liczba �wiate� w silniku.
*/
#define ENGINE_MAX_LIGHTS		10



/**@brief �cie�ka do pliku, do kt�rego b�d� trafia�y pomiary wydajno�ci.*/
static const char* PERFORMANCE_STATISTICS_FILE_PATH = "logs/performance.txt";



/**@def PERFORMANCE_CHECK
@brief W��cza funkcje klasy @ref PerformanceCheck do kodu silnika. Mierzy wydajno�� wybranych fragment�w kodu.
*/
#define PERFORMANCE_CHECK

#ifdef _DEBUG
	/**@def SHOW_MEMORY_LEAKS
	@brief W��cza do kodu wszystkie elementy niezb�dne do wykrywania wyciek�w
	*/
	#define SHOW_MEMORY_LEAKS
#endif
