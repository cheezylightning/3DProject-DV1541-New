//FileName: normalmapshaderclass.h


#include <windows.h>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <string.h>
#include "GameTime.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "SpriteBatch.h"
#include "DDSTextureLoader.h"


using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")


class NormalMapShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	struct LightBufferType
	{
		XMVECTOR diffuseColor;
		XMVECTOR lightDirection;
		float padding;
	};
	
public:
	NormalMapShaderClass();
	NormalMapShaderClass(const NormalMapShaderClass& NormalMap);
	~NormalMapShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**, XMVECTOR, XMVECTOR);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	bool OutputShaderErrorMessage(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**, XMVECTOR, XMVECTOR);
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**, XMVECTOR, XMVECTOR);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;

	// constant buffer to interface with light direction and light color
	ID3D11Buffer* m_lightBuffer;

};




