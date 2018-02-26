#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
	: m_Direct3D(NULL),
	m_Model(NULL),
	//m_ColorShader(NULL)
	//m_TextureShader(NULL)
	m_LightShader(NULL),
	m_Light(NULL),
	m_Maya(NULL)
{
	ZeroMemory(m_fileName, 256);
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

	CameraClass::GetInstance()->SetPosition(0.f, 0.f, -10.0f);

	m_Maya = new MayaModel;
	if (!m_Maya)
	{
		return false;
	}

	strcpy_s(m_fileName, "../Model/cube.obj");
	m_Maya->GetModelFilename(m_fileName);

	if (!m_Maya->ReadFileCounts())
	{
		return false;
	}

	if (!m_Maya->LoadDataStructures())
	{
		return false;
	}

	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	if (!m_Model->Init(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "../Model/Cube.txt", "../Texture/stone01.tga"))
	{
		MessageBox(_hWnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

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
	
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}

void GraphicsClass::Destroy()
{
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
	static float rotation = 0.0f;

	rotation += (float)XM_PI * 0.005f;

	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	if (!Render(rotation))
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float _rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	CameraClass::GetInstance()->Render();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	CameraClass::GetInstance()->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixScaling(3.0f, 3.0f, 3.0f) * XMMatrixRotationY(_rotation);

	m_Model->Render(m_Direct3D->GetDeviceContext());

	if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), CameraClass::GetInstance()->GetPosition(),
		m_Light->GetSpecularColor(), m_Light->GetSpecularPower()))
	{
		return false;
	}

	m_Direct3D->EndScene();

	return true;
}
