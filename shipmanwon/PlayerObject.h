#pragma once
#include "MyObject.h"
#include <vector>

class CPlayerObject : public CMyObject
{
public:
	CPlayerObject() = default;
	CPlayerObject(const CPlayerObject&) = default;
	~CPlayerObject() = default;

	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;
	bool	renderObject(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix) override;

	//void	CStageObject::renderBuffers(ID3D11DeviceContext* deviceContext) override;
	void createCylinder(void);
	void createTopCap(void);
	void createBottomCap(void);

private:
	std::wstring textureFilename = std::wstring();
	ID3D11Resource* Resource;

	ID3D11Device*				temp_device = nullptr;
	ID3D11ShaderResourceView*	g_pTextureRV = nullptr;
	ID3D11SamplerState*			g_pSamplerLinear = nullptr;

	HRESULT loadTexture(); //override???
	void createShader();

	void	renderBuffers(ID3D11DeviceContext* deviceContext) override;
	bool	initializeBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices, int m_vertexCount, int m_indexCount);

	int m_vertexCount = 4;
	int m_indexCount = 6;

// 	VertexType vertices[4] =
// 	{
// 		{ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
// 		{ DirectX::XMFLOAT3(0.0f, 4.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
// 		{ DirectX::XMFLOAT3(0.0f, -4.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
// 		{ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }
// 	};

// 	unsigned long indices[6] =
// 	{
// 		0, 1, 2, 2, 1, 3
// 	};

	//////////////////////////////////////////////////////////////////////////
	//원기둥 그리는 변수들
	float bottomRadius;
	float topRadius;
	float height;
	UINT sliceCount;
	UINT stackCount;

	std::vector<VertexType> cyVerticies;
	std::vector<unsigned long> cyIndices;
};

