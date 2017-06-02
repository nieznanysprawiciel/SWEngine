#pragma once
/**@file TimeManager.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

@brief Plik zawiera deklaracj� klasy TimeManager do obs�ugi rzeczy zwi�zanych z czasem.*/

#include "swCommonLib/Common/TypesDefinitions.h"

/**@brief Klasa obs�uguje wszystko co zwi�zane z czasem.

Do zada� klasy nale�y:
- pobieranie i przeliczanie czasu
- obs�uga event�w czasowych
- Robienie statystyk wykonania poszczeg�lnych fragment�w kodu

Funkcje zaczynaj�ce si� od query pobieraj� czas z systemu.
Funkcje get zwracaj� warto�ci obliczone wcze�niej i przechowywane w klasie.
Tak naprawd� powinno si� raczej korzysta� z funkcji get, poniewa� w silniku nie liczy si�
czas aktualny, ale czas jaki jest w danej klatce.*/
class TimeManager
{
private:
	//time control
	int64						m_beginTime;		///<Zawiera czas rozpocz�cia dzia�ania aplikacji. (Tak dok�adniej czas wej�cia w p�tl� renderingu. Wcze�nie odbywa si� wczytywanie Levelu, co mo�e trwa�.)

	int64						m_timerFrequency;	///<Cz�stotliwo�� timera
	int64						m_timePrevious;		///<B�dziemy mierzy� r�nic� miedzy czasami w kolejnych klatkach.
	int64						m_elapsedTime;		///<Do licznika FPS�w, czas od ostatniej aktualizacji m_framesPerSec
	float						m_lag;				///<Op�nienie wzgl�dem normalego tempa updatowania sceny
	unsigned int				m_frames;			///<Zlicza ramki od ostatniego wyzerowania
	float						m_framesPerSec;		///<Najwa�niejszy licznik w grze
public:
	TimeManager();
	~TimeManager() = default;

	void			InitTimer();
	float			onStartRenderFrame();
	int64			QueryCurrentTime();
	double			QueryTimeFromBegin();
	float			GetCurrentLag();
	inline int64	GetBeginTime()					{ return m_beginTime; }				///<Zwraca czas jaki zosta� zarejestrowany przy inicjacji silnika.
	inline int64	GetTimerFrequency()				{ return m_timerFrequency; }		///<Zwraca cz�stotliwo�� taktowania uzywanego timera.
	inline float	GetFrameLag()					{ return m_lag; }					///<Zwraca zmienn� m_lag.
	inline int64	GetTime()						{ return m_timePrevious; }			///<Zwraca czas, jaki zosta� ostatnio zarejestrowany przy wywo�aniu onStartRenderFrame.
	inline void		UpdateTimeLag( float timeLag )	{ m_lag = timeLag; }				///<Aktualizuje zmienn� m_lag.
};

