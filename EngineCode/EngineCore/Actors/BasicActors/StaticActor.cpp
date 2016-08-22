#include "EngineCore/stdafx.h"
#include "StaticActor.h"

#include "EngineCore/ModelsManager/Model3DFromFile.h"

#include "Common/MemoryLeaks.h"



RTTR_REGISTRATION
{
	rttr::registration::class_< StaticActor >( "StaticActor" )
		.property( "Position", &StaticActor::position )
		(
			rttr::metadata( MetaDataType::Category, "Transformation" ),
			rttr::policy::prop::BindAsPtr()
		)
		.property( "Orientation", &StaticActor::orientation )
		(
			rttr::metadata( MetaDataType::Category, "Transformation" ),
			rttr::policy::prop::BindAsPtr()
		)
#ifdef _SCALEABLE_OBJECTS
		.property( "Scale", &StaticActor::scale )
		(
			rttr::metadata( MetaDataType::Category, "Transformation" )
		)
#endif
		.property( "Asset", &StaticActor::GetModel, &StaticActor::SetModel )
		(
			rttr::metadata( MetaDataType::Category, "Mesh" )
		);
}

using namespace DirectX;

// ================================ //
//
StaticActor::StaticActor()
{//inicjujemy obiekt w punkcie ( 0.0, 0.0, 0.0 ) i zorientowany tak jak jego mesh
	position.x = 0.0;
	position.y = 0.0;
	position.z = 0.0;

	position_back = position;

	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4(&orientation,quaternion);

	orientation_back = orientation;

	swap_data = false;

	InitNullVisibleComponent();
}

// ================================ //
//
StaticActor::StaticActor( const XMFLOAT3& pos, const XMFLOAT4& orient )
{
	position = pos;
	position_back = pos;
	orientation = orient;
	orientation_back = orient;

	swap_data = false;

	InitNullVisibleComponent();
}

// ================================ //
//
StaticActor::StaticActor	( BufferObject* vertexBuffer, BufferObject* indexBuffer )
{
	position.x = 0.0;
	position.y = 0.0;
	position.z = 0.0;

	position_back = position;

	XMVECTOR quaternion = XMQuaternionIdentity();
	XMStoreFloat4(&orientation,quaternion);

	orientation_back = orientation;

	swap_data = false;

	InitNullVisibleComponent();
	vertex_buffer = vertexBuffer;
	index_buffer = indexBuffer;
}

// ================================ //
//
StaticActor::~StaticActor()
{
	//Kasujac obiekt nie wolno nam niczego usuwa�, bo nic nie nale�y do nas
	//jedynie kasujemy referencje

	//kasujemy referencje bezpo�rednie
	DeleteAllReferences();
}

// ================================ //
//
void	StaticActor::InitNullVisibleComponent()
{
	m_modelRef = nullptr;
	model_changed = false;
	vertex_buffer = nullptr;
	index_buffer = nullptr;
#ifdef _SCALEABLE_OBJECTS
	scale = 1.0;
#endif
}

/**@brief Zwraca interpolowan� pozycj� obiektu mi�dzy przechowywanymi po�o�eniami.

@param[in] time_lag Procent czasu jaki up�yn�� od ostaniej klatki do nast�pnej
Zakres [0,1].*/
XMVECTOR StaticActor::GetInterpolatedPosition( float time_lag ) const
{
	XMVECTOR pos2 = XMLoadFloat3( &position );
	XMVECTOR pos1 = XMLoadFloat3( &position_back );
	if ( XMVector3Equal( pos1, pos2 ) )
		return pos1;

	if ( swap_data )
		pos1 = XMVectorLerp( pos2, pos1, time_lag );
	else
		pos1 = XMVectorLerp( pos1, pos2, time_lag );

	pos1 = XMVectorSetW( pos1, 1 );
	return pos1;
}

/**@brief Zwraca interpolowan� orientacj� obiektu mi�dzy przechowywanymi po�o�eniami.

@param[in] time_lag Procent czasu jaki up�yn�� od ostaniej klatki do nast�pnej
Zakres [0,1].*/
XMVECTOR StaticActor::GetInterpolatedOrientation( float time_lag ) const
{
	XMVECTOR orient2 = XMLoadFloat4( &orientation );
	XMVECTOR orient1 = XMLoadFloat4( &orientation_back );
	if ( XMVector3Equal( orient1, orient2 ) )	// Wa�ne! Oblcizenia na floatach s� niedok�adne i troszk� wszystko lata.
		return orient1;

	if ( swap_data )
		return XMQuaternionSlerp( orient2, orient1, time_lag );
	else
		return XMQuaternionSlerp( orient1, orient2, time_lag );
}


