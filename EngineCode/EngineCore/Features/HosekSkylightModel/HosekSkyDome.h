#pragma once

/**@file HosekSkyDome.h
@brief Plik zawiera deklaracjê klasy HosekSkyDome, do wyœwietlania nieba
u¿ywaj¹c modelu Hodeka i Wilkiego.*/

#include "EngineCore/Actors/Extended/SkyDome.h"
#include "HosekSkyModel.h"



namespace sw
{


/**@brief Struktura wierzcho³ka u¿ywana przez klasê HosekSkyDome.

Kolor wierzcho³ka jest ustalany jest przypisaywany na sta³e do wierzcho³ków
i mo¿e byæ zmieniony po wywo³aniu funkcji UpdateSkyDome. Kopu³a nieba
nie jest w ¿aden sposób oœwietlona wiêc nie przechowujemy normalnych.
Nie u¿ywamy te¿ tekstur, wiêc nie potrzeba ¿adnych UVsów.*/
struct SkyDomeVertex
{
	DirectX::XMFLOAT3 position;		///<Pozycja wierzcho³ka
	DirectX::XMFLOAT3 color;		///<Kolor wierzcho³ka
};

/**@brief Klasa s³u¿y do wyœwietlania kopu³y nieba.

Wierzcho³ki i indeksy s¹ zapisywane do buforów tylnych. Nastêpnie po zakoñczeniu generowania
s¹ przepisywane w jednym momencie do buforów DirectXowych. Dzieki temu mo¿emy generowaæ nowy uk³ad nieba
w innym w¹tku, a potem w jednym momencie go przepisaæ.
Generowanie nieba jest zasadniczo procesem d³ugim i nie mo¿na go wykonywac w czasie rzeczywistym.*/
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

