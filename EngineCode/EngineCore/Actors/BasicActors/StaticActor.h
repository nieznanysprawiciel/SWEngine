#pragma once

#include "EngineObject.h"

#include <DirectXMath.h>


/**@brief Klasa bazowa dla wszystkich obiektów statycznych w silniku.

Obiekty posiadaj¹ po dwie zmienne na orientacjê i pozycjê. Przechowywana jest zawsze pozycja
z poprzedniej klatki, dziêki czemu mo¿na interpolowaæ po³o¿enie. (Wyœwietlanie jest opóŸnione
w czasie o jedn¹ klatkê, ¿eby interpolacja nie musia³a wyprzedzaæ faktycznych po³o¿eñ).

Swapowanie nastêpujê w funkcji @ref DynamicActor::Move. @todo Trzeba zbadaæ czy nie ma przypadków,
w których nie bêdzie zachodziæ swapowanie, a powinno (wydaje mi siê ¿e przy niektórych kontrolerach)
i jakoœ rozwi¹zaæ tê sytuacjê.
*/
class StaticActor : public EngineObject
{
	RTTR_ENABLE( EngineObject )
	RTTR_REGISTRATION_FRIEND
private:
	DirectX::XMFLOAT3		position;				///< Pozycja obiektu (lub bufor tylny)
	DirectX::XMFLOAT4		orientation;			///< Orientacja obiektu wyra¿ona kwaternionem (lub bufor tylny)
	DirectX::XMFLOAT3		position_back;			///< Pozycja obiektu (lub bufor tylny)
	DirectX::XMFLOAT4		orientation_back;		///< Orientacja obiektu wyra¿ona kwaternionem (lub bufor tylny)

	bool					swap_data;	///< Zmienna identyfikuj¹ca, które zmienne opisuj¹ce po³o¿enie s¹ u¿ywane.
protected:

	/** @brief Funkcja przemieszcza obiekt w podane miejsce.
	Wartoœci poœrednie miedzy podanymi po³o¿eniami zostan¹ zinterpolowane.
	Nowa wartoœæ jest wpisywana do bufora tylnego pozycji.

	@param[in] pos Po³o¿enie docelowe obiektu.
	*/
	void SetPosition( const DirectX::XMVECTOR& pos )
	{
		if ( swap_data )
			XMStoreFloat3( &position, pos );
		else
			XMStoreFloat3( &position_back, pos );
	}

	/** @brief Funkcja ustawia orientacjê obiektu. Wartoœci poœrednie s¹ interpolowane.
	
	Nowa wartoœæ jest wpisywana do bufora tylnego orientacji.

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
	StaticActor();			///< Kontruktor domyœlny inicjuje obiekt w œrodku uk³adu wspó³rzêdnych.
	StaticActor( const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT4& orient );	///< Inicjuje obiekt w podanym po³o¿eniu

	/**@brief Przemieszcza obiekt w podane miejsce.
	
	Funkcja powinna zostaæ u¿yta, je¿eli obiekt ma zostaæ przemiszczony skokowo w jakieœ miejsce.
	Wartoœci poœrednie nie bêd¹ interpolowane. Funkcjê nale¿y tak¿e wywo³aæ przy inicjacji obiektu.
	
	@param[in] pos Nowe po³o¿enie obiektu.*/
	void Teleport						( const DirectX::XMVECTOR& pos )
	{ XMStoreFloat3( &position, pos ); XMStoreFloat3( &position_back, pos ); }

	/**@brief Zmienia orientacjê obiektu na podan¹.

	Funkcja powinna zostaæ u¿yta, je¿eli obiekt ma zostaæ obrócony skokowo.
	Wartoœci poœrednie nie bêd¹ interpolowane. Funkcjê nale¿y tak¿e wywo³aæ przy inicjacji obiektu.

	@param[in] quaternion Kwaternion nowego obrotu.*/
	void TeleportOrientation			( const DirectX::XMVECTOR& quaternion )
	{ XMStoreFloat4( &orientation, quaternion ); XMStoreFloat4( &orientation_back, quaternion ); }


	/**@brief Pobiera aktualn¹ pozycjê obiektu.
	
	@return Wektor zawieraj¹cy pozycjê obiektu.*/
	DirectX::XMVECTOR GetPosition() const
	{
		if ( swap_data )
			return XMLoadFloat3( &position_back );
		else
			return XMLoadFloat3( &position );
	}

	/**@brief Pobiera aktualn¹ orientacjê obiektu.

	@return Wektor zawieraj¹cy orientacjê obiektu.*/
	DirectX::XMVECTOR GetOrientation() const
	{
		if ( swap_data )
			return XMLoadFloat4( &orientation_back );
		else
			return XMLoadFloat4( &orientation );
	}

	/**@brief Funkcja zamienia aktualne bufory na pozycjê i orientacjê.*/
	void swap() { if ( swap_data ) swap_data = false; else swap_data = true; }

	DirectX::XMVECTOR		GetInterpolatedPosition		( float frame_percent ) const;
	DirectX::XMVECTOR		GetInterpolatedOrientation	( float frame_percent ) const;

	static EngineObject*			Create()	{ return new StaticActor; }
};


