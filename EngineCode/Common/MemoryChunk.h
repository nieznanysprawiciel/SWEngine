#pragma once

#include "Common/TypesDefinitions.h"

#include <vector>

/**@brief Klasa przechowuje pami�� o dowolnym przeznaczeniu.*/
class MemoryChunk
{
private:
protected:
	int8*			m_memory;
	uint32			m_size;
public:
	/**@brief Tworzy pusty obszar pami�ci.*/
	MemoryChunk()
	{
		m_memory = nullptr;
		m_size = 0;
	}

	/**@brief Przejmuje na w�asno�� podany obszar pami�ci.*/
	MemoryChunk( int8* dataPointer, uint32 dataSize )
		:	m_memory( dataPointer ),
			m_size( dataSize )
	{}
	template<typename Type>
	MemoryChunk( std::vector<Type>&& vector )
	{
		//vector.
	}

	/**Zwalnia pami�� po zasobach.*/
	~MemoryChunk()
	{
		delete[] m_memory;
	}

	MemoryChunk( const MemoryChunk& ) = delete;		///<Na razie nie mo�na wykonywa� kopii. Jedyna opcja to konstrukcja obiektu z semantyk� move.
	MemoryChunk( MemoryChunk&& chunk )				///<Tworzy obiekt kradn�c pami�� z podanego w parametrze obiektu.	
	{
		if( &chunk != this )
		{
			delete[] m_memory;
			m_memory = chunk.m_memory;
			m_size = chunk.m_size;
			chunk.m_size = 0;
			chunk.m_memory = nullptr;
		}
	}

	/**@brief Zwraca otypowany wska�nik na pami�� obiektu. @note Nie ma tu �adnej kontroli typ�w.*/
	template<typename Type>
	inline Type*			GetMemory		()		{ return m_memory; }
	inline uint32			GetMemorySize	()		{ return m_size; }			///<Zwraca rozmiar pami�ci przechowywanej w obiekcie.

	/**@brief Zwraca referencj� na obiekt w tablicy pod podanym indeksem.
	
	@attention Funkcja nie sprawdza czy indeks nie wychodzi poza pami�� w klasie.
	@param[in] index Indeks w tablicy. U�ywana jest arytmetyka wska�nikowa dla typu Type.*/
	template<typename Type>
	inline Type&			Get				( uint32 index )
	{	return static_cast<Type*>( m_memory )[ index ];		}
};


