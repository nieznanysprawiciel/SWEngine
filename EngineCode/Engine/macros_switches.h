#pragma once

/**
@file macros_switches.h
@brief Ten plik zawiera makrodefinicje b�d�ce prze��cznikami mi�dzy r�nymi opcjami kompilacji.
Wi�kszo�� z tych makrodefinicji pozwala na wyb�r jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba b�dzie si� zdecydowa� na kt�r�� z nich.*/

#include "types_definitions.h"

extern const unsigned int FRAMES_PER_SEC_UPDATE;	///<Co tyle sekund aktualizujemy frames_per_sec.

///@def _INTERPOLATE_POSITIONS
///@brief W��cza funkcj� interpoluj�ca po�o�enia obiekt�w.
#define _INTERPOLATE_POSITIONS

//#define _QUATERNION_SPEED			//@def _QUATERNION_SPEED
/*	Predko�� obrotowa jest wyra�ona kwaternionem
Je�eli to makro nie jest zdefiniowane, to pr�dko�� obrotowa jest wyra�ona jako wektor
4-elementowy, w kt�rym pierwsze 3 sk�adowe opisuj� wektor obrotu, a sk�adowa w k�t obrotu*/

///@def __TEST
///@brief W��cza funkcje testuj�ce
#define __TEST

#define _SCALEABLE_OBJECTS			///umo�liwia ustawienie skalowania dla meshy
/**@def ENGINE_MAX_TEXTURES
@brief Maksymalna liczba tekstur w silniku
*/

#define ENGINE_MAX_TEXTURES		8
#define ENGINE_MAX_LIGHTS		2


//#define INDEX_BUFFER_UINT16		// Definiujemy czy chcemy mie� bufor indeks�w 16-bitowy czy 32-bitowy
#define INDEX_BUFFER_UINT32			// Wybra� tylko jedno, inaczej b�dzie b��d kompilacji

extern const unsigned int VERTICIES_MAX_COUNT;		///<Zawiera maksymalny rozmiar bufora indeks�w, czyli jednocze�nie liczb� wierzcho�k�w mesha.

extern const char* PERFORMANCE_STATISTICS_FILE_PATH;		///<Klasa PerformanceCheck b�dzie wysy�a�a tutaj zarejestrowane dane.

///@typedef VERT_INDEX
///@brief Definiujemy typ w buforze indeks�w.

///W zale�no�ci od tego czy wybierzemy bufor 16 bitowy czy 32 bitowy VERT_INDEX jest ustawiany jako UINT16 lub UINT32.
#if defined(INDEX_BUFFER_UINT16)
	typedef uint16 VERT_INDEX;
	#define INDEX_BUFFER_FORMAT DXGI_FORMAT_R16_UINT
#elif defined(INDEX_BUFFER_UINT32)
	typedef uint32 VERT_INDEX;
	#define INDEX_BUFFER_FORMAT DXGI_FORMAT_R32_UINT
#else
	typedef uint32 VERT_INDEX;
	#define INDEX_BUFFER_FORMAT DXGI_FORMAT_R32_UINT
#endif


/**@def PERFORMANCE_CHECK
@brief W��cza funkcje klasy PerformaceCheck do kodu silnika. Mierzy wydajno�� wybranych fragment�w kodu.
*/
#define PERFORMANCE_CHECK

#ifdef _DEBUG
	/**@def SHOW_MEMORY_LEAKS
	@brief W��cza do kodu wszystkie elementy niezb�dne do wykrywania wyciek�w
	*/
	#define SHOW_MEMORY_LEAKS
#endif
