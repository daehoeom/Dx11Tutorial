#include "BitmapClass.h"



BitmapClass::BitmapClass()
	: m_vertexBuffer(NULL),
	m_indexBuffer(NULL),
	m_Texture(NULL)
{
}

BitmapClass::BitmapClass(const BitmapClass& _arg)
{
}

BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Init(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, int _screenWidth, int _screenHeight, WCHAR* _textureFilename,
	int _bitmapWidth, int _bitmapHeight)
{
	m_screenWidth = _screenWidth;
	m_screenHeight = _screenHeight;

	m_bitmapWidth = _bitmapWidth;
	m_bitmapHeight = _bitmapHeight;

	m_previousPosX = -1;
	m_previousPosY = -1;

	if (!InitBuffers(_device))
	{
		return false;
	}

	if (!LoadTexture(_device, _deviceContext, _textureFilename))
	{
		return false;
	}

	return true;
}

void BitmapClass::Destroy()
{
	ReleaseTexture();

	DestroyBuffers();

	return;
}

bool BitmapClass::Render(ID3D11DeviceContext* _deviceContext, int _positionX, int _positionY)
{
	if (!UpdateBuffers(_deviceContext, _positionX, _positionY))
	{
		return false;
	}

	RenderBuffers(_deviceContext);

	return true;
}

int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool BitmapClass::InitBuffers(ID3D11Device* _device)
{
	HRESULT result;
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	// 두 개의 삼각형을 만들어야 하기 때문에
	m_vertexCount = 6;

	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	ZeroMemory(vertices, (sizeof(VertexType) * m_vertexCount));

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// Modelclass와 달리 동적 정점버퍼를 생성하여 정점 버퍼 내부의 값을 수정하게 한다.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = _device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = _device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	safe_delete_array(vertices);
	safe_delete_array(indices);

	return true;
}

void BitmapClass::DestroyBuffers()
{
	safe_release(m_indexBuffer);
	safe_release(m_vertexBuffer);

	return;
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* _deviceContext, int _positionX, int _positionY)
{
	HRESULT result;
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	if ((_positionX == m_previousPosX) && (_positionY == m_previousPosY))
	{
		return true;
	}

	m_previousPosX = _positionX;
	m_previousPosY = _positionY;

	left = (float)((m_screenWidth / 2) * -1) + (float)_positionX;
	right = left + (float)m_bitmapWidth;

	top = (float)(m_screenHeight / 2) - (float)_positionY;
	bottom = top - (float)m_bitmapHeight;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	vertices[0].position = XMFLOAT3(left, top, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(left, top, 0.0f);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	result = _deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		HR(result);
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	_deviceContext->Unmap(m_vertexBuffer, 0);

	safe_delete_array(vertices);

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* _deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool BitmapClass::LoadTexture(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, WCHAR* _filename)
{
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	if (!m_Texture->Init(_device, _deviceContext, ConvertWCtoC(_filename)))
	{
		return false;
	}
	return true;
}

void BitmapClass::ReleaseTexture()
{
	safe_delete_destroy(m_Texture);

	return;
}

