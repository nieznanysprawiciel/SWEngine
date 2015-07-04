#include "stdafx.h"
#include "TimeManager.h"

#include "Common\memory_leaks.h"

/**@brief Kontruktor inicjuje warto�ci frames i frames_per_sec na 0.*/
TimeManager::TimeManager()
{
	frames = 0;
	frames_per_sec = 0;
	lag = 0.0;
	timer_frequency = 1;	// �eby unikn�� dzielenia przez zero, je�eli kto� nie zainicjuje timera.
}

/**@brief Funkcja wywo�ywana po zainicjowaniu silnika i przed wej�ciem
w p�tl� renderingu. Ustawia odpowiednie dane dla zegara, w tym cz�stotliwo�� oraz czas pocz�tkowy.*/
void TimeManager::initTimer()
{
	LARGE_INTEGER timeTemp;

	//Sprawdzamy cz�stotliwo�� timera systemowego, a potem inicjujemy licznik czasu
	QueryPerformanceFrequency( &timeTemp );
	timer_frequency = timeTemp.QuadPart;
	QueryPerformanceCounter( &timeTemp );		//inicjujemy licznik czasu
	time_previous = timeTemp.QuadPart;

	//FPS counter
	elapsed_time = 0;
	frames = 0;

	begin_time = time_previous;			// Poniewa� w�a�nie zainicjowali�my timer, to zapisujemy czas inicjacji.
}

/**@brief Funkcja oblicza interwa� czasowy jaki up�yn�� od ostatniej ramki.

Poza tym s� tu generowane eventy dotycz�ce czasu, op�nie� itp.
@return Zwraca interwa� jaki up�yn�� od ostatniego wywo�ania funkcji.*/
float TimeManager::onStartRenderFrame()
{
	int64 timeCurrent = queryCurrentTime();

	int64 timeDiff;
	timeDiff = timeCurrent - time_previous;
	float timeInterval = (float)timeDiff / timer_frequency;

	lag += timeInterval;

	//zliczanie FPS�w
	elapsed_time += timeDiff;
	if ( elapsed_time >= FRAMES_PER_SEC_UPDATE * timer_frequency )	//aktualizujemy co 10 sekund
	{
		frames_per_sec = (float)frames / FRAMES_PER_SEC_UPDATE;	//FRAMES_PER_SEC_UPDATE w sekundach
		elapsed_time = elapsed_time % ( FRAMES_PER_SEC_UPDATE * timer_frequency );
		frames = 0;		//zerujemy liczb� klatek
	}

	//todo:	generujemy eventy czasowe

	//zapisujemy obecny czas i wychodzimy z funkcji
	time_previous = timeCurrent;
	++frames;		//inkrementujemy licznik klatek
	return timeInterval;
}

/**@brief Wywo�uje QueryPerformanceCounter, �eby pobra� aktualny czas.

Nale�y pami�ta�, �e najcz�ciej interesuje nas czas aktualnej klatki albo lag.
Mo�na je pobra� funkcjami TimeManager::getTime i TimeManager::getTimeLag.
*/
int64 TimeManager::queryCurrentTime()
{
	LARGE_INTEGER time_temp;
	QueryPerformanceCounter( &time_temp );
	return time_temp.QuadPart;
}

/**@brief Zwraca czas w sekundach od w��czenia silnika.
*/
double TimeManager::queryTimeFromBegin()
{
	int64 currentTime = queryCurrentTime();
	int64 timeDiff = currentTime - begin_time;
	return (double)timeDiff / timer_frequency;
}
