#pragma once
/**
@file MaterialInfo.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/TypesDefinitions.h"

/// Maksymalna liczba tekstur w materiale.
#define MAX_MATERIAL_TEXTURES		5



namespace sw
{


/**@brief Struktura opisuj¹ca dodatkowe bufory sta³ych dla shaderów.
@ingroup Materials*/
struct AdditionalBufferInfo
{
	ShaderType		ShaderType;		///< Type shadera, do którego ma zostaæ zbindowany bufor.
	rttr::type		BufferType;		///< Type of buffer, which must be bound to material by actor.
	uint32			BufferSize;		///< Rozmiar bufora w bajtach.

	AdditionalBufferInfo()
		: BufferType( rttr::type::get_by_name( "" ) )
	{}

	std::string		GetBufferType		() const { return BufferType.get_name().to_string(); }
	void			SetBufferType		( std::string name );
};





/**@brief Deskryptor materia³u.
@ingroup Materials*/
struct MaterialInfo
{
	std::vector< AdditionalBufferInfo >		AdditionalBuffers;		///< Opis dodatkowych buforów, jakie musi posiadaæ aktor.
	std::wstring							MaterialName;			///< Nazwa materia³u. Mo¿e to byæ œcie¿ka do pliku z którego powsta³.
	UPtr< ShadingModelBase >				ShadingData;			///< Przechowuje parametry modelu cieniowania. @see ShadingModelData


	MaterialInfo	( const std::wstring& name, std::vector< AdditionalBufferInfo >&& addBuff, UPtr< ShadingModelBase >&& shadModel );
	~MaterialInfo	();
};






// ================================ //
//
inline MaterialInfo::MaterialInfo( const std::wstring& name, std::vector< AdditionalBufferInfo >&& addBuff, UPtr< ShadingModelBase >&& shadModel )
{
	AdditionalBuffers = std::move( addBuff );
	MaterialName = name;
	ShadingData = std::move( shadModel );
}

// ================================ //
//
inline MaterialInfo::~MaterialInfo	()
{}

// ================================ //
//
inline void			AdditionalBufferInfo::SetBufferType( std::string name )
{
	auto newType = rttr::type::get( name );
	if( newType.is_valid() )
		BufferType = newType;
}

}	// sw

