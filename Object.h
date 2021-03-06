//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"

#define DIR_FORWARD					0x01
#define DIR_BACKWARD				0x02
#define DIR_LEFT					0x04
#define DIR_RIGHT					0x08
#define DIR_UP						0x10
#define DIR_DOWN					0x20

class CShader;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40

struct MATERIALLOADINFO
{
	XMFLOAT4						m_xmf4AlbedoColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4						m_xmf4EmissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4SpecularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	float							m_fGlossiness = 0.0f;
	float							m_fSmoothness = 0.0f;
	float							m_fSpecularHighlight = 0.0f;
	float							m_fMetallic = 0.0f;
	float							m_fGlossyReflection = 0.0f;

	UINT							m_nType = 0x00;

	//char							m_pstrAlbedoMapName[64] = { '\0' };
	//char							m_pstrSpecularMapName[64] = { '\0' };
	//char							m_pstrMetallicMapName[64] = { '\0' };
	//char							m_pstrNormalMapName[64] = { '\0' };
	//char							m_pstrEmissionMapName[64] = { '\0' };
	//char							m_pstrDetailAlbedoMapName[64] = { '\0' };
	//char							m_pstrDetailNormalMapName[64] = { '\0' };
};

struct MATERIALSLOADINFO
{
	int								m_nMaterials = 0;
	MATERIALLOADINFO				*m_pMaterials = NULL;
};

class CMaterialColors
{
public:
	CMaterialColors() { }
	CMaterialColors(MATERIALLOADINFO *pMaterialInfo);
	virtual ~CMaterialColors() { }

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	XMFLOAT4						m_xmf4Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	XMFLOAT4						m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f); //(r,g,b,a=power)
	XMFLOAT4						m_xmf4Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
};

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

private:
	int								m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	CShader							*m_pShader = NULL;

	CMaterialColors					*m_pMaterialColors = NULL;

	void SetMaterialColors(CMaterialColors *pMaterialColors);
	void SetShader(CShader *pShader);
	void SetIlluminatedShader() { SetShader(m_pIlluminatedShader); }

	void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList);

protected:
	static CShader					*m_pIlluminatedShader;

public:
	static void CMaterial::PrepareShaders(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, CShader** shader);
};

class CGameObject
{
public:

	XMFLOAT4X4						m_xmf4x4Transform;
	CGameObject* m_pParent = NULL;
	CGameObject* m_pChild = NULL;
	CGameObject* m_pSibling = NULL;

	void UpdateTransform(XMFLOAT4X4* pxmf4x4Parent);
	void ResetTransform() {
		m_xmf4x4Transform._11 = 1; m_xmf4x4Transform._12 = 0; m_xmf4x4Transform._13 = 0; m_xmf4x4Transform._14 = 0;
		m_xmf4x4Transform._21 = 0; m_xmf4x4Transform._22 = 1; m_xmf4x4Transform._23 = 0; m_xmf4x4Transform._24 = 0;
		m_xmf4x4Transform._31 = 0; m_xmf4x4Transform._32 = 0; m_xmf4x4Transform._33 = 1; m_xmf4x4Transform._34 = 0;
		m_xmf4x4Transform._41 = 0; m_xmf4x4Transform._42 = 0; m_xmf4x4Transform._43 = 0; m_xmf4x4Transform._44 = 1;
	}

	//==============================

	//???? ?????? ????????. 
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	//???? ?????? ?????? ????????. 
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	BoundingOrientedBox				OOBB;	// ???? ?????? ????
public:
	CGameObject(int nMeshes = 1);
	virtual ~CGameObject();
	//???? ?????? ???? ???? ?????? ???????? ???? ???? ?????? ?????? ???????? ???? ???????? ?? ????????.
	CMesh** m_ppMeshes = NULL;
	int m_nMeshes = 0;

public:
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);

private:
	int m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

protected:
	XMFLOAT4X4 m_xmf4x4World;
	// CMesh* m_pMesh = NULL;
	CShader* m_pShader = NULL;

public:
	void ReleaseUploadBuffers();
	virtual void SetMesh(int nIndex, CMesh* pMesh);	
	virtual void SetShader(CShader* pShader);
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	//???? ?????? ???? ???? ???????? ???? ?????? ????(x-??, y-??, z-??) ?????? ????????. 
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	//???? ?????? ?????? ????????.
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);

	//???? ?????? ???? x-??, y-??, z-?? ???????? ????????.
	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	//???? ?????? ????(x-??, y-??, z-??)????.
	virtual void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
};

class CHelicopterObject : public CGameObject
{
protected:
	CGameObject* m_pMainRotorFrame = NULL;
	CGameObject* m_pTailRotorFrame = NULL;

private:
	int								m_nReferences = 0;

public:
	void AddRef();
	void Release();

public:
	CHelicopterObject();
    virtual ~CHelicopterObject();

public:
	float		obj_speed = 1;
	bool		new_target = false;
	XMFLOAT3	search_vector;
	bool		Target_On = false;	// ?????? ???? ???? ????
	int			MyTargetNumber = -1;
	XMFLOAT3	start_position;

	char							m_pstrFrameName[64];

	CMesh							*m_pMesh = NULL;

	int								m_nMaterials = 0;
	CMaterial						**m_ppMaterials = NULL;

	/*XMFLOAT4X4					m_xmf4x4Transform;
	XMFLOAT4X4						m_xmf4x4World;*/

	CHelicopterObject 					*m_pParent = NULL;
	CHelicopterObject 					*m_pChild = NULL;
	CHelicopterObject 					*m_pSibling = NULL;

	virtual void SetMesh(CMesh *pMesh);
	virtual void SetShader(CShader* pShader);
	virtual void SetShader(int nMaterial, CShader* pShader);
	virtual void SetMaterial(int nMaterial, CMaterial* pMaterial);
	
	void SetChild(CHelicopterObject* pChild, bool bReferenceUpdate = false);

	virtual void BuildMaterials(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList) { }

	virtual void OnInitialize();
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4 *pxmf4x4Parent=NULL);
	virtual void AnimateEngine(float fTimeElapsed, XMFLOAT4X4 *pxmf4x4Parent=NULL);

	virtual void OnPrepareRender() { }
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera=NULL);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, XMFLOAT4X4 *pxmf4x4World);
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList *pd3dCommandList, CMaterial *pMaterial);

	virtual void ReleaseUploadBuffers();

	void SetScale(float x, float y, float z);

	void SetRotation(XMFLOAT3* axis, float angle);
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
	void Rotate(XMFLOAT4 *pxmf4Quaternion);

	CHelicopterObject *GetParent() { return(m_pParent); }
	void UpdateTransform(XMFLOAT4X4 *pxmf4x4Parent=NULL);
	CHelicopterObject *FindFrame(char *pstrFrameName);

	UINT GetMeshType() { return((m_pMesh) ? m_pMesh->GetType() : 0); }

public:
	static MATERIALSLOADINFO *LoadMaterialsInfoFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, FILE *pInFile);
	static CMeshLoadInfo *LoadMeshInfoFromFile(FILE *pInFile);

	static CHelicopterObject *LoadFrameHierarchyFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, FILE *pInFile);
	static CHelicopterObject *LoadGeometryFromFile(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, ID3D12RootSignature *pd3dGraphicsRootSignature, char *pstrFileName);

	static void PrintFrameInfo(CHelicopterObject *pGameObject, CHelicopterObject *pParent);

public:
	BoundingOrientedBox oobb;
};

class CTerrainObject
{
public:
	//???? ?????? ????????. 
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	//???? ?????? ?????? ????????. 
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

public:
	CTerrainObject(int nMeshes = 1);
	virtual ~CTerrainObject();
	//???? ?????? ???? ???? ?????? ???????? ???? ???? ?????? ?????? ???????? ???? ???????? ?? ????????.
	CTerrainMesh** m_ppMeshes = NULL;
	int m_nMeshes = 0;

public:
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);

private:
	int m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

protected:
	XMFLOAT4X4 m_xmf4x4World;
	// CMesh* m_pMesh = NULL;
	CShader* m_pShader = NULL;

public:
	void ReleaseUploadBuffers();
	virtual void SetMesh(int nIndex, CTerrainMesh* pMesh);
	virtual void SetShader(CShader* pShader);
	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	//???? ?????? ???? ???? ???????? ???? ?????? ????(x-??, y-??, z-??) ?????? ????????. 
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	//???? ?????? ?????? ????????.
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);

	//???? ?????? ???? x-??, y-??, z-?? ???????? ????????.
	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	//???? ?????? ????(x-??, y-??, z-??)????.
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
};

class CHeightMapTerrain : public CTerrainObject
{
public:
	CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color);
	virtual ~CHeightMapTerrain();

private:
	//?????? ???? ?????? ?????? ??????????.
	CHeightMapImage* m_pHeightMapImage;
	//???? ???? ?????? ???? ????????. 
	int m_nWidth;
	int m_nLength;
	//?????? ?????? ?? ?? ?????? ???????? ???????? ?????? ????????. 
	XMFLOAT3 m_xmf3Scale;

public:
	//?????? ?????? ???????? ????????(???? ??????). ???? ???? ?????? ???????? y?? ???? ??????. 
	float GetHeight(float x, float z) { return(m_pHeightMapImage->GetHeight(x / m_xmf3Scale.x, z / m_xmf3Scale.z) * m_xmf3Scale.y); }

	//?????? ???? ?????? ???????? ????????(???? ??????). ???? ???? ???? ?????? ????????.
	XMFLOAT3 GetNormal(float x, float z) {
		return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z)));
	}

	int GetHeightMapWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMapImage->GetHeightMapLength()); }
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }

	//?????? ????(????/????)?? ????????. ???? ???? ?????? ???????? ???? ??????. 
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }
};

class CApacheObject : public CHelicopterObject
{
public:
	CApacheObject();
	virtual ~CApacheObject();

public:
	virtual void OnInitialize();
	virtual void Animate(float fTimeElapsed, XMFLOAT4X4* pxmf4x4Parent = NULL);
};