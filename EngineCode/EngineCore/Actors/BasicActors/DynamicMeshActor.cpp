#include "EngineCore/stdafx.h"
#include "DynamicMeshActor.h"


#include "EngineCore/ModelsManager/Model3DFromFile.h"
#include "GraphicAPI/IRenderer.h"
#include "GraphicAPI/MeshResources.h"

#include "Common/MemoryLeaks.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< DynamicMeshActor >( "DynamicMeshActor" )
#ifdef _SCALEABLE_OBJECTS
		.property( "Scale", &DynamicMeshActor::scale )
		(
			rttr::metadata( MetaDataType::Category, "Transformation" )
		)
#endif
		.property( "Asset", &DynamicMeshActor::m_modelRef )
		(
			rttr::metadata( MetaDataType::Category, "Mesh" )
		);
}


DynamicMeshActor::DynamicMeshActor()
{
	m_modelRef = nullptr;
	model_changed = false;
	vertex_buffer = nullptr;
	index_buffer = nullptr;
#ifdef _SCALEABLE_OBJECTS
	scale = 1.0;
#endif
}

DynamicMeshActor::DynamicMeshActor( BufferObject* vertexBuffer, BufferObject* indexBuffer )
	: vertex_buffer( vertexBuffer ),
	index_buffer( indexBuffer )
{
	m_modelRef = nullptr;
	model_changed = false;
#ifdef _SCALEABLE_OBJECTS
	scale = 1.0;
#endif
}

DynamicMeshActor::~DynamicMeshActor()
{
	//Kasujac obiekt nie wolno nam niczego usuwa�, bo nic nie nale�y do nas
	//jedynie kasujemy referencje

	//kasujemy referencje bezpo�rednie
	DeleteAllReferences();
}

bool DynamicMeshActor::SetModel(Model3DFromFile* model)
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

void DynamicMeshActor::AddModelPart( ModelPart& modelPart )
{
	model_parts.push_back( modelPart );
	model_changed = true;
}


/**
Dodajemy odwo�ania do wszystkich istniej�cych element�w w przekazanym wska�niku.

@param[in] part Struktura ModelPart opisuj�ce cz�� mesha, w kt�rej dodajemy referencje.*/
void DynamicMeshActor::AddReferences( const ModelPart* part )
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
void DynamicMeshActor::DeleteAllReferences( )
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


