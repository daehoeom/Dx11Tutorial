#include "LightShaderClass.h"



LightShaderClass::LightShaderClass()
	: m_vertexShader(NULL),
	m_pixelShader(NULL),
	m_layout(NULL),
	m_sampleState(NULL),
	m_matrixBuffer(NULL),
	m_lightBuffer(NULL)
{
}

LightShaderClass::LightShaderClass(const LightShaderClass& arg)
{
}

LightShaderClass::~LightShaderClass()
{
}

bool LightShaderClass::Init(ID3D11Device* _device, HWND _hWnd)
{
	if (!InitShader(_device, _hWnd, L"../Shader/light.vs", L"../Shader/light.ps"))
	{
		return false;
	}

	return true;
}

void LightShaderClass::Destroy()
{
	DestroyShader();

	return;
}

bool LightShaderClass::Render(ID3D11DeviceContext* _deviceContext, int _indexCount, XMMATRIX& _world, XMMATRIX& _view,
	XMMATRIX& _proj, ID3D11ShaderResourceView* _texture, XMFLOAT3 _lightDir, XMFLOAT4 _diffuseColor)
{
	if (!SetShaderParameters(_deviceContext, _world, _view, _proj, _texture, _lightDir, _diffuseColor))
	{
		return false;
	}

	RenderShader(_deviceContext, _indexCount);

	return true;
}

bool LightShaderClass::InitShader(ID3D11Device* _device, HWND _hWnd, WCHAR* _vsFilename, WCHAR* _psFilename)
{
	ID3D10Blob* errorMessage = NULL;
	ID3D10Blob* vertexShaderBuffer = NULL;
	ID3D10Blob* pixelShaderBuffer = NULL;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	if (FAILED(D3DCompileFromFile(_vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage)))
	{
		if (errorMessage)
		{
			OutputShadererrorMessage(errorMessage, _hWnd, _vsFilename);
		}
		else
		{
			MessageBox(_hWnd, _vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	if (FAILED(D3DCompileFromFile(_psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage)))
	{
		if (errorMessage)
		{
			OutputShadererrorMessage(errorMessage, _hWnd, _psFilename);
		}
		else
		{
			MessageBox(_hWnd, _psFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	if (FAILED(_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		NULL, &m_vertexShader)))
	{
		return false;
	}

	if (FAILED(_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
		NULL, &m_pixelShader)))
	{
		return false;
	}

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	if (FAILED(_device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout)))
	{
		return false;
	}

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(_device->CreateSamplerState(&samplerDesc, &m_sampleState)))
	{
		return false;
	}

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	if (FAILED(_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer)))
	{
		return false;
	}

	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	if (FAILED(_device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer)))
	{
		return false;
	}

	return true;
}

void LightShaderClass::DestroyShader()
{
	SAFE_RELEASE(m_lightBuffer);
	SAFE_RELEASE(m_matrixBuffer);
	SAFE_RELEASE(m_sampleState);
	SAFE_RELEASE(m_layout);
	SAFE_RELEASE(m_pixelShader);
	SAFE_RELEASE(m_vertexShader);

	return;
}

void LightShaderClass::OutputShadererrorMessage(ID3D10Blob* _errorMessage, HWND _hWnd, WCHAR* _shaderFilename)
{
	char* compileErrors;
	int error = 0;
	unsigned long bufferSize;
	char buffer[256];
	FILE* fp;

	ZeroMemory(&buffer, 256);
	compileErrors = (char*)(_errorMessage->GetBufferPointer());

	bufferSize = _errorMessage->GetBufferSize();

	for (unsigned int i = 0; i < bufferSize; i++)
	{
		buffer[i] = compileErrors[i];
	}

	error = fopen_s(&fp, "shader-error.txt", "w");
	
	if (error != 0)
	{
		return;
	}

	fprintf(fp, buffer);
	
	fclose(fp);

	SAFE_RELEASE(_errorMessage);

	MessageBox(_hWnd, L"Error compiling shader. Check shader-error.txt for message.", _shaderFilename, MB_OK);
	return;
}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* _deviceContext, XMMATRIX& _world, XMMATRIX& _view, XMMATRIX& _proj,
	ID3D11ShaderResourceView* _texture, XMFLOAT3 _lightDirection, XMFLOAT4 _diffuseColor)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;

	_world = XMMatrixTranspose(_world);
	_view = XMMatrixTranspose(_view);
	_proj = XMMatrixTranspose(_proj);

	if (FAILED(_deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = _world;
	dataPtr->view = _view;
	dataPtr->projection = _proj;

	_deviceContext->Unmap(m_matrixBuffer, 0);

	bufferNumber = 0;

	_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	_deviceContext->PSSetShaderResources(0, 1, &_texture);

	if (FAILED(_deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->diffuseColor = _diffuseColor;
	dataPtr2->lightDirection = _lightDirection;
	dataPtr2->padding = 0.0f;

	_deviceContext->Unmap(m_lightBuffer, 0);

	bufferNumber = 0;

	_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext* _deviceContext, int _indexCount)
{
	_deviceContext->IASetInputLayout(m_layout);

	_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	_deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	_deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	_deviceContext->DrawIndexed(_indexCount, 0, 0);

	return;
}