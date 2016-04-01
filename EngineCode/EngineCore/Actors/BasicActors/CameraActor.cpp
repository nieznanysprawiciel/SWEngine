#include "EngineCore/stdafx.h"
#include "CameraActor.h"

#include "Common/MemoryLeaks.h"

RTTR_REGISTRATION
{
	rttr::registration::class_< CameraActor >( "CameraActor" );
}


using namespace DirectX;

/**@brief Tworzy macierz projekcji dla danej kamery i umieszcza w polu projection_matrix

@param[in] angle K¹t widzenia w pionie
@param[in] X_to_Y Stosunek Szerokoœci do wysokoœci ekranu
@param[in] near_plane Bli¿sza p³aszczyzna obcinania
@param[in] far_plane Dalsza p³aszczyzna obcinania*/
void CameraActor::SetProjectionMatrix( float angle, float X_to_Y, float near_plane, float far_plane )
{
	XMMATRIX proj_matrix = XMMatrixPerspectiveFovLH( angle, X_to_Y, near_plane, far_plane );
	proj_matrix = XMMatrixTranspose( proj_matrix );
	XMStoreFloat4x4( &projection_matrix, proj_matrix );
}


