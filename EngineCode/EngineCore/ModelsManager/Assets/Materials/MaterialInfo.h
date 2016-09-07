#pragma once

/// Maksymalna liczba tekstur w materiale.
#define MAX_MATERIAL_TEXTURES		5



/**@brief Struktura opisuj¹ca dodatkowe bufory sta³ych dla shaderów.
@ingroup Materials*/
struct AdditionalBufferInfo
{
	ShaderType		ShaderType;		///< Type shadera, do którego ma zostaæ zbindowany bufor.
	rttr::type		BufferType;		///< Type of buffer, which must be bound to material by actor.
	uint32			BufferSize;		///< Rozmiar bufora w bajtach.

	AdditionalBufferInfo()
		: BufferType( rttr::type::get( "" ) )
	{}
};


/**@brief Deskryptor materia³u.
@ingroup Materials*/
struct MaterialInfo
{
	std::vector< AdditionalBufferInfo >		AdditionalBuffers;		///< Opis dodatkowych buforów, jakie musi posiadaæ aktor.
	std::wstring							MaterialName;			///< Nazwa materia³u. Mo¿e to byæ œcie¿ka do pliku z którego powsta³.
	ShadingModelBase*						ShadingData;			///< Przechowuje parametry modelu cieniowania. @see ShadingModelData


	MaterialInfo	( const std::wstring& name, std::vector< AdditionalBufferInfo >&& addBuff, ShadingModelBase* shadModel );
};

inline MaterialInfo::MaterialInfo( const std::wstring& name, std::vector< AdditionalBufferInfo >&& addBuff, ShadingModelBase* shadModel )
{
	AdditionalBuffers = std::move( addBuff );
	MaterialName = name;
	ShadingData = shadModel;
}
