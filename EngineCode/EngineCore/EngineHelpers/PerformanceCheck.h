#pragma once
/**@file PerformanceCheck.h
@author nieznanysprawiciel
@copyright Plik jest cz�ci� silnika graficznego SWEngine.

Zawiera deklaracj� klasy PerformanceCheck, kt�ra s�u�y do pomiaru 
czasu zu�ywanego na wykonanie wybranych zada�.*/

#include <string>
#include <vector>
#include "Common/TypesDefinitions.h"



/**@brief Struktura przechowuje dane na temat wydajno�ci jednej badanej rzeczy.*/
struct _PerformanceData
{
	uint64				LastStartTime;				///<Ostatni zarejestrowany czas dla badanej zadania.
	uint64				WholeTime;					///<Zlicza ca�y czas jaki zosta� sp�dzony na wykonywaniu zadania.
	uint64				MinTime;					///<Minimalny czas wyknywania zadania.
	uint64				MaxTime;					///<Maksymalny czas wykonywania zadania.
	uint32				NumExecutions;				///<Ile razy zosta�o wykonane zadanie - ilo�� parzystych wywo�a� StartPerformanceCheck i EndPerformanceCheck.
	std::string			TaskName;					///<Nazwa zadania jaka zostanie wy�wietlona w statystykach.
};

/**@brief Klasa s�u�y do pomiaru wydajno�ci poszczeg�lnych fragment�w kodu.

Spos�b u�ycia klasy:
S� dwie metody. Mo�na normalnie korzysta� z klasy jak ze zwyk�ego obiektu i wywo�ywa� jej funkcje
lub mo�na u�y� zamiast tego specjalnie zdefiniowanych makr. W drugim przypadku kod s�u��cy do pomiaru b�dzie
za��czany jedynie w przypadku zdefiniowania makra PERFORMANCE_CHECK. W innym przypadku wszystkie u�yte makra b�d�
rozwija�y si� do pustych instrukcji.

1. Normalne u�ywanie klasy.
Aby u�y� klasy nale�y zarejestrowa� nazwy wybranych zada�. Robi si� to funkcj� RegisterTaskName.
Funkcja zwraca identyfikator, kt�ry potem pos�u�y do wywo�ywania pomiar�w dla tego zadania.

Ka�de zadanie musi by� otoczone wywo�aniami funkcji StartPerformanceCheck i EndPerformanceCheck z podaniem indeksu zadania.

Aby wypisa� wyniki, nale�y wywo�a� funkcj� printPerformanceStatisticsAsync podaj�c nazw� pliku, do kt�rego statystyki zostan� wys�ane.
Funkcja dzia�a asynchronicznie, wi�c nie czeka si� a� plik zostanie zapisany, tylko wykonuje kod silnika dalej.

\code
#include PerformanceCheck.h

unsigned int taskID;
PerformanceCheck stats;

myClass::myClass()
{
	taskID = stats.RegisterTaskName("TaskName");
}

void timer()
{
	stats.StartPerformanceCheck( taskID );

	// Tutaj kod, kt�rego wydajno�� mierzymy

	stats.EndPerformanceCheck( taskID );
}

\endcode

2. U�ytkowanie klasy z u�yciem makr.
Aby u�y� klasy nale�y zdefiniowa� makro PERFORMANCE_CHECK przed zaincludowaniem nag��wka.
Makro to robi dwie rzeczy:
- w��cza definicje pozosta�ych potrzebnych makr
- za��cza kod tworz�cy obiekt tej klasy (obiekt jest zmienn� globaln� w pliku PerformanceCheck.cpp)

Nast�pnie trzeba zadeklarowa� ch�� u�ycia pomiaru dla jakiego� zadania wywo�uj�� makro REGISTER_PERFORMANCE_CHECK i podaj�c nazw�
zadania. Makro powinno zosta� wywo�ane globalnie, w tej samej przestrzeni nazw, w jakiej b�dzie wywo�ywana funkcja do pomiaru czasu.

Nazwa powinna by� pojedynczym ci�giem znak�w. NIE mo�e by� w cudzys�owie.

W miejscu pomiaru nale�y wywo�a� parami makra START_PERFORMANCE_CHECK i END_PERFORMANCE_CHECK podaj�c w parametrze
taki sam ci�g znak�w, jakim zosta�o zarejestrowane zadanie.

Wypisanie statystyk robi si� za pomoc� makra PRINT_STATISTICS i podaje nazw� pliku.
@attention Nazwa pliku jest w cudzys�owie w przeciwie�stwie do innych miejsc, w kt�rych wywo�uje si� makra.

\code
#include PerformanceCheck.h

REGISTER_PERFORMANCE_CHECK( TASK_NAME )

void FunctionToProfile()
{
	START_PERFORMANCE_CHECK( TASK_NAME )

// Tutaj kod, kt�rego wydajno�� mierzymy

	END_PERFORMANCE_CHECK( TASK_NAME )
}

\endcode

*/
class PerformanceCheck
{
private:
	std::vector<_PerformanceData>		m_samples;		///<Zapisane dane na temat poszczeg�lnych zada�, dla kt�rych mierzymy wydajno��.

public:
	PerformanceCheck() = default;
	~PerformanceCheck() = default;

	void				StartPerformanceCheck	( unsigned int index );
	void				EndPerformanceCheck		( unsigned int index );
	void				ClearSamples			();

	unsigned int		RegisterTaskName		( const char* taskName );

	void				PrintPerformanceStatisticsAsync	( const std::string& outputFile );

private:
	static void			Print					( const std::string& outputFile, std::vector<_PerformanceData> data_copy );
};


#ifdef PERFORMANCE_CHECK

PerformanceCheck&		GetPerformanceCheck();

#define REGISTER_PERFORMANCE_CHECK( name ) static unsigned int __performance_task_id ## name = GetPerformanceCheck().RegisterTaskName( # name );

#define START_PERFORMANCE_CHECK( x )	GetPerformanceCheck().StartPerformanceCheck( __performance_task_id ## x );
#define END_PERFORMANCE_CHECK( x )		GetPerformanceCheck().EndPerformanceCheck( __performance_task_id ## x );
#define PRINT_STATISTICS( s )			GetPerformanceCheck().PrintPerformanceStatisticsAsync( s );
#define PERFORMACE_CHECK_CLEAR_SAMPLES	GetPerformanceCheck().ClearSamples();

#else

#define START_PERFORMANCE_CHECK( x )
#define END_PERFORMANCE_CHECK( x )
#define REGISTER_PERFORMANCE_CHECK( x )
#define PRINT_STATISTICS( s )
#define PERFORMACE_CHECK_CLEAR 

#endif
