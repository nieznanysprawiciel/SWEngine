#include "EngineCore/stdafx.h"
#include "TimeManager.h"

#include "Common/memory_leaks.h"

/**@brief Kontruktor inicjuje warto�ci m_frames i m_framesPerSec na 0.*/
TimeManager::TimeManager()
{
	m_frames = 0;
	m_framesPerSec = 0;
	m_lag = 0.0;
	m_timerFrequency = 1;	// �eby unikn�� dzielenia przez zero, je�eli kto� nie zainicjuje timera.
}

/**@brief Funkcja wywo�ywana po zainicjowaniu silnika i przed wej�ciem
w p�tl� renderingu. Ustawia odpowiednie dane dla zegara, w tym cz�stotliwo�� oraz czas pocz�tkowy.*/
void TimeManager::InitTimer()
{
	LARGE_INTEGER timeTemp;

	//Sprawdzamy cz�stotliwo�� timera systemowego, a potem inicjujemy licznik czasu
	QueryPerformanceFrequency( &timeTemp );
	m_timerFrequency = timeTemp.QuadPart;
	QueryPerformanceCounter( &timeTemp );		//inicjujemy licznik czasu
	m_timePrevious = timeTemp.QuadPart;

	//FPS counter
	m_elapsedTime = 0;
	m_frames = 0;

	m_beginTime = m_timePrevious;			// Poniewa� w�a�nie zainicjowali�my timer, to zapisujemy czas inicjacji.
}

/**@brief Funkcja oblicza interwa� czasowy jaki up�yn�� od ostatniej ramki.

Poza tym s� tu generowane eventy dotycz�ce czasu, op�nie� itp.
@return Zwraca interwa� jaki up�yn�� od ostatniego wywo�ania funkcji.*/
float TimeManager::onStartRenderFrame()
{
	int64 timeCurrent = QueryCurrentTime();

	int64 timeDiff;
	timeDiff = timeCurrent - m_timePrevious;
	float timeInterval = (float)timeDiff / m_timerFrequency;

	m_lag += timeInterval;

	//zliczanie FPS�w
	m_elapsedTime += timeDiff;
	if ( m_elapsedTime >= FRAMES_PER_SEC_UPDATE * m_timerFrequency )	//aktualizujemy co 10 sekund
	{
		m_framesPerSec = (float)m_frames / FRAMES_PER_SEC_UPDATE;	//FRAMES_PER_SEC_UPDATE w sekundach
		m_elapsedTime = m_elapsedTime % ( FRAMES_PER_SEC_UPDATE * m_timerFrequency );
		m_frames = 0;		//zerujemy liczb� klatek
	}

	//todo:	generujemy eventy czasowe

	//zapisujemy obecny czas i wychodzimy z funkcji
	m_timePrevious = timeCurrent;
	++m_frames;		//inkrementujemy licznik klatek
	return timeInterval;
}

/**@brief Wywo�uje QueryPerformanceCounter, �eby pobra� aktualny czas.

Nale�y pami�ta�, �e najcz�ciej interesuje nas czas aktualnej klatki albo lag.
Mo�na je pobra� funkcjami TimeManager::GetTime i TimeManager::GetFrameLag.

Ta funkcja pobiera aktualny czas nie�ny od wy�wietlania klatek.
*/
int64 TimeManager::QueryCurrentTime()
{
	LARGE_INTEGER time_temp;
	QueryPerformanceCounter( &time_temp );
	return time_temp.QuadPart;
}

/**@brief Zwraca czas w sekundach od w��czenia silnika.

Podawany jest aktualny czas, a nie czas zapisany na pocz�tku klatki.
*/
double TimeManager::QueryTimeFromBegin()
{
	int64 currentTime = QueryCurrentTime();
	int64 timeDiff = currentTime - m_beginTime;
	return (double)timeDiff / m_timerFrequency;
}

float TimeManager::GetCurrentLag()
{
	int64 timeCurrent = QueryCurrentTime();
	
	int64 timeDiff;
	timeDiff = timeCurrent - m_timePrevious;
	float timeInterval = (float)timeDiff / m_timerFrequency;

	return m_lag + timeInterval;
}
