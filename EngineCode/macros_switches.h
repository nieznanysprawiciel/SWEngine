#pragma once

/**
@file macros_switches.h
@brief Ten plik zawiera makrodefinicje bêd¹ce prze³¹cznikami miêdzy ró¿nymi opcjami kompilacji.
Wiêkszoœæ z tych makrodefinicji pozwala na wybór jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba bêdzie siê zdecydowaæ na któr¹œ z nich.*/

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
/**@def ENGINE_MAX_LIGHTS
@brief Maksymalna liczba œwiate³ w silniku
*/
#define ENGINE_MAX_LIGHTS		2



// Nazwy dla domyœlnych shaderów i materia³u
#define DEFAULT_VERTEX_SHADER_STRING	L"default_vertex_shader"
#define DEFAULT_PIXEL_SHADER_STRING		L"default_pixel_shader"
#define DEFAULT_MATERIAL_STRING			L"::default_material"


/**
@def DEFAULT_VERTEX_SHADER_STRING
@brief Nazwa domyœlnego vertex shadera w ModelsManager.
@def DEFAULT_PIXEL_SHADER_STRING
@brief Nazwa domyœlnego pixel shadera w ModelsManager.
@def DEFAULT_MATERIAL_STRING
@brief Nazwa domyœlnego materia³u w ModelsManager.
*/

#define DEFAULT_VERTEX_SHADER_ENTRY		"VS"
#define DEFAULT_PIXEL_SHADER_ENTRY		"PS"


//#define INDEX_BUFFER_UINT16		// Definiujemy czy chcemy mieæ bufor indeksów 16-bitowy czy 32-bitowy
#define INDEX_BUFFER_UINT32			// Wybraæ tylko jedno, inaczej bêdzie b³¹d kompilacji

///@def VERTICIES_MAX_COUNT
///@brief VERTICIES_MAX_COUNT zawiera sta³¹, która jest maksymaln¹ liczb¹ wierzcho³ków w buforze
#if defined(INDEX_BUFFER_UINT16)
	#define VERTICIES_MAX_COUNT 0xFFFF
#elif defined(INDEX_BUFFER_UINT32)
	#define VERTICIES_MAX_COUNT 0xFFFFFFFF
#else
	#define VERTICIES_MAX_COUNT	0xFFFFFFFF
#endif

///@typedef VERT_INDEX
///@brief Definiujemy typ w buforze indeksów.

///W zale¿noœci od tego czy wybierzemy bufor 16 bitowy czy 32 bitowy VERT_INDEX jest ustawiany jako UINT16 lub UINT32.
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
	@brief W³¹cza do kodu wszystkie elementy niezbêdne do wykrywania wycieków
	*/
	#define SHOW_MEMORY_LEAKS
#endif