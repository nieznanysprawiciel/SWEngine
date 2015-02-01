#pragma once

/*Ten plik zawiera makrodefinicje b�d�ce prze��cznikami mi�dzy r�nymi opcjami kompilacji.
Wi�kszo�� z tych makrodefinicji pozwala na wyb�r jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba b�dzie si� zdecydowa� na kt�r�� z nich.*/

#define _INTERPOLATE_POSITIONS		//w��cza funkcj� interpoluj�ca po�o�enia obiekt�w
//#define _QUATERNION_SPEED			//predko�� obrotowa jest wyra�ona kwaternionem
/*Je�eli to makro nie jest zdefiniowane, to pr�dko�� obrotowa jest wyra�ona jako wektor
4-elementowy, w kt�rym pierwsze 3 sk�adowe opisuj� wektor obrotu, a sk�adowa w k�t obrotu*/


//#define __TEST		//w��cza funkcje testuj�ce
#define FIXED_FRAMES_COUNT		//ustawia w funkcji Engine::render_frame() sta�� liczb� klatek na sekund�

#define _SCALEABLE_OBJECTS		//umo�liwia ustawienie skalowania dla meshy
#define ENGINE_MAX_TEXTURES		8	//maksymalna liczba tekstur w silniku
#define ENGINE_MAX_LIGHTS		2	//maksymalna liczba �wiate� w silniku



// Nazwy dla domy�lnych shader�w i materia�u
#define DEFAULT_VERTEX_SHADER_STRING	L"default_vertex_shader"
#define DEFAULT_PIXEL_SHADER_STRING		L"default_pixel_shader"
#define DEFAULT_MATERIAL_STRING			L"::default_material"


//#define INDEX_BUFFER_UINT16		// Definiujemy czy chcemy mie� bufor indeks�w 16-bitowy czy 32-bitowy
#define INDEX_BUFFER_UINT32			// Wybra� tylko jedno, inaczej b�dzie b��d kompilacji


// VERTICIES_MAX_COUNT zawiera sta��, kt�ra jest maksymaln� liczb� wierzcho�k�w w buforze
#if defined(INDEX_BUFFER_UINT16)
	#define VERTICIES_MAX_COUNT 0xFFFF
#elif defined(INDEX_BUFFER_UINT32)
	#define VERTICIES_MAX_COUNT 0xFFFFFFFF
#else
	#define VERTICIES_MAX_COUNT	0xFFFFFFFF
#endif

// Definiujemy typ w buforze indeks�w
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
	#define SHOW_MEMORY_LEAKS			// W��cza do kodu wszystkie elementy niezb�dne do wykrywania wyciek�w
#endif