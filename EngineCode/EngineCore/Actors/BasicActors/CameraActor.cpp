#include "EngineCore/stdafx.h"
#include "CameraActor.h"

#include "Common/MemoryLeaks.h"

RTTR_REGISTRATION
{
	rttr::registration::class_< CameraActor >( "CameraActor" )
		.property( "IsPerspective", &CameraActor::GetIsPerspective, &CameraActor::SetPerspective )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "Width", &CameraActor::GetWidth, &CameraActor::SetWidth )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "Height", &CameraActor::GetHeight, &CameraActor::SetHeight )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "Near Plane", &CameraActor::GetNearPlane, &CameraActor::SetNearPlane )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "Far Plane", &CameraActor::GetFarPlane, &CameraActor::SetFarPlane )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		)
		.property( "FOV", &CameraActor::GetFov, &CameraActor::SetFov )
			(	rttr::metadata( MetaDataType::Category, "Camera View" )		);
}


using namespace DirectX;

/**@brief Tworzy domyœln¹ kamerê perspektywiczn¹ o parametrach:
m_width:		1920
m_height:		1080
m_nearPlane		1.0
m_farPlane		100000.0
m_fov			45.0*/
CameraActor::CameraActor()
{
	m_isPerspective = true;
	m_height = 1080;
	m_width = 1920;
	m_nearPlane = 1.0f;
	m_farPlane = 100000.0f;
	m_fov = 45.0f;

	SetPerspectiveProjectionMatrix( DirectX::XMConvertToRadians( m_fov ), m_width / m_height, m_nearPlane, m_farPlane );
}


/**@brief Tworzy macierz projekcji perspektywicznej dla danej kamery i umieszcza w polu projection_matrix.

@param[in] angle K¹t widzenia w pionie
@param[in] aspect Stosunek Szerokoœci do wysokoœci ekranu
@param[in] nearPlane Bli¿sza p³aszczyzna obcinania
@param[in] farPlane Dalsza p³aszczyzna obcinania*/
void CameraActor::SetPerspectiveProjectionMatrix		( float angle, float aspect, float nearPlane, float farPlane )
{
	XMMATRIX proj_matrix = XMMatrixPerspectiveFovLH( angle, aspect, nearPlane, farPlane );
	proj_matrix = XMMatrixTranspose( proj_matrix );
	XMStoreFloat4x4( &m_projectionMatrix, proj_matrix );
}

/**@brief Tworzy macierz projekcji ortogonalnej dla danej kamery i umieszcza w polu projection_matrix.

@param[in] angle K¹t widzenia w pionie
@param[in] aspect Stosunek Szerokoœci do wysokoœci ekranu
@param[in] nearPlane Bli¿sza p³aszczyzna obcinania
@param[in] farPlane Dalsza p³aszczyzna obcinania*/
void CameraActor::SetOrthogonalProjectionMatrix			( float width, float height, float nearPlane, float farPlane )
{
	XMMATRIX proj_matrix = XMMatrixOrthographicLH( width, height, nearPlane, farPlane );
	proj_matrix = XMMatrixTranspose( proj_matrix );
	XMStoreFloat4x4( &m_projectionMatrix, proj_matrix );
}

/**@brief Ustawia macierz perspektywiczn¹ lub ortogonaln¹.*/
void				CameraActor::SetPerspective		( bool value )
{
	m_isPerspective = value;
	UpdateMatrix();
}

/**@brief Ustawia szerokoœæ.*/
void				CameraActor::SetWidth			( float width )
{
	m_width = width;
	UpdateMatrix();
}

/**@brief Ustawia wysokoœæ.*/
void				CameraActor::SetHeight			( float height )
{
	m_height = height;
	UpdateMatrix();
}

/**@brief Ustawia bli¿sz¹ p³aszczyznê obcianania.*/
void				CameraActor::SetNearPlane		( float plane )
{
	m_nearPlane = plane;
	UpdateMatrix();
}

/**@brief Ustawia dalsz¹ p³aszczyznê obcinania.*/
void				CameraActor::SetFarPlane		( float plane )
{
	m_farPlane = plane;
	UpdateMatrix();
}

/**@brief Ustawia k¹t widzenia.*/
void				CameraActor::SetFov				( float fov )
{
	m_fov = fov;
	UpdateMatrix();
}

/**@copydoc CameraData*/
CameraData CameraActor::GetCameraData()
{
	CameraData data;
	data.IsPerspective = m_isPerspective;
	data.FarPlane = m_farPlane;
	data.Fov = m_fov;
	data.Height = m_height;
	data.NearPlane = m_nearPlane;
	data.Width = m_width;
	XMStoreFloat4( &data.Orientation, GetOrientation() );
	XMStoreFloat3( &data.Position, GetPosition() );
	return data;
}

/**@brief Aktualizuje przechowywan¹ macierz na podstawie wartoœci pól klasy.*/
void				CameraActor::UpdateMatrix		()
{
	if( m_isPerspective )
		SetPerspectiveProjectionMatrix( DirectX::XMConvertToRadians( m_fov ), m_width / m_height, m_nearPlane, m_farPlane );
	else
		SetOrthogonalProjectionMatrix( m_width, m_height, m_nearPlane, m_farPlane );
}