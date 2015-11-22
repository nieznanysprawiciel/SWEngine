#pragma once

#include "../../MeshResources.h"
#include "../DX11Initializer/DX11APIObjects.h"


/**@brief Implementacja compute shadera w DirectX11.
@ingroup DX11API*/
class DX11ComputeShader :	 public ComputeShaderObject, public DX11APIObjects
{
private:
	ID3D11ComputeShader*			m_computeShader;
protected:
	~DX11ComputeShader();
public:
	DX11ComputeShader();

	inline ID3D11ComputeShader*		Get() { return m_computeShader; }

	virtual bool					ReloadFromFile			() override;
	virtual bool					ReloadFromBinFile		() override;
	virtual void					SaveShaderBinFile		( const std::wstring& file_name ) override;

	static DX11ComputeShader*		CreateFromFile( const std::wstring& file_name, const std::string& shader_name, const char* shader_model = "vs_4_0" );
	static DX11ComputeShader*		CreateFromBinFile( const std::wstring& file_name );

	void							SetFileName				( const std::wstring& fileName ) { m_shaderFile = fileName; }
	void							SetShaderName			( const std::string& shaderName ) { m_shaderEntry = shaderName; }
};

