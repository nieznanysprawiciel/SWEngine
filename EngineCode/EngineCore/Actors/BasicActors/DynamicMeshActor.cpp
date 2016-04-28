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
	//Kasujac obiekt nie wolno nam niczego usuwaæ, bo nic nie nale¿y do nas
	//jedynie kasujemy referencje

	//kasujemy referencje bezpoœrednie
	DeleteAllReferences();
}

bool DynamicMeshActor::SetModel(Model3DFromFile* model)
{
	if( model == nullptr )
		return false;

	//najpierw czyœcimy poprzedni¹ zawartoœæ
	//kasujemy referencje bezpoœrednie
	DeleteAllReferences();

	//dodajemy now¹ zawartoœæ
	m_modelRef = model;
	model->AddObjectReference();

	vertex_buffer = model->get_vertex_buffer();
	if ( !vertex_buffer )
		return false;	// Nie ma bufora wierzcho³ków, to nie ma dalej czego szukaæ
	vertex_buffer->AddObjectReference();


	index_buffer = model->get_index_buffer();
	if ( index_buffer )		// Jak nie ma bufora indeksów to w zasadzie krzywda siê nikomu nie dzieje
		index_buffer->AddObjectReference();		// Ale trzeba pilnowaæ, ¿eby nie dodawaæ odwo³añ do obiektu, którego nie ma

	Size count = model->get_parts_count();
	model_parts.reserve( count );

	for( Size i = 0; i < count; ++i )
	{//przepisujemy sobie wskaŸniki
		register const ModelPart* part = model->get_part( (unsigned int)i );

		model_parts.push_back( *part );
		AddReferences(part);			//Dodajemy odwo³ania
	}

	//Pytanie czy to ma sens. Funkcja reserve ustawi³a wielkoœæ wektora na przynajmniej count.
	//Je¿eli da³a wiêcej, to warto by by³o zmniejszyæ, bo zawartoœæ tych wektorów siê ju¿ nie zmieni.
	//Problemem jest, ¿e obie te funkcje wed³ug dokumentacji nie s¹ wi¹¿¹ce.
	model_parts.shrink_to_fit( );

	model_changed = false;		//Zawartoœæ tablic odpowiada modelowi

	return true;
}

void DynamicMeshActor::AddModelPart( ModelPart& modelPart )
{
	model_parts.push_back( modelPart );
	model_changed = true;
}


/**
Dodajemy odwo³ania do wszystkich istniej¹cych elementów w przekazanym wskaŸniku.

@param[in] part Struktura ModelPart opisuj¹ce czêœæ mesha, w której dodajemy referencje.*/
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
Kasuje odwo³ania do obiektów, których w³asnoœci¹ jest ModelsManager albo Model3DFromFile
w tablicy model_parts oraz wskaŸniku m_modelRef i vertex_buffer.

¯adne obiekty nie s¹ kasowane, poniewa¿ nie nale¿¹ one do nas.
Wszystkie zmienne s¹ za to czyszczone.*/
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
	{//ka¿dy element mo¿e byæ nullptrem
		register ModelPart* part = &model_parts[k];

		//ka¿dy element mo¿e byæ nullptrem
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


