//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 3;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);

	m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights[0].m_fRange = 6000.0f;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(0.0f, 5000.0f, 0.0f);
	m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.01f);
	m_pLights[0].m_bEnable = true;

	m_pLights[1].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[1].m_fRange = 6000.0f;
	m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[1].m_xmf4Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 0.0f);
	m_pLights[1].m_xmf3Position = XMFLOAT3(0.0f, 0.5f, 0.0f);
	m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(0.6f, 0.6f, 0.5f);
	m_pLights[1].m_fFalloff = 8.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(120.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(60.0f));
	m_pLights[1].m_bEnable = true;

	m_pLights[2].m_nType = SPOT_LIGHT;
	m_pLights[2].m_fRange = 5000.0f;
	m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[2].m_xmf4Specular = XMFLOAT4(0.9f, 0.9f, 0.9f, 0.0f);
	m_pLights[2].m_xmf3Position = XMFLOAT3(0.0f, 2000.0f, 0.0f);
	m_pLights[2].m_xmf3Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	m_pLights[2].m_xmf3Attenuation = XMFLOAT3(0.0001f, 0.0001f, 0.0001f);
	m_pLights[2].m_fFalloff = 9.0f;
	m_pLights[2].m_fPhi = (float)cos(XMConvertToRadians(120.0f));
	m_pLights[2].m_fTheta = (float)cos(XMConvertToRadians(60.0f));
	m_pLights[2].m_bEnable = true;
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
	BuildDefaultLightsAndMaterials();

	m_ppShaderObjcet = new CShader * [2];

	m_ppShaderObjcet[0] = new CIlluminatedShader();
	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, &m_ppShaderObjcet[0]);
	m_ppShaderObjcet[0]->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	m_ppShaderObjcet[1] = new CTerrainShader();
	m_ppShaderObjcet[1]->CreateShader(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	// 터레인 생성

	/*XMFLOAT3 xmf3Scale(16.0f, 8.0f,16.0f);
	XMFLOAT3 xmf3Pos(-4000.0f, -1500.0f, -4000.0f);*/

	// 터레인 생성
	XMFLOAT3 xmf3Scale(16.0f, 8.0f, 16.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.2f, 0.0f, 0.0f);
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Assets/Image/Terrain/terrain5.raw"), 1025, 1025, 257, 257, xmf3Scale, xmf4Color);

	XMFLOAT3 xmf3Pos(0.0f, 0.0f, 0.0f);
	m_pTerrain->SetPosition(xmf3Pos);
	m_pTerrain->SetShader(m_ppShaderObjcet[1]);

	// 게임 오브젝트 생성
	m_nGameObjects = 0;
	m_ppGameObjects = new CHelicopterObject * [m_nGameObjects];

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Release();
		delete[] m_ppGameObjects;
	}

	if (m_pTerrain) delete m_pTerrain;
	if (m_pLights) delete[] m_pLights;

	
	for (int i = 0; i < 2; i++) 
	{
		m_ppShaderObjcet[i]->Release();
	}
	if (m_ppShaderObjcet) delete[] m_ppShaderObjcet;
}

ID3D12RootSignature* CScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[4];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	
	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[2].Constants.Num32BitValues = 16;
	pd3dRootParameters[2].Constants.ShaderRegister = 3; //terrain
	pd3dRootParameters[2].Constants.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[3].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[3].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void**)&m_pcbMappedLights);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}
	if (m_ppShaderObjcet)
	{
		for (int i = 0; i < 2; i++)
			m_ppShaderObjcet[i]->Release();
	}
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->ReleaseUploadBuffers();
	if (m_pTerrain) m_pTerrain->ReleaseUploadBuffers();
	
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'W': m_ppGameObjects[0]->MoveForward(+1.0f); break;
		case 'S': m_ppGameObjects[0]->MoveForward(-1.0f); break;
		case 'A': m_ppGameObjects[0]->MoveStrafe(-1.0f); break;
		case 'D': m_ppGameObjects[0]->MoveStrafe(+1.0f); break;
		case 'Q': m_ppGameObjects[0]->MoveUp(+1.0f); break;
		case 'R': m_ppGameObjects[0]->MoveUp(-1.0f); break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

bool CScene::ProcessInput(UCHAR* pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i]->Animate(fTimeElapsed, NULL);
	if (m_pLights)
	{
		XMFLOAT3 offset = XMFLOAT3(0, 100, 50);
		XMStoreFloat3(&offset, XMVectorAdd(XMLoadFloat3(&m_pPlayer->GetPosition()), XMLoadFloat3(&offset)));
		m_pLights[1].m_xmf3Position = offset;
		m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
	}
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(3, d3dcbLightsGpuVirtualAddress); //Lights
	
	if (m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera);

	for (int i = 0; i < m_nGameObjects; i++)
	{
		if (m_ppGameObjects[i])
		{
			m_ppGameObjects[i]->Animate(m_fElapsedTime, NULL);
			m_ppGameObjects[i]->UpdateTransform(NULL);
			m_ppGameObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}
}

