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

// 두 객체간 충돌 체크
bool CScene::isCollision(CHelicopterObject* other)
{
	ContainmentType colltype = m_pPlayer->OOBB.Contains(other->OOBB);
	switch (colltype)
	{
	case DISJOINT:	// 밖에 있는 경우
		break;
	case INTERSECTS:// 교차된 경우
		return true;
		break;
	case CONTAINS:	// 포함된 경우
		return true;
		break;
	default:
		break;
	}

	return false;
}

bool CScene::isOtherCollision(CHelicopterObject* other, int my_number)
{
	for (int i = 0; i < 4; ++i) {
		if (my_number == i) continue;

		ContainmentType colltype = other->OOBB.Contains(m_ppGameObjects[i]->OOBB);
		switch (colltype)
		{
		case DISJOINT:	// 밖에 있는 경우
			break;
		case INTERSECTS:// 교차된 경우
			return true;
			break;
		case CONTAINS:	// 포함된 경우
			return true;
			break;
		default:
			break;
		}
	}

	return false;
}

// 미사일의 목표물 지정함수
int CScene::Target_On(CHelicopterObject* my)
{
	// 헬리콥터의 개수
	for (int i = 0; i < 4; ++i) {
		ContainmentType colltype = my->OOBB.Contains(m_ppGameObjects[i]->OOBB);
		switch (colltype)
		{
		case DISJOINT:	// 밖에 있는 경우	-> Target Off
			break;
		case INTERSECTS:// 교차된 경우		-> Target On
			my->MyTargetNumber = i;
			if (Vector3::Length(Vector3::Subtract(my->GetPosition(), m_ppGameObjects[i]->GetPosition())) < 15.0f)
			{
				m_ppGameObjects[4]->Target_On = false;
				m_ppGameObjects[4]->MyTargetNumber = -1;
				m_ppGameObjects[4]->ResetTransform();
				m_ppGameObjects[4]->SetScale(1.0f, 1.0f, 1.0f);

				m_ppGameObjects[i]->SetPosition(m_ppGameObjects[i]->start_position);
			}
			return 0;
			break;
		case CONTAINS:	// 포함된 경우		-> Target On
			break;
		default:
			break;
		}
	}
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
	XMFLOAT3 xmf3Scale(16.0f, 8.0f, 16.0f);
	XMFLOAT4 xmf4Color(0.0f, 0.2f, 0.0f, 0.0f);
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Assets/Image/Terrain/terrain5.raw"), 1025, 1025, 257, 257, xmf3Scale, xmf4Color);

	XMFLOAT3 xmf3Pos(0.0f, 0.0f, 0.0f);
	m_pTerrain->SetPosition(xmf3Pos);
	m_pTerrain->SetShader(m_ppShaderObjcet[1]);

	// 게임 오브젝트 생성
	m_nGameObjects = 5;
	m_ppGameObjects = new CHelicopterObject * [m_nGameObjects];

	{
		CHelicopterObject* pGameObject = CHelicopterObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Helicopters/Apache.bin");
		CHelicopterObject* pApacheObject = new CHelicopterObject();
		pApacheObject = new CApacheObject();
		pApacheObject->SetChild(pGameObject, true);
		pApacheObject->OnInitialize();
		pApacheObject->SetPosition(m_pTerrain->GetWidth() / 2.0f, 1000.0f, m_pTerrain->GetWidth());
		pApacheObject->Rotate(0.0f, 0.0f, 0.0f);
		pApacheObject->OOBB.Center = pApacheObject->GetPosition();
		pApacheObject->OOBB.Extents = { 50.0f, 50.0f, 50.0f };
		pApacheObject->start_position = pApacheObject->GetPosition();
		pApacheObject->obj_speed = 15.0f;

		m_ppGameObjects[0] = pApacheObject;
	}
	{
		CHelicopterObject* pGameObject = CHelicopterObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Helicopters/Apache.bin");
		CHelicopterObject* pApacheObject = new CHelicopterObject();
		pApacheObject = new CApacheObject();
		pApacheObject->SetChild(pGameObject, true);
		pApacheObject->OnInitialize();
		pApacheObject->SetPosition(m_pTerrain->GetWidth() / 2.0f, 1000.0f, m_pTerrain->GetWidth() * 2.0f);
		pApacheObject->Rotate(0.0f, 0.0f, 0.0f);
		pApacheObject->OOBB.Center = pApacheObject->GetPosition();
		pApacheObject->OOBB.Extents = { 50.0f, 50.0f, 50.0f };
		pApacheObject->start_position = pApacheObject->GetPosition();
		pApacheObject->obj_speed = 15.0f;

		m_ppGameObjects[1] = pApacheObject; 
	}
	{
		CHelicopterObject* pGameObject = CHelicopterObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Helicopters/Apache.bin");
		CHelicopterObject* pApacheObject = new CHelicopterObject();
		pApacheObject = new CApacheObject();
		pApacheObject->SetChild(pGameObject, true);
		pApacheObject->OnInitialize();
		pApacheObject->SetPosition(m_pTerrain->GetWidth() / 2.0f, 1000.0f, m_pTerrain->GetWidth() * 4.0f);
		pApacheObject->Rotate(0.0f, 0.0f, 0.0f);
		pApacheObject->OOBB.Center = pApacheObject->GetPosition();
		pApacheObject->OOBB.Extents = { 50.0f, 50.0f, 50.0f };
		pApacheObject->start_position = pApacheObject->GetPosition();
		pApacheObject->obj_speed = 15.0f;

		m_ppGameObjects[2] = pApacheObject;
	}
	{
		CHelicopterObject* pGameObject = CHelicopterObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Helicopters/Apache.bin");
		CHelicopterObject* pApacheObject = new CHelicopterObject();
		pApacheObject = new CApacheObject();
		pApacheObject->SetChild(pGameObject, true);
		pApacheObject->OnInitialize();
		pApacheObject->SetPosition(m_pTerrain->GetWidth() / 2.0f, 1000.0f, m_pTerrain->GetWidth() * 6.0f);
		pApacheObject->Rotate(0.0f, 0.0f, 0.0f);
		pApacheObject->OOBB.Center = pApacheObject->GetPosition();
		pApacheObject->OOBB.Extents = { 50.0f, 50.0f, 50.0f };
		pApacheObject->start_position = pApacheObject->GetPosition();
		pApacheObject->obj_speed = 15.0f;

		m_ppGameObjects[3] = pApacheObject;
	}

	{
		CHelicopterObject* pGameObject = CHelicopterObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Tree.bin");
		CHelicopterObject* Tree = new CHelicopterObject();
		Tree = new CApacheObject();
		Tree->SetChild(pGameObject, true);
		Tree->OnInitialize();
		Tree->SetPosition(m_pTerrain->GetWidth() / 2.0f, -4000.0f, -4000.0f);
		Tree->Rotate(0.0f, 0.0f, 0.0f);
		Tree->OOBB.Center = Tree->GetPosition();
		Tree->OOBB.Extents = { 50.0f, 50.0f, 50.0f };
		Tree->start_position = Tree->GetPosition();

		m_ppGameObjects[4] = Tree;
	}
	
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
		case VK_SPACE:
			m_ppGameObjects[4]->Target_On = false;
			m_ppGameObjects[4]->MyTargetNumber = -1;
			m_ppGameObjects[4]->ResetTransform();
			m_ppGameObjects[4]->SetScale(1.0f, 1.0f, 1.0f);
			m_ppGameObjects[4]->SetPosition(m_pPlayer->GetPosition());
			break;
		case VK_F4:
			m_pPlayer->SetPosition({ m_pTerrain->GetWidth() / 2, 1000.0f, 100.0f });
			m_ppGameObjects[0]->SetPosition(m_pTerrain->GetWidth() / 2, 1000.0f, 2000.0f);
			m_ppGameObjects[0]->start_position = m_ppGameObjects[0]->GetPosition();
			m_ppGameObjects[1]->SetPosition(m_pTerrain->GetWidth() / 2 - 1000.0f, 1000.0f, 2000.0f);
			m_ppGameObjects[1]->start_position = m_ppGameObjects[1]->GetPosition();
			m_ppGameObjects[2]->SetPosition(m_pTerrain->GetWidth() / 2 + 1000.0f, 1000.0f, 2000.0f);
			m_ppGameObjects[2]->start_position = m_ppGameObjects[2]->GetPosition();
			m_ppGameObjects[3]->SetPosition(m_pTerrain->GetWidth() / 2, 1000.0f - 1000.0f, 2000.0f);
			m_ppGameObjects[3]->start_position = m_ppGameObjects[3]->GetPosition();
;			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

// 플레이어 키 입력 설정
bool CScene::ProcessInput(UCHAR* pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;

	// 게임 오브젝트 애니메이션 - 제대로 실행 됨
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
	pd3dCommandList->SetGraphicsRootConstantBufferView(3, d3dcbLightsGpuVirtualAddress);
	
	if (m_pTerrain) m_pTerrain->Render(pd3dCommandList, pCamera);

	timerate += 1;
	if (timerate == 1200) {	// 대략 20(60 * 20)초에 1번 진입하는 if문
		m_ppGameObjects[spawn_number]->SetPosition(m_pTerrain->GetWidth() / 2.0f, 1000.0f, m_pTerrain->GetWidth());
		timerate = 0;
		spawn_number += 1;
		if (spawn_number >= 4)
			spawn_number = 0;
	}

	for (int i = 0; i < m_nGameObjects; i++)
	{
		

		if (m_ppGameObjects[i])
		{
			m_ppGameObjects[i]->Animate(m_fElapsedTime, NULL);
			m_ppGameObjects[i]->UpdateTransform(NULL);

			// 이동거리
			XMFLOAT3 dis{};
			
			if (i >= 4) {
				Target_On(m_ppGameObjects[i]);

				if (m_ppGameObjects[i]->MyTargetNumber >= 0) {	// 누군가와 접촉했다면

					// 단위벡터 만큼씩 이동한다.
					dis = Vector3::Normalize(Vector3::Subtract(m_ppGameObjects[m_ppGameObjects[i]->MyTargetNumber]->GetPosition(), m_ppGameObjects[i]->GetPosition()));	// 미사일 -> 적의 방향벡터
					dis = Vector3::Multiply(dis, 1000.0f);
					m_ppGameObjects[i]->SetPosition(Vector3::Add(m_ppGameObjects[i]->GetPosition(), dis));

					// XZ평면 회전
					{
						XMFLOAT3 zero = { 0.0f, 0.0f, 0.0f };
						dis = Vector3::Subtract(zero, dis);
						XMFLOAT3 XxX = { 0.0f, 0.0f, 1.0f };
						float rd2 = acos((XxX.x * dis.x + XxX.z * dis.z) / (1 * (sqrt(dis.x * dis.x + dis.z * dis.z))));
						rd2 = rd2 * (180 / 3.14) + 180.0f;

						XMFLOAT3 axis_Y = { 0.0f, 1.0f, 0.0f };

						if (m_ppGameObjects[i]->GetPosition().x > m_ppGameObjects[m_ppGameObjects[i]->MyTargetNumber]->GetPosition().x)
							rd2 *= -1;
						m_ppGameObjects[i]->SetRotation(&axis_Y, rd2);
					}
					// X축 회전
					{
						float xz_lenght = sqrt(((m_ppGameObjects[m_ppGameObjects[i]->MyTargetNumber]->GetPosition().x - m_ppGameObjects[i]->GetPosition().x) 
												* (m_ppGameObjects[m_ppGameObjects[i]->MyTargetNumber]->GetPosition().x - m_ppGameObjects[i]->GetPosition().x))
											+ ((m_ppGameObjects[m_ppGameObjects[i]->MyTargetNumber]->GetPosition().z - m_ppGameObjects[i]->GetPosition().z) 
												* (m_ppGameObjects[m_ppGameObjects[i]->MyTargetNumber]->GetPosition().z - m_ppGameObjects[i]->GetPosition().z)));

						rd = atan2f((m_ppGameObjects[i]->GetPosition().y - m_ppGameObjects[m_ppGameObjects[i]->MyTargetNumber]->GetPosition().y), xz_lenght);
						rd = rd * (180 / 3.14);

						m_ppGameObjects[i]->Rotate(-rd, 0.0f, 0.0f);
					}

					m_ppGameObjects[i]->SetScale(6.0f, 6.0f, 6.0f);
					m_ppGameObjects[4]->Rotate(rand(), rand(), rand());

					m_ppGameObjects[i]->OOBB.Center = m_ppGameObjects[i]->GetPosition();	// OOBB 박스 위치 갱신
				}
				else if (m_ppGameObjects[i]->MyTargetNumber == -1) {
					m_ppGameObjects[i]->search_vector = Vector3::Normalize(m_pPlayer->GetLook());
					m_ppGameObjects[i]->search_vector = Vector3::Multiply(m_ppGameObjects[i]->search_vector, 4);

					m_ppGameObjects[i]->SetPosition(Vector3::Add(m_ppGameObjects[i]->GetPosition(), m_ppGameObjects[i]->search_vector));
					m_ppGameObjects[i]->OOBB.Center = m_ppGameObjects[i]->GetPosition();	// OOBB 박스 위치 갱신
				}
			}
			else {
				dis = Vector3::Normalize(Vector3::Subtract(m_pPlayer->GetPosition(), m_ppGameObjects[i]->GetPosition()));	// 적 -> 플레이어의 방향벡터
				dis = Vector3::Multiply(dis, m_ppGameObjects[i]->obj_speed);

				// 플레이어와 객체들 충돌 체크 후 조치
				if (isCollision(m_ppGameObjects[i])) {
					m_pPlayer->SetVelocity({ 0.0f, 0.0f, 0.0f });
					m_pPlayer->SetPosition(prev_position);
				}
				else {
					prev_position = m_pPlayer->GetPosition();

					m_ppGameObjects[i]->SetPosition(Vector3::Add(m_ppGameObjects[i]->GetPosition(), dis));
					m_ppGameObjects[i]->OOBB.Center = m_ppGameObjects[i]->GetPosition();
				}

				// 객체와 다른 객체간 충돌인 경우
				if (isOtherCollision(m_ppGameObjects[i], i)) {
					m_ppGameObjects[i]->SetPosition(m_ppGameObjects[i]->start_position);
				}

				// XZ평면 회전
				{
					XMFLOAT3 zero = { 0.0f, 0.0f, 0.0f };
					dis = Vector3::Subtract(zero, dis);
					XMFLOAT3 XxX = { 0.0f, 0.0f, 1.0f };
					float rd2 = acos((XxX.x * dis.x + XxX.z * dis.z) / (1 * (sqrt(dis.x * dis.x + dis.z * dis.z))));
					rd2 = rd2 * (180 / 3.14) + 180.0f;

					XMFLOAT3 axis_Y = { 0.0f, 1.0f, 0.0f };

					if (m_pPlayer->GetPosition().x > m_ppGameObjects[i]->GetPosition().x)
						rd2 *= -1;
					m_ppGameObjects[i]->SetRotation(&axis_Y, rd2);
				}
				// X축 회전
				{
					float xz_lenght = sqrt(((m_pPlayer->GetPosition().x - m_ppGameObjects[i]->GetPosition().x) * (m_pPlayer->GetPosition().x - m_ppGameObjects[i]->GetPosition().x))
						+ ((m_pPlayer->GetPosition().z - m_ppGameObjects[i]->GetPosition().z) * (m_pPlayer->GetPosition().z - m_ppGameObjects[i]->GetPosition().z)));

					rd = atan2f((m_pPlayer->GetPosition().y - m_ppGameObjects[i]->GetPosition().y), xz_lenght);
					rd = rd * (180 / 3.14);

					m_ppGameObjects[i]->Rotate(-rd, 0.0f, 0.0f);
				}
			}

			//객체와 터레인 충돌체크
			XMFLOAT3 xmf3PlayerPosition = m_ppGameObjects[i]->GetPosition();
			float fHeight = GetTerrain()->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z) + 6.0f;	// 현재 맵 위치의 높이

			if (xmf3PlayerPosition.y < fHeight)					// 맵보다 플레이어가 낮을 경우
			{
				// 높이 조정
				xmf3PlayerPosition.y = fHeight;
				m_ppGameObjects[i]->SetPosition(xmf3PlayerPosition);
				m_ppGameObjects[i]->OOBB.Center = xmf3PlayerPosition;	// OOBB 박스 위치 갱신
			}
			
			m_ppGameObjects[i]->Render(pd3dCommandList, pCamera);
		}
		
	}
}
