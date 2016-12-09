/**
@file PerformanceCheck.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "EngineCore/stdafx.h"
#include "PerformanceCheck.h"

#include <future>
#include <fstream>
#include <iomanip>
#include <limits>

///<@todo Don't use windows.h here. Build platform specific libraries.
#include <Windows.h>

#include "Common/MemoryLeaks.h"


#ifdef PERFORMANCE_CHECK

PerformanceCheck&		GetPerformanceCheck()
{
	static PerformanceCheck performanceCheck;
	return performanceCheck;
}

#endif

const int floatPrintPrecision = 8;		///< Liczba miejsc po przecinku jakie bêd¹ wyœwietlane dla floatów.

/**@brief Funkcja wywo³ywana na poczatku wykonywania zadania. Zapisuje
aktualny czas.

@param[in] index Indeks zadania w tablicy m_samples.*/
void			PerformanceCheck::StartPerformanceCheck( unsigned int index )
{
	if ( index < m_samples.size() )
	{
		_LARGE_INTEGER currentTime;
		QueryPerformanceCounter( &currentTime );
		m_samples[ index ].LastStartTime = currentTime.QuadPart;
	}
}

/**@brief Funkcja wywo³ywana pod koniec wykonywania zadania.
Aktualizuje wszystkie statystyki.

@param[in] index Indeks zadania w tablicy m_samples.*/
void			PerformanceCheck::EndPerformanceCheck( unsigned int index )
{
	if( index < m_samples.size() )
	{
		_LARGE_INTEGER currentTime;
		QueryPerformanceCounter( &currentTime );

		uint64 timeSpent = currentTime.QuadPart - m_samples[ index ].LastStartTime;
		m_samples[ index ].WholeTime += timeSpent;

		if( timeSpent > m_samples[ index ].MaxTime )
			m_samples[ index ].MaxTime = timeSpent;
		else if( timeSpent < m_samples[ index ].MinTime )
			m_samples[ index ].MinTime = timeSpent;

		m_samples[ index ].NumExecutions++;
	}
}

/**@brief Rejestruje nazwê zadania do pomiaru wydajnoœci.

@param[in] taskName Nazwa zadania
@return Zwraca identyfikator zadania, którym mo¿na siê odwo³ywaæ potem do funkcji mierz¹cych czas.*/
unsigned int	PerformanceCheck::RegisterTaskName( const char* taskName )
{
	_PerformanceData newTask;
	newTask.TaskName = taskName;
	newTask.MaxTime = 0;
	newTask.MinTime = std::numeric_limits< uint32 >::max();
	newTask.NumExecutions = 0;
	newTask.WholeTime = 0;

	m_samples.push_back( newTask );
	return (unsigned int)( m_samples.size() - 1 );
}

/**@brief Asynchronicznie drukuje statystyki do podanego pliku.

Poniewa¿ statystyki s¹ zapisywane w oddzielnym w¹tku, to zbyt szybkie wywo³ania tej funkcji po sobie
mog¹ spowodowaæ, ¿e na³o¿¹ siê one na siebie.

@param[in] outputFile Plik, do którego maj¹ trafiæ statystyki.*/
void			PerformanceCheck::PrintPerformanceStatisticsAsync( const std::string& outputFile )
{
	std::future<void> printAsync = std::async( Print, outputFile, m_samples );
}

/**@brief Faktyczna funkcja drukuj¹ca.*/
void			PerformanceCheck::Print( const std::string& outputFile, std::vector<_PerformanceData> data_copy )
{
	LARGE_INTEGER counterFreq;
	QueryPerformanceFrequency( &counterFreq );

	std::fstream output;
	output.open( outputFile.c_str(), std::fstream::out | std::fstream::app );
	if ( !output.good() )
		return;

	output << "Performance test:				Average			Minimum			Maximum" << std::endl;

	for ( auto taskData : data_copy )
	{
		output.width( 32 );
		output << std::left;
		output << taskData.TaskName;
		output.width( 0 );
		output << std::fixed;
		output << std::setprecision( floatPrintPrecision ) << 1000 * taskData.WholeTime / double( taskData.NumExecutions * counterFreq.QuadPart ) << " ms	";
		output << std::setprecision( floatPrintPrecision ) << 1000 * taskData.MinTime / double( counterFreq.QuadPart ) << " ms	";
		output << std::setprecision( floatPrintPrecision ) << 1000 * taskData.MaxTime / double( counterFreq.QuadPart ) << " ms" << std::endl;
	}
	output << std::endl;

	output.close();
}

// ================================ //
//
void			PerformanceCheck::ClearSamples()
{
	for( auto& sample : m_samples )
	{
		sample.MaxTime = 0;
		sample.MinTime = std::numeric_limits< uint32 >::max();
		sample.WholeTime = 0;
		sample.NumExecutions = 0;
	}
	
}
