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


/**@brief Struktura opisuj�ca dodatkowe bufory sta�ych dla shader�w.
@ingroup Materials*/
struct AdditionalBufferInfo
{
	ShaderType		ShaderType;		///< Type shadera, do kt�rego ma zosta� zbindowany bufor.
	rttr::type		BufferType;		///< Type of buffer, which must be bound to material by actor.
	uint32			BufferSize;		///< Rozmiar bufora w bajtach.

	AdditionalBufferInfo()
		: BufferType( rttr::type::get_by_name( "" ) )
	{}

	std::string		GetBufferType		() const { return BufferType.get_name().to_string(); }
	void			SetBufferType		( std::string name );
};





/**@brief Deskryptor materia�u.
@ingroup Materials*/
struct MaterialInfo
{
	std::vector< AdditionalBufferInfo >		AdditionalBuffers;		///< Opis dodatkowych bufor�w, jakie musi posiada� aktor.
	std::wstring							MaterialName;			///< Nazwa materia�u. Mo�e to by� �cie�ka do pliku z kt�rego powsta�.
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

