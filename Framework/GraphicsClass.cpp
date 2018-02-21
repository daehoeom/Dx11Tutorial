#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
	: m_Direct3D(NULL),
	m_Model(NULL),
	//m_ColorShader(NULL)
	//m_TextureShader(NULL)
	m_LightShader(NULL),
	m_Light(NULL)
{
}

GraphicsClass::GraphicsClass(const GraphicsClass& arg)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Init(int _screenWidth, int _screenHeight, HWND _hWnd)
{
	m_Direct3D = new D3DClass;

	if (!m_Direct3D)
	{
		return false;
	}

	if (!m_Direct3D->Init(_screenWidth, _screenHeight, true, _hWnd, false, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(_hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	CameraClass::GetInstance()->SetPosition(0.f, 0.f, -5.0f);

	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	if (!m_Model->Init(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../Texture/stone01.tga"))
	{
		MessageBox(_hWnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	//m_ColorShader = new ColorShaderClass;
	//if (!m_ColorShader)
	//{
		//return false;
	//}

	//if (!m_ColorShader->Init(m_Direct3D->GetDevice(), _hWnd))
	//{
		//MessageBox(_hWnd, L"Could not initialize the color shader object", L"Error", MB_OK);
		//return false;
	//}

	/*m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	if (!m_TextureShader->Init(m_Direct3D->GetDevice(), _hWnd))
	{
		MessageBox(_hWnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
	}*/

	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	if (!m_LightShader->Init(m_Direct3D->GetDevice(), _hWnd))
	{
		MessageBox(_hWnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
	}

	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void GraphicsClass::Destroy()
{
	/*if (m_ColorShader)
	{
		m_ColorShader->Destroy();
		SAFE_DELETE(m_ColorShader);
	}*/

	/*if (m_TextureShader)
	{
		m_TextureShader->Destroy();
		SAFE_DELETE(m_TextureShader);
	}*/

	SAFE_DELETE(m_Light);

	if (m_LightShader)
	{
		m_LightShader->Destroy();
		SAFE_DELETE(m_LightShader);
	}

	if (m_Model)
	{
		m_Model->Destroy();
		SAFE_DELETE(m_Model);
	}

	if (m_Direct3D)
	{
		m_Direct3D->Destroy();
		SAFE_DELETE(m_Direct3D);
	}

	return;
}

bool GraphicsClass::Frame()
{
	if (!Render())
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	CameraClass::GetInstance()->Render();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	CameraClass::GetInstance()->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	/*if (!m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
	{
		return false;
	}*/

	/*if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture()))
	{
		return false;
	}*/

	if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		m_Light->GetDirection(), m_Light->GetDiffuseColor()))
	{
		return false;
	}

	m_Direct3D->EndScene();

	return true;
}