//====================================================================================//
//			Visible component	
//====================================================================================//

bool		StaticActor::SetModel( Model3DFromFile* model )
{
	if( model == nullptr )
		return false;

	//najpierw czy�cimy poprzedni� zawarto��
	//kasujemy referencje bezpo�rednie
	DeleteAllReferences();

	//dodajemy now� zawarto��
	m_modelRef = model;
	model->AddObjectReference();

	vertex_buffer = model->get_vertex_buffer();
	if ( !vertex_buffer )
		return false;	// Nie ma bufora wierzcho�k�w, to nie ma dalej czego szuka�
	vertex_buffer->AddObjectReference();


	index_buffer = model->get_index_buffer();
	if ( index_buffer )		// Jak nie ma bufora indeks�w to w zasadzie krzywda si� nikomu nie dzieje
		index_buffer->AddObjectReference();		// Ale trzeba pilnowa�, �eby nie dodawa� odwo�a� do obiektu, kt�rego nie ma

	Size count = model->get_parts_count();
	model_parts.reserve( count );

	for( Size i = 0; i < count; ++i )
	{//przepisujemy sobie wska�niki
		register const ModelPart* part = model->get_part( (unsigned int)i );

		model_parts.push_back( *part );
		AddReferences(part);			//Dodajemy odwo�ania
	}

	//Pytanie czy to ma sens. Funkcja reserve ustawi�a wielko�� wektora na przynajmniej count.
	//Je�eli da�a wi�cej, to warto by by�o zmniejszy�, bo zawarto�� tych wektor�w si� ju� nie zmieni.
	//Problemem jest, �e obie te funkcje wed�ug dokumentacji nie s� wi���ce.
	model_parts.shrink_to_fit( );

	model_changed = false;		//Zawarto�� tablic odpowiada modelowi

	return true;
}

void		StaticActor::AddModelPart( ModelPart& modelPart )
{
	model_parts.push_back( modelPart );
	model_changed = true;
}


/**
Dodajemy odwo�ania do wszystkich istniej�cych element�w w przekazanym wska�niku.

@param[in] part Struktura ModelPart opisuj�ce cz�� mesha, w kt�rej dodajemy referencje.*/
void		StaticActor::AddReferences( const ModelPart* part )
{
	if ( part == nullptr )
		return;

	if ( part->material )
		part->material->AddObjectReference();
	if ( part->mesh )
		part->mesh->AddObjectReference();
	if ( part->pixel_shader )
		part->pixel_shader->AddObjectReference();
	for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
		if ( part->texture[i] )
			part->texture[i]->AddObjectReference( );
	if ( part->vertex_shader )
		part->vertex_shader->AddObjectReference();

}

/**
Kasuje odwo�ania do obiekt�w, kt�rych w�asno�ci� jest ModelsManager albo Model3DFromFile
w tablicy model_parts oraz wska�niku m_modelRef i vertex_buffer.

�adne obiekty nie s� kasowane, poniewa� nie nale�� one do nas.
Wszystkie zmienne s� za to czyszczone.*/
void		StaticActor::DeleteAllReferences( )
{
	if ( m_modelRef != nullptr )
		m_modelRef->DeleteObjectReference( );
	m_modelRef = nullptr;

	if ( vertex_buffer )
		vertex_buffer->DeleteObjectReference();
	vertex_buffer = nullptr;

	if ( index_buffer )
		index_buffer->DeleteObjectReference( );
	index_buffer = nullptr;

	for ( unsigned int k = 0; k < model_parts.size( ); ++k )
	{//ka�dy element mo�e by� nullptrem
		register ModelPart* part = &model_parts[k];

		//ka�dy element mo�e by� nullptrem
		if ( part->material )
			part->material->DeleteObjectReference();
		if ( part->mesh )
			part->mesh->DeleteObjectReference( );
		if ( part->pixel_shader )
			part->pixel_shader->DeleteObjectReference( );
		for ( int i = 0; i < ENGINE_MAX_TEXTURES; ++i )
			if ( part->texture[i] )
				part->texture[i]->DeleteObjectReference( );
		if ( part->vertex_shader )
			part->vertex_shader->DeleteObjectReference( );
	}
	model_parts.clear();
}

