#include "SWMaterialLoader.h"

#include "EngineCore/ModelsManager/ModelsManager.h"
#include "GraphicAPI/MeshResources.h"




template< typename ShaderType >
struct ShaderFunGetter
{
	typedef ShaderType* (ModelsManager::*ShaderLoadFun)( const std::wstring&, const std::string& );


	static ShaderLoadFun		ShaderLoadingFunction()
	{	static_assert( false, "Specialize struct" ); };
};

template<>
struct ShaderFunGetter< VertexShader >
{
	typedef VertexShader* (ModelsManager::*ShaderLoadFun)( const std::wstring&, const std::string& );

	static ShaderLoadFun		ShaderLoadingFunction()
	{	return &ModelsManager::LoadVertexShader; }
};

template<>
struct ShaderFunGetter< PixelShader >
{
	typedef PixelShader* (ModelsManager::*ShaderLoadFun)( const std::wstring&, const std::string& );

	static ShaderLoadFun		ShaderLoadingFunction()
	{	return &ModelsManager::LoadPixelShader; }
};

template<>
struct ShaderFunGetter< GeometryShader >
{
	typedef GeometryShader* (ModelsManager::*ShaderLoadFun)( const std::wstring&, const std::string& );

	static ShaderLoadFun		ShaderLoadingFunction()
	{	return &ModelsManager::LoadGeometryShader; }
};

template<>
struct ShaderFunGetter< ControlShader >
{
	typedef ControlShader* (ModelsManager::*ShaderLoadFun)( const std::wstring&, const std::string& );

	static ShaderLoadFun		ShaderLoadingFunction()
	{	return &ModelsManager::LoadControlShader; }
};

template<>
struct ShaderFunGetter< EvaluationShader >
{
	typedef EvaluationShader* (ModelsManager::*ShaderLoadFun)( const std::wstring&, const std::string& );

	static ShaderLoadFun		ShaderLoadingFunction()
	{	return &ModelsManager::LoadEvaluationShader; }
};




template< typename ShaderType >
inline Nullable< ResourcePtr< ShaderType > >	SWMaterialLoader::LoadShader	( IDeserializer* deser, const std::string& shaderNameString )
{
	ShaderLoadInfo shader = DeserializeShader( deser, shaderNameString );

	if( !shader.first )
		return Nullable< ResourcePtr< ShaderType > >( shaderNameString + " path is not set." );

	if( !shader.second )
		return Nullable< ResourcePtr< ShaderType > >( shaderNameString + " entry function is not set." );


	typename ShaderFunGetter< ShaderType >::ShaderLoadFun LoadShaderFunPtr = ShaderFunGetter< ShaderType >::ShaderLoadingFunction();

	std::wstring filePath = Convert::FromString< std::wstring >( std::string( shader.first ), std::wstring() );
	std::string entryFunction = std::string( shader.second );

	ShaderType* shaderObject = (m_modelsManager->*LoadShaderFunPtr)( filePath, entryFunction );
	if( !shaderObject )
		return Nullable< ResourcePtr< ShaderType > >( shaderNameString + " file could not be loaded." );

	return Nullable< ResourcePtr< ShaderType > >( ResourcePtr< ShaderType >( shaderObject ) );
}


