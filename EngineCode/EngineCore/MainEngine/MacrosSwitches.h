#pragma once
/**
@file macros_switches.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Ten plik zawiera makrodefinicje bêd¹ce prze³¹cznikami miêdzy ró¿nymi opcjami kompilacji.
Wiêkszoœæ z tych makrodefinicji pozwala na wybór jednej z opcji tylko tymczasowo,
w docelowej wersji silnika trzeba bêdzie siê zdecydowaæ na któr¹œ z nich.*/

#include "Common/TypesDefinitions.h"

#include "Common/RTTR.h"


///Co tyle sekund aktualizujemy m_framesPerSec.
static const unsigned int FRAMES_PER_SEC_UPDATE = 10;
static const float FIXED_MOVE_UPDATE_INTERVAL = ((float)1 / (float)56);	///<Interwa³, po którym nastêpuje kolejne przeliczenie po³o¿eñ obiektów (w sekundach).

///@def _INTERPOLATE_POSITIONS
///@brief W³¹cza do kodu funkcje interpoluj¹ca po³o¿enia obiektów.
#define _INTERPOLATE_POSITIONS

//#define _QUATERNION_SPEED			//@def _QUATERNION_SPEED
/*	Predkoœæ obrotowa jest wyra¿ona kwaternionem
Je¿eli to makro nie jest zdefiniowane, to prêdkoœæ obrotowa jest wyra¿ona jako wektor
4-elementowy, w którym pierwsze 3 sk³adowe opisuj¹ wektor obrotu, a sk³adowa w k¹t obrotu*/

///@def __TEST
///@brief W³¹cza funkcje testuj¹ce silnika.
///Funkcje testuj¹ce maj¹ na celu umo¿liwienie wczytywania modeli i ustawiania stanów obiektów,
///mimo ¿e nie s¹ stworzone docelowe mechanizmy, których ma u¿ywaæ silnik.
#define __TEST

#define _SCALEABLE_OBJECTS			///<Umo¿liwia ustawienie skalowania dla meshy.


/**@def ENGINE_MAX_LIGHTS
@brief Maksymalna liczba œwiate³ w silniku.
*/
#define ENGINE_MAX_LIGHTS		10



/**@brief Œcie¿ka do pliku, do którego bêd¹ trafia³y pomiary wydajnoœci.*/
static const char* PERFORMANCE_STATISTICS_FILE_PATH = "logs/performance.txt";



/**@def PERFORMANCE_CHECK
@brief W³¹cza funkcje klasy @ref PerformanceCheck do kodu silnika. Mierzy wydajnoœæ wybranych fragmentów kodu.
*/
#define PERFORMANCE_CHECK

#ifdef _DEBUG
	/**@def SHOW_MEMORY_LEAKS
	@brief W³¹cza do kodu wszystkie elementy niezbêdne do wykrywania wycieków
	*/
	#define SHOW_MEMORY_LEAKS
#endif
