#pragma once
/**
@file TextureBindFlag.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "GraphicAPI/MeshResources.h"

/**@brief Texture binding to shader stages (alias for uint8).
@see ShaderType*/
typedef uint8 TextureBindingFlag;


/**@brief Contains texture and binding flags.

By default texture is bound to pixel shader stage.

@see ShaderType
@ingroup Materials*/
class TextureBinding
{
	RTTR_REGISTRATION_FRIEND
private:

	ResourcePtr< TextureObject >	m_texture;
	TextureBindingFlag				m_flags;

public:
	explicit	TextureBinding		();
	explicit	TextureBinding		( TextureBindingFlag flags );
	explicit	TextureBinding		( TextureObject* tex );
	explicit	TextureBinding		( const ResourcePtr< TextureObject >& tex );
	explicit	TextureBinding		( ResourcePtr< TextureObject >&& tex );

	bool		IsBound	( ShaderType type );
	void		Bind	( ShaderType type, bool value = true );

	bool		IsBoundVertexShader		();
	bool		IsBoundPixelShader		();
	bool		IsBoundGemetryShader	();
	bool		IsBoundTesselationControlShader		();
	bool		IsBoundTesselationEvaluationShader	();

	void		BindVertexShader				( bool value );
	void		BindPixelShader					( bool value );
	void		BindGeometryShader				( bool value );
	void		BindTesselationControlShader	( bool value );
	void		BindTesselationEvaluationShader	( bool value );

};

//====================================================================================//
//			Inline implementation	
//====================================================================================//



/**@brief Check if is bound.*/
inline bool		TextureBinding::IsBound	( ShaderType type )
{
	return ( m_flags & (uint8)type ) != 0;
}

/**@brief Bind or unbind from shader stage.*/
inline void		TextureBinding::Bind		( ShaderType type, bool value )
{
	if( value )
		m_flags = m_flags | (uint8)type;
	else
		m_flags = m_flags & ( 0xFF ^ (uint8)type );
}

/**@brief */
inline bool		TextureBinding::IsBoundVertexShader()
{
	return IsBound( ShaderType::VertexShader );
}

/**@brief */
inline bool		TextureBinding::IsBoundPixelShader()
{
	return IsBound( ShaderType::PixelShader );
}

/**@brief */
inline bool		TextureBinding::IsBoundGemetryShader()
{
	return IsBound( ShaderType::GeometryShader );
}

/**@brief */
inline bool		TextureBinding::IsBoundTesselationControlShader()
{
	return IsBound( ShaderType::TesselationControlShader );
}

/**@brief */
inline bool		TextureBinding::IsBoundTesselationEvaluationShader()
{
	return IsBound( ShaderType::TesselationEvaluationShader );
}

/**@brief */
inline void		TextureBinding::BindVertexShader( bool value )
{
	Bind( ShaderType::VertexShader, value );
}

/**@brief */
inline void		TextureBinding::BindPixelShader( bool value )
{
	Bind( ShaderType::PixelShader, value );
}

/**@brief */
inline void		TextureBinding::BindGeometryShader( bool value )
{
	Bind( ShaderType::GeometryShader, value );
}

/**@brief */
inline void		TextureBinding::BindTesselationControlShader( bool value )
{
	Bind( ShaderType::TesselationControlShader, value );
}

/**@brief */
inline void		TextureBinding::BindTesselationEvaluationShader( bool value )
{
	Bind( ShaderType::TesselationEvaluationShader, value );
}

