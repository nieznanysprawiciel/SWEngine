#pragma once

/**
@file macros_switches.h
@brief Ten plik zawiera makrodefinicje bêd¹ce prze³¹cznikami miêdzy ró¿nymi opcjami kompilacji.
Wiêkszoœæ z tych makrodefinicji pozwala na wybór jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba bêdzie siê zdecydowaæ na któr¹œ z nich.*/

#include "types_definitions.h"

extern const unsigned int FRAMES_PER_SEC_UPDATE;	///<Co tyle sekund aktualizujemy frames_per_sec.

///@def _INTERPOLATE_POSITIONS
///@brief W³¹cza funkcjê interpoluj¹ca po³o¿enia obiektów.
#define _INTERPOLATE_POSITIONS

//#define _QUATERNION_SPEED			//@def _QUATERNION_SPEED
/*	Predkoœæ obrotowa jest wyra¿ona kwaternionem
Je¿eli to makro nie jest zdefiniowane, to prêdkoœæ obrotowa jest wyra¿ona jako wektor
4-elementowy, w którym pierwsze 3 sk³adowe opisuj¹ wektor obrotu, a sk³adowa w k¹t obrotu*/

///@def __TEST
///@brief W³¹cza funkcje testuj¹ce
#define __TEST
///@brief Ustawia w funkcji Engine::render_frame() sta³¹ liczbê klatek na sekundê
#define FIXED_FRAMES_COUNT

#define _SCALEABLE_OBJECTS			///umo¿liwia ustawienie skalowania dla meshy
/**@def ENGINE_MAX_TEXTURES
@brief Maksymalna liczba tekstur w silniku
*/

#define ENGINE_MAX_TEXTURES		8
#define ENGINE_MAX_LIGHTS		2


//#define INDEX_BUFFER_UINT16		// Definiujemy czy chcemy mieæ bufor indeksów 16-bitowy czy 32-bitowy
#define INDEX_BUFFER_UINT32			// Wybraæ tylko jedno, inaczej bêdzie b³¹d kompilacji

extern const unsigned int VERTICIES_MAX_COUNT;		///<Zawiera maksymalny rozmiar bufora indeksów, czyli jednoczeœnie liczbê wierzcho³ków mesha.


///@typedef VERT_INDEX
///@brief Definiujemy typ w buforze indeksów.

///W zale¿noœci od tego czy wybierzemy bufor 16 bitowy czy 32 bitowy VERT_INDEX jest ustawiany jako UINT16 lub UINT32.
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

#ifdef _DEBUG
	/**@def SHOW_MEMORY_LEAKS
	@brief W³¹cza do kodu wszystkie elementy niezbêdne do wykrywania wycieków
	*/
	#define SHOW_MEMORY_LEAKS
#endif