#include "Normalmapshaderclass.h"

HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}

NormalMapShaderClass::NormalMapShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
	m_lightBuffer = 0;
}

NormalMapShaderClass::NormalMapShaderClass(const NormalMapShaderClass& other)
{
}

NormalMapShaderClass::NormalMapShaderClass()
{
}

bool NormalMapShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize Vertex & Pixel Shaders.
	result = InitializeShader(device, hwnd, L"normalmap.vs", L"normalmap.ps");
	if (!result)
	{
		return false;
	}
	return true;
}

void NormalMapShaderClass::Shutdown()
{
	//Shutdown the vertex and pixel Shader as well as the related objects.
	ShutdownShader();

	return;
}

bool NormalMapShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView** textureArray, XMVECTOR lightDirection, XMVECTOR diffuseColor)
{
	bool result;

	//Set shader oarameters that it will use for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, textureArray, lightDirection, diffuseColor);

	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;

}

bool NormalMapShaderClass::InitializeShader(ID3D11Device*device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3DBlob* errorMessage;
	ID3DBlob* vertexShaderBuffer;
	ID3DBlob* pixelShaderBuffer;
	


	//The polygon layout is now set to five elements to accommodate the tangent and binormal.

	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = CompileShader(L"NormalShader_vs.hlsl", "main", "vs_5_0", &vertexShaderBuffer);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}


	//The bump map pixel shader is loaded here.

		// Compile the pixel shader code.
		result = D3D10CompileShader(psFilename, NULL, NULL, "BumpMapPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

}