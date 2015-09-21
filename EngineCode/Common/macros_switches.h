#pragma once
/**
@file macros_switches.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Ten plik zawiera makrodefinicje b�d�ce prze��cznikami mi�dzy r�nymi opcjami kompilacji.
Wi�kszo�� z tych makrodefinicji pozwala na wyb�r jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba b�dzie si� zdecydowa� na kt�r�� z nich.*/

#include "Common/TypesDefinitions.h"

///Co tyle sekund aktualizujemy frames_per_sec.
static const unsigned int FRAMES_PER_SEC_UPDATE = 10;			

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

/**@def ENGINE_MAX_TEXTURES
@brief Maksymalna liczba tekstur w silniku.
*/
#define ENGINE_MAX_TEXTURES		8
/**@def ENGINE_MAX_LIGHTS
@brief Maksymalna liczba �wiate� w silniku.
*/
#define ENGINE_MAX_LIGHTS		2


///@todo Stworzy� mechanizmy pozwalaj�ce na wybieranie bufor�w indeks�w 8, 16 i 32 bitowych
//#define INDEX_BUFFER_UINT8
//#define INDEX_BUFFER_UINT16		// Definiujemy czy chcemy mie� bufor indeks�w 16-bitowy czy 32-bitowy
#define INDEX_BUFFER_UINT32			// Wybra� tylko jedno, inaczej b�dzie b��d kompilacji

#if defined(INDEX_BUFFER_UINT16)
	static const unsigned int VERTICIES_MAX_COUNT = 0xFFFF;
#elif defined(INDEX_BUFFER_UINT32)
	static const unsigned int VERTICIES_MAX_COUNT = 0xFFFFFFFF;
#else
	static const unsigned int VERTICIES_MAX_COUNT = 0xFFFFFFFF;
#endif

/**@brief �cie�ka do pliku, do kt�rego b�d� trafia�y pomiary wydajno�ci.*/
static const char* PERFORMANCE_STATISTICS_FILE_PATH = "logs/performance.txt";

///@typedef VERT_INDEX
///@brief Definiujemy typ w buforze indeks�w.

///W zale�no�ci od tego czy wybierzemy bufor 16 bitowy czy 32 bitowy VERT_INDEX jest ustawiany jako UINT16 lub UINT32.
#if defined(INDEX_BUFFER_UINT16)
	typedef uint16 VERT_INDEX;
#elif defined(INDEX_BUFFER_UINT32)
	typedef uint32 VERT_INDEX;
#else
	typedef uint32 VERT_INDEX;
#endif


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
