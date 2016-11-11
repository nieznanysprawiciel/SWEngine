#pragma once

#include "ActorBase.h"
#include "GraphicAPI/MeshResources.h"
#include "EngineCore/Actors/Components/VisibleComponent.h"

#include <DirectXMath.h>


class Model3DFromFile;
class BufferObject;

/**@brief Klasa bazowa dla wszystkich obiekt�w statycznych w silniku.

Klasa StaticActor posiada komponenty odpowiedzialne za po�o�enie/orientacj� obiekt�w,
kolizje oraz elementy wizualne (mesh, mo�e na p�niejszym etapie animacja).

Obiekty posiadaj� po dwie zmienne na orientacj� i pozycj�. Przechowywana jest zawsze pozycja
z poprzedniej klatki, dzi�ki czemu mo�na interpolowa� po�o�enie. (Wy�wietlanie jest op�nione
w czasie o jedn� klatk�, �eby interpolacja nie musia�a wyprzedza� faktycznych po�o�e�).

Swapowanie nast�puj� w funkcji @ref DynamicActor::Move. @todo Trzeba zbada� czy nie ma przypadk�w,
w kt�rych nie b�dzie zachodzi� swapowanie, a powinno (wydaje mi si� �e przy niekt�rych kontrolerach)
i jako� rozwi�za� t� sytuacj�.

@ingroup Actors*/
class StaticActor : public ActorBase
{
	RTTR_ENABLE( ActorBase )
	RTTR_REGISTRATION_FRIEND
private:
	DirectX::XMFLOAT3		position;				///< Pozycja obiektu (lub bufor tylny)
	DirectX::XMFLOAT4		orientation;			///< Orientacja obiektu wyra�ona kwaternionem (lub bufor tylny)
	DirectX::XMFLOAT3		position_back;			///< Pozycja obiektu (lub bufor tylny)
	DirectX::XMFLOAT4		orientation_back;		///< Orientacja obiektu wyra�ona kwaternionem (lub bufor tylny)

	bool					swap_data;				///< Zmienna identyfikuj�ca, kt�re zmienne opisuj�ce po�o�enie s� u�ywane.

#ifdef _SCALEABLE_OBJECTS
private:
	float						scale;		///<Skalowanie wzgl�dem wszystkich osi.
public:
	void				SetScale		( float sc )	{ scale = sc; };
	float				GetScale		()				{ return scale; }
#endif
protected:
	
	VisibleComponent		m_visibleComponent;

protected:

	/** @brief Funkcja przemieszcza obiekt w podane miejsce.
	Warto�ci po�rednie miedzy podanymi po�o�eniami zostan� zinterpolowane.
	Nowa warto�� jest wpisywana do bufora tylnego pozycji.

	@param[in] pos Po�o�enie docelowe obiektu.
	*/
	inline void			SetPosition		( const DirectX::XMVECTOR pos );

	/** @brief Funkcja ustawia orientacj� obiektu. Warto�ci po�rednie s� interpolowane.
	
	Nowa warto�� jest wpisywana do bufora tylnego orientacji.

	@param[in] quaternion Orientacja docelowa obiektu.
	*/
	inline void			SetOrientation	( const DirectX::XMVECTOR quaternion );

public:
	explicit	StaticActor	();			///< Kontruktor domy�lny inicjuje obiekt w �rodku uk�adu wsp�rz�dnych.
	explicit	StaticActor	( const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT4& orient );	///< Inicjuje obiekt w podanym po�o�eniu
	explicit	StaticActor	( BufferObject* vertexBuffer, BufferObject* indexBuffer );
	virtual		~StaticActor();

	/**@brief Przemieszcza obiekt w podane miejsce.
	
	Funkcja powinna zosta� u�yta, je�eli obiekt ma zosta� przemiszczony skokowo w jakie� miejsce.
	Warto�ci po�rednie nie b�d� interpolowane. Funkcj� nale�y tak�e wywo�a� przy inicjacji obiektu.
	
	@param[in] pos Nowe po�o�enie obiektu.*/
	inline void			Teleport		( const DirectX::XMVECTOR pos );


	/**@brief Zmienia orientacj� obiektu na podan�.

	Funkcja powinna zosta� u�yta, je�eli obiekt ma zosta� obr�cony skokowo.
	Warto�ci po�rednie nie b�d� interpolowane. Funkcj� nale�y tak�e wywo�a� przy inicjacji obiektu.

	@param[in] quaternion Kwaternion nowego obrotu.*/
	inline void			TeleportOrientation			( const DirectX::XMVECTOR quaternion );


	/**@brief Pobiera aktualn� pozycj� obiektu.
	
	@return Wektor zawieraj�cy pozycj� obiektu.*/
	inline DirectX::XMVECTOR	GetPosition			() const;

	/**@brief Pobiera aktualn� orientacj� obiektu.

	@return Wektor zawieraj�cy orientacj� obiektu.*/
	inline DirectX::XMVECTOR	GetOrientation		() const;


	/**@brief Funkcja zamienia aktualne bufory na pozycj� i orientacj�.*/
	inline void					Swap				();

	DirectX::XMVECTOR			GetInterpolatedPosition		( float frame_percent ) const;
	DirectX::XMVECTOR			GetInterpolatedOrientation	( float frame_percent ) const;

	static ActorBase*			Create()	{ return new StaticActor; }

public:

	bool							SetModel		( const ResourcePtr< MeshAsset >& model );
	const ResourcePtr< MeshAsset >&	GetModel		()		{ return m_visibleComponent.GetAsset(); }

};



// ================================ //
//
inline void		StaticActor::SetPosition	( const DirectX::XMVECTOR pos )
{
	if ( swap_data )
		XMStoreFloat3( &position, pos );
	else
		XMStoreFloat3( &position_back, pos );
}

// ================================ //
//
inline void		StaticActor::SetOrientation	( const DirectX::XMVECTOR quaternion )
{
	if ( swap_data )
		XMStoreFloat4( &orientation, quaternion );
	else
		XMStoreFloat4( &orientation_back, quaternion );
}

// ================================ //
//
inline void		StaticActor::Teleport		( const DirectX::XMVECTOR pos )
{
	XMStoreFloat3( &position, pos ); 
	XMStoreFloat3( &position_back, pos );
}

// ================================ //
//
inline void		StaticActor::TeleportOrientation	( const DirectX::XMVECTOR quaternion )
{ 
	XMStoreFloat4( &orientation, quaternion ); 
	XMStoreFloat4( &orientation_back, quaternion );
}

// ================================ //
//
DirectX::XMVECTOR StaticActor::GetPosition() const
{
	if ( swap_data )
		return XMLoadFloat3( &position_back );
	else
		return XMLoadFloat3( &position );
}

// ================================ //
//
inline DirectX::XMVECTOR	StaticActor::GetOrientation		() const
{
	if ( swap_data )
		return XMLoadFloat4( &orientation_back );
	else
		return XMLoadFloat4( &orientation );
}

// ================================ //
//
inline void		StaticActor::Swap		()
{ 
	if ( swap_data )
		swap_data = false;
	else swap_data = true; 
}
