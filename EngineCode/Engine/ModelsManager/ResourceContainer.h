#pragma once

#include <map>

template <class TYPE>
class ResourceContainer
{
	friend class ModelsManager;
private:
	unsigned int count;

protected:
	std::map<std::wstring, TYPE> container;

	// Kasowanie obiekt�w
	int force_remove( const std::wstring& name );
	int force_remove( unsigned int id );
	void force_remove_all();
public:
	ResourceContainer();
	~ResourceContainer();

	// Kasowanie obiekt�w
	int remove( const std::wstring& name );
	int remove( unsigned int id );
	int remove_unused();

	// Dodawanie obiekt�w
	void add( const std::wstring& name, TYPE resource );

	// Dost�p do obiekt�w
	TYPE get( unsigned int id );
	inline unsigned int get_next_id() { return count; }

	inline TYPE get( const std::wstring& name )
	{
		auto iter = container.find( name );
		if ( iter != container.end() )
			return iter->second;
		return nullptr;
	}
};

template <class TYPE>
ResourceContainer<TYPE>::ResourceContainer()
{
	count = 1;
}

template <class TYPE>
ResourceContainer<TYPE>::~ResourceContainer( )
{
	for ( auto iter = container.begin( ); iter != container.end( ); iter++ )
	{
		delete iter->second;
	}
	container.clear();
}

template <class TYPE>
TYPE ResourceContainer<TYPE>::get( unsigned int id )
{
	for ( auto iter = container.begin(); iter != container.end(); iter++ )
	{
		if ( iter->second->get_id() == id )
			return iter->second;
	}
	return nullptr;
}

//-------------------------------------------------------------------------------//
//							dodawanie obiekt�w
//-------------------------------------------------------------------------------//

/*Dodaje element do kontanera.
Je�eli element ju� istnia�, to po prostu nie zostanie wstawiony, dlatego
przed uzyciem warto pobra� element o danej nazwie, �eby sprawdzi�
czy dodawanie jest konieczne.*/
template <class TYPE>
void ResourceContainer<TYPE>::add( const std::wstring& name, TYPE resource )
{
	auto new_element = make_pair<std::wstring, TYPE>( name, resource );
	container.insert( new_element );

	++count;			// Inkrementujemy licznik
}

//-------------------------------------------------------------------------------//
//							kasowanie obiekt�w
//-------------------------------------------------------------------------------//

/*Usuwa element o podanej nazwie, je�eli nie ma do niego odwo�a�.
Zwracana warto��:
0	-	w przypadku powodzenia
-1	-	nie znaleziono elementu
1	-	nie da si� usun��, bo jest w u�yciu*/
template <class TYPE>
int ResourceContainer<TYPE>::remove( const std::wstring& name )
{
	auto iter = container.find( name );
	if ( iter != container.end() )
		return -1;		// Nie znale�li�my elementu

	if ( !iter->second->can_delete() )
		return 1;		// Nie mo�emy skasowa�, bo s� odwo�ania

	delete iter->second;		// Zwalniamy pami�� spod wska�nika
	container.erase( iter );	// Kasujemy element z mapy

	return 0;			// Wychodzimy z powodzeniem
}


/*Usuwa element o podanym indeksie, je�eli nie ma do niego odwo�a�.
Zwracana warto��:
0	-	w przypadku powodzenia
-1	-	nie znaleziono elementu
1	-	nie da si� usun��, bo jest w u�yciu*/
template <class TYPE>
int ResourceContainer<TYPE>::remove( unsigned int id )
{
	for ( auto iter = container.begin( ); iter != container.end( ); iter++ )
	{
		if ( iter->second->get_id() == id )
		{
			// Sprawdzamy czy nie ma jakich� odwo�a� do obiektu
			if ( !iter->second->can_delete() )
				return 1;				// S� odwo�ania, wi�c nie kasujemy

			delete iter->second;		// Zwalniamy pami�� spod wska�nika
			container.erase( iter );	// Kasujemy element z mapy

			return 0;					// Zwracamy 0 jako powodzenie operacji
		}
	}
	return -1;		// Nie znale�li�my elementu
}

/* Kasuje wszystkie elementy w kontenerze, kt�re nie s� u�ywane przez
�aden obiekt. Kasowanie jest w pe�ni bezpieczne.

Zwraca liczbe usuni�tych element�w.*/
template <class TYPE>
int ResourceContainer<TYPE>::remove_unused()
{
	int count = 0;
	for ( auto iter = container.begin(); iter != container.end(); iter++ )
	{// Iterujemy po ca�ej mapie
		if ( iter->second->can_delete() )
		{
			// Mo�emy skasowa� obiekt, bo nikt go nie u�ywa
			delete iter->second;		// Zwalniamy pami�� spod wska�nika
			container.erase( iter );	// Kasujemy element z mapy

			++count;
		}
	}

	return count;
}


/* #protected
Wymusza skasowanie podanego elementu

Zwracana warto��:
0	-	w przypadku powodzenia
-1	-	nie znaleziono elementu*/
template <class TYPE>
int ResourceContainer<TYPE>::force_remove( const std::wstring& name )
{
	auto iter = container.find( name );
	if ( iter != container.end( ) )
		return -1;		// Nie znale�li�my elementu

	delete iter->second;		// Zwalniamy pami�� spod wska�nika
	return 0;
}

/* #protected
Wymusza skasowanie podanego elementu

Zwracana warto��:
0	-	w przypadku powodzenia
-1	-	nie znaleziono elementu*/
template <class TYPE>
int ResourceContainer<TYPE>::force_remove( unsigned int id )
{
	for ( auto iter = container.begin( ); iter != container.end( ); iter++ )
	{
		if ( iter->second->get_id( ) == id )
		{
			delete iter->second;		// Zwalniamy pami�� spod wska�nika
			return 0;
		}
	}
	return -1;		// Nie znaleziono elementu
}

/*Kasuje wszystkie elementy niezale�nie od tego czy by�y u�ywane,
a nast�pnie czy�ci map�.*/
template <class TYPE>
void ResourceContainer<TYPE>::force_remove_all( )
{
	for ( auto iter = container.begin(); iter != container.end(); iter++ )
	{// Iterujemy po ca�ej mapie
		delete iter->second;		// Zwalniamy pami�� spod wska�nika
	}
	container.clear();
}
