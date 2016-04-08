#pragma once

#include "EngineObject.h"

#include <DirectXMath.h>


/**@brief Klasa bazowa dla wszystkich obiekt�w statycznych w silniku.

Obiekty posiadaj� po dwie zmienne na orientacj� i pozycj�. Przechowywana jest zawsze pozycja
z poprzedniej klatki, dzi�ki czemu mo�na interpolowa� po�o�enie. (Wy�wietlanie jest op�nione
w czasie o jedn� klatk�, �eby interpolacja nie musia�a wyprzedza� faktycznych po�o�e�).

Swapowanie nast�puj� w funkcji @ref DynamicActor::Move. @todo Trzeba zbada� czy nie ma przypadk�w,
w kt�rych nie b�dzie zachodzi� swapowanie, a powinno (wydaje mi si� �e przy niekt�rych kontrolerach)
i jako� rozwi�za� t� sytuacj�.
*/
class StaticActor : public EngineObject
{
	RTTR_ENABLE( EngineObject )
	RTTR_REGISTRATION_FRIEND
private:
	DirectX::XMFLOAT3		position;				///< Pozycja obiektu (lub bufor tylny)
	DirectX::XMFLOAT4		orientation;			///< Orientacja obiektu wyra�ona kwaternionem (lub bufor tylny)
	DirectX::XMFLOAT3		position_back;			///< Pozycja obiektu (lub bufor tylny)
	DirectX::XMFLOAT4		orientation_back;		///< Orientacja obiektu wyra�ona kwaternionem (lub bufor tylny)

	bool					swap_data;	///< Zmienna identyfikuj�ca, kt�re zmienne opisuj�ce po�o�enie s� u�ywane.
protected:

	/** @brief Funkcja przemieszcza obiekt w podane miejsce.
	Warto�ci po�rednie miedzy podanymi po�o�eniami zostan� zinterpolowane.
	Nowa warto�� jest wpisywana do bufora tylnego pozycji.

	@param[in] pos Po�o�enie docelowe obiektu.
	*/
	void SetPosition( const DirectX::XMVECTOR& pos )
	{
		if ( swap_data )
			XMStoreFloat3( &position, pos );
		else
			XMStoreFloat3( &position_back, pos );
	}

	/** @brief Funkcja ustawia orientacj� obiektu. Warto�ci po�rednie s� interpolowane.
	
	Nowa warto�� jest wpisywana do bufora tylnego orientacji.

	@param[in] quaternion Orientacja docelowa obiektu.
	*/
	void SetOrientation( const DirectX::XMVECTOR& quaternion )
	{
		if ( swap_data )
			XMStoreFloat4( &orientation, quaternion );
		else
			XMStoreFloat4( &orientation_back, quaternion );
	}
public:
	StaticActor();			///< Kontruktor domy�lny inicjuje obiekt w �rodku uk�adu wsp�rz�dnych.
	StaticActor( const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT4& orient );	///< Inicjuje obiekt w podanym po�o�eniu

	/**@brief Przemieszcza obiekt w podane miejsce.
	
	Funkcja powinna zosta� u�yta, je�eli obiekt ma zosta� przemiszczony skokowo w jakie� miejsce.
	Warto�ci po�rednie nie b�d� interpolowane. Funkcj� nale�y tak�e wywo�a� przy inicjacji obiektu.
	
	@param[in] pos Nowe po�o�enie obiektu.*/
	void Teleport						( const DirectX::XMVECTOR& pos )
	{ XMStoreFloat3( &position, pos ); XMStoreFloat3( &position_back, pos ); }

	/**@brief Zmienia orientacj� obiektu na podan�.

	Funkcja powinna zosta� u�yta, je�eli obiekt ma zosta� obr�cony skokowo.
	Warto�ci po�rednie nie b�d� interpolowane. Funkcj� nale�y tak�e wywo�a� przy inicjacji obiektu.

	@param[in] quaternion Kwaternion nowego obrotu.*/
	void TeleportOrientation			( const DirectX::XMVECTOR& quaternion )
	{ XMStoreFloat4( &orientation, quaternion ); XMStoreFloat4( &orientation_back, quaternion ); }


	/**@brief Pobiera aktualn� pozycj� obiektu.
	
	@return Wektor zawieraj�cy pozycj� obiektu.*/
	DirectX::XMVECTOR GetPosition() const
	{
		if ( swap_data )
			return XMLoadFloat3( &position_back );
		else
			return XMLoadFloat3( &position );
	}

	/**@brief Pobiera aktualn� orientacj� obiektu.

	@return Wektor zawieraj�cy orientacj� obiektu.*/
	DirectX::XMVECTOR GetOrientation() const
	{
		if ( swap_data )
			return XMLoadFloat4( &orientation_back );
		else
			return XMLoadFloat4( &orientation );
	}

	/**@brief Funkcja zamienia aktualne bufory na pozycj� i orientacj�.*/
	void swap() { if ( swap_data ) swap_data = false; else swap_data = true; }

	DirectX::XMVECTOR		GetInterpolatedPosition		( float frame_percent ) const;
	DirectX::XMVECTOR		GetInterpolatedOrientation	( float frame_percent ) const;

	static EngineObject*			Create()	{ return new StaticActor; }
};


