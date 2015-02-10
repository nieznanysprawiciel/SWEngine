#pragma once

/**
@file macros_switches.h
@brief Ten plik zawiera makrodefinicje b�d�ce prze��cznikami mi�dzy r�nymi opcjami kompilacji.
Wi�kszo�� z tych makrodefinicji pozwala na wyb�r jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba b�dzie si� zdecydowa� na kt�r�� z nich.*/

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
///@brief Ustawia w funkcji Engine::render_frame() sta�� liczb� klatek na sekund�
#define FIXED_FRAMES_COUNT

#define _SCALEABLE_OBJECTS			///umo�liwia ustawienie skalowania dla meshy
/**@def ENGINE_MAX_TEXTURES
@brief Maksymalna liczba tekstur w silniku
*/
#define ENGINE_MAX_TEXTURES		8
/**@def ENGINE_MAX_LIGHTS
@brief Maksymalna liczba �wiate� w silniku
*/
#define ENGINE_MAX_LIGHTS		2



// Nazwy dla domy�lnych shader�w i materia�u
#define DEFAULT_VERTEX_SHADER_STRING	L"default_vertex_shader"
#define DEFAULT_PIXEL_SHADER_STRING		L"default_pixel_shader"
#define DEFAULT_MATERIAL_STRING			L"::default_material"


/**
@def DEFAULT_VERTEX_SHADER_STRING
@brief Nazwa domy�lnego vertex shadera w ModelsManager.
@def DEFAULT_PIXEL_SHADER_STRING
@brief Nazwa domy�lnego pixel shadera w ModelsManager.
@def DEFAULT_MATERIAL_STRING
@brief Nazwa domy�lnego materia�u w ModelsManager.
*/

#define DEFAULT_VERTEX_SHADER_ENTRY		"VS"
#define DEFAULT_PIXEL_SHADER_ENTRY		"PS"


//#define INDEX_BUFFER_UINT16		// Definiujemy czy chcemy mie� bufor indeks�w 16-bitowy czy 32-bitowy
#define INDEX_BUFFER_UINT32			// Wybra� tylko jedno, inaczej b�dzie b��d kompilacji

///@def VERTICIES_MAX_COUNT
///@brief VERTICIES_MAX_COUNT zawiera sta��, kt�ra jest maksymaln� liczb� wierzcho�k�w w buforze
#if defined(INDEX_BUFFER_UINT16)
	#define VERTICIES_MAX_COUNT 0xFFFF
#elif defined(INDEX_BUFFER_UINT32)
	#define VERTICIES_MAX_COUNT 0xFFFFFFFF
#else
	#define VERTICIES_MAX_COUNT	0xFFFFFFFF
#endif

///@typedef VERT_INDEX
///@brief Definiujemy typ w buforze indeks�w.

///W zale�no�ci od tego czy wybierzemy bufor 16 bitowy czy 32 bitowy VERT_INDEX jest ustawiany jako UINT16 lub UINT32.
#if defined(INDEX_BUFFER_UINT16)
	typedef UINT16 VERT_INDEX;
	#define INDEX_BUFFER_FORMAT DXGI_FORMAT_R16_UINT
#elif defined(INDEX_BUFFER_UINT32)
	typedef UINT32 VERT_INDEX;
	#define INDEX_BUFFER_FORMAT DXGI_FORMAT_R32_UINT
#else
	typedef UINT32 VERT_INDEX;
	#define INDEX_BUFFER_FORMAT DXGI_FORMAT_R32_UINT
#endif

#ifdef _DEBUG
	/**@def SHOW_MEMORY_LEAKS
	@brief W��cza do kodu wszystkie elementy niezb�dne do wykrywania wyciek�w
	*/
	#define SHOW_MEMORY_LEAKS
#endif