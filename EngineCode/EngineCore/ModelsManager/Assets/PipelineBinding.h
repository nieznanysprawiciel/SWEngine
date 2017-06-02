#pragma once
/**
@file PipelineBinding.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "swGraphicAPI/Resources/MeshResources.h"

/**@brief Resource binding to shader stages (alias for uint8).
@see ShaderType*/
typedef uint8 BindingFlag;
typedef uint8 BindingPoint;
typedef uint16 SegmentIdx;


/**@brief Contains resource and binding flags.

By default resource is bound to pixel shader stage.

@see ShaderType
@ingroup Materials
@ingroup Textures
*/
template< typename ResourceType >
class Binding
{
	RTTR_REGISTRATION_FRIEND
private:

	ResourcePtr< ResourceType >		m_resource;
	BindingFlag						m_flags;
	BindingPoint					m_point;
	SegmentIdx						m_segmentIdx;

public:
	explicit	Binding		();
	explicit	Binding		( BindingFlag flags );
	explicit	Binding		( ResourceType* tex );
	explicit	Binding		( const ResourcePtr< ResourceType >& tex );
	explicit	Binding		( ResourcePtr< ResourceType >&& tex );

	bool		IsBound		( ShaderType type );
	void		Bind		( ShaderType type, bool value = true );

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
//			Constructors	
//====================================================================================//

template< typename ResourceType >
Binding< ResourceType >::Binding()
	:	m_flags( (uint8)ShaderType::PixelShader )
{ }

template< typename ResourceType >
Binding< ResourceType >::Binding( BindingFlag flags )
	:	m_flags( flags )
{ }

template< typename ResourceType >
Binding< ResourceType >::Binding( ResourceType* tex )
	:	m_texture( tex )
	,	m_flags( (uint8)ShaderType::PixelShader )
{ }

template< typename ResourceType >
Binding< ResourceType >::Binding( const ResourcePtr< ResourceType >& tex )
	:	m_texture( tex )
	,	m_flags( (uint8)ShaderType::PixelShader )
{ }

template< typename ResourceType >
Binding< ResourceType >::Binding( ResourcePtr< ResourceType >&& tex )
	:	m_texture( std::move( tex ) )
	,	m_flags( (uint8)ShaderType::PixelShader )
{ }


//====================================================================================//
//			Inline implementation	
//====================================================================================//



/**@brief Check if is bound.*/
template< typename ResourceType >
inline bool		Binding< ResourceType >::IsBound		( ShaderType type )
{
	return ( m_flags & (uint8)type ) != 0;
}

/**@brief Bind or unbind from shader stage.*/
template< typename ResourceType >
inline void		Binding< ResourceType >::Bind		( ShaderType type, bool value )
{
	if( value )
		m_flags = m_flags | (uint8)type;
	else
		m_flags = m_flags & ( 0xFF ^ (uint8)type );
}

/**@brief */
template< typename ResourceType >
inline bool		Binding< ResourceType >::IsBoundVertexShader()
{
	return IsBound( ShaderType::VertexShader );
}

/**@brief */
template< typename ResourceType >
inline bool		Binding< ResourceType >::IsBoundPixelShader()
{
	return IsBound( ShaderType::PixelShader );
}

/**@brief */
template< typename ResourceType >
inline bool		Binding< ResourceType >::IsBoundGemetryShader()
{
	return IsBound( ShaderType::GeometryShader );
}

/**@brief */
template< typename ResourceType >
inline bool		Binding< ResourceType >::IsBoundTesselationControlShader()
{
	return IsBound( ShaderType::TesselationControlShader );
}

/**@brief */
template< typename ResourceType >
inline bool		Binding< ResourceType >::IsBoundTesselationEvaluationShader()
{
	return IsBound( ShaderType::TesselationEvaluationShader );
}

/**@brief */
template< typename ResourceType >
inline void		Binding< ResourceType >::BindVertexShader( bool value )
{
	Bind( ShaderType::VertexShader, value );
}

/**@brief */
template< typename ResourceType >
inline void		Binding< ResourceType >::BindPixelShader( bool value )
{
	Bind( ShaderType::PixelShader, value );
}

/**@brief */
template< typename ResourceType >
inline void		Binding< ResourceType >::BindGeometryShader( bool value )
{
	Bind( ShaderType::GeometryShader, value );
}

/**@brief */
template< typename ResourceType >
inline void		Binding< ResourceType >::BindTesselationControlShader( bool value )
{
	Bind( ShaderType::TesselationControlShader, value );
}

/**@brief */
template< typename ResourceType >
inline void		Binding< ResourceType >::BindTesselationEvaluationShader( bool value )
{
	Bind( ShaderType::TesselationEvaluationShader, value );
}

