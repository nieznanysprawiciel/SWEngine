#pragma once

/*Ten plik zawiera makrodefinicje bêd¹ce prze³¹cznikami miêdzy ró¿nymi opcjami kompilacji.
Wiêkszoœæ z tych makrodefinicji pozwala na wybór jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba bêdzie siê zdecydowaæ na któr¹œ z nich.*/

#define _INTERPOLATE_POSITIONS		//w³¹cza funkcjê interpoluj¹ca po³o¿enia obiektów
//#define _QUATERNION_SPEED			//predkoœæ obrotowa jest wyra¿ona kwaternionem
/*Je¿eli to makro nie jest zdefiniowane, to prêdkoœæ obrotowa jest wyra¿ona jako wektor
4-elementowy, w którym pierwsze 3 sk³adowe opisuj¹ wektor obrotu, a sk³adowa w k¹t obrotu*/


//#define __TEST		//w³¹cza funkcje testuj¹ce
#define FIXED_FRAMES_COUNT		//ustawia w funkcji Engine::render_frame() sta³¹ liczbê klatek na sekundê

#define _SCALEABLE_OBJECTS		//umo¿liwia ustawienie skalowania dla meshy
#define ENGINE_MAX_TEXTURES		8	//maksymalna liczba tekstur w silniku
#define ENGINE_MAX_LIGHTS		2	//maksymalna liczba œwiate³ w silniku



// Nazwy dla domyœlnych shaderów i materia³u
#define DEFAULT_VERTEX_SHADER_STRING	L"default_vertex_shader"
#define DEFAULT_PIXEL_SHADER_STRING		L"default_pixel_shader"
#define DEFAULT_MATERIAL_STRING			L"::default_material"


//#define INDEX_BUFFER_UINT16		// Definiujemy czy chcemy mieæ bufor indeksów 16-bitowy czy 32-bitowy
#define INDEX_BUFFER_UINT32			// Wybraæ tylko jedno, inaczej bêdzie b³¹d kompilacji


// VERTICIES_MAX_COUNT zawiera sta³¹, która jest maksymaln¹ liczb¹ wierzcho³ków w buforze
#if defined(INDEX_BUFFER_UINT16)
	#define VERTICIES_MAX_COUNT 0xFFFF
#elif defined(INDEX_BUFFER_UINT32)
	#define VERTICIES_MAX_COUNT 0xFFFFFFFF
#else
	#define VERTICIES_MAX_COUNT	0xFFFFFFFF
#endif

// Definiujemy typ w buforze indeksów
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
	#define SHOW_MEMORY_LEAKS			// W³¹cza do kodu wszystkie elementy niezbêdne do wykrywania wycieków
#endif