#pragma once

/**@file HosekSkyDome.h
@brief Plik zawiera deklaracj� klasy HosekSkyDome, do wy�wietlania nieba
u�ywaj�c modelu Hodeka i Wilkiego.*/

#include "EngineCore/Actors/Extended/SkyDome.h"
#include "HosekSkyModel.h"



namespace sw
{


/**@brief Struktura wierzcho�ka u�ywana przez klas� HosekSkyDome.

Kolor wierzcho�ka jest ustalany jest przypisaywany na sta�e do wierzcho�k�w
i mo�e by� zmieniony po wywo�aniu funkcji UpdateSkyDome. Kopu�a nieba
nie jest w �aden spos�b o�wietlona wi�c nie przechowujemy normalnych.
Nie u�ywamy te� tekstur, wi�c nie potrzeba �adnych UVs�w.*/
struct SkyDomeVertex
{
	DirectX::XMFLOAT3 position;		///<Pozycja wierzcho�ka
	DirectX::XMFLOAT3 color;		///<Kolor wierzcho�ka
};

/**@brief Klasa s�u�y do wy�wietlania kopu�y nieba.

Wierzcho�ki i indeksy s� zapisywane do bufor�w tylnych. Nast�pnie po zako�czeniu generowania
s� przepisywane w jednym momencie do bufor�w DirectXowych. Dzieki temu mo�emy generowa� nowy uk�ad nieba
w innym w�tku, a potem w jednym momencie go przepisa�.
Generowanie nieba jest zasadniczo procesem d�ugim i nie mo�na go wykonywac w czasie rzeczywistym.*/
class HosekSkyDome : public SkyDome
{
	RTTR_ENABLE( SkyDome );
private:
	Index32*					m_backIdxBuffer;		///< Backbuffer index buffer.
	SkyDomeVertex*				m_backVertexBuffer;		///< Backbuffer vertex buffer.
	unsigned int				m_verticiesCount;		///< Number of vertcies in buffer.

	HosekSkylightModel			m_skylightModel;		///< Hosek model for computing light.

private:

	void				GenerateSphere		( int vertical, int horizontal, float radius );

public:

	explicit		HosekSkyDome	();
	virtual			~HosekSkyDome	();

	void InitSkyDome( DirectX::XMVECTOR sun_direction,
					  const double turbidity,
					  const double* albedo,
					  int vertical,
					  int horizontal,
					  float radius,
					  float sky_intensity = 1.0f,
					  float sun_intensity = 1.0f );
	void UpdateSkyDome( DirectX::XMVECTOR sun_direction,
						const double turbidity,
						const double* albedo,
						float sky_intensity = 1.0f,
						float sun_intensity = 1.0f );
	void UpdateBuffers( IRenderer* renderer ) override;
};


}	// sw

