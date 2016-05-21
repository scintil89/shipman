#include "stdafx.h"
#include "StageObject.h"
#include "config.h"

bool CStageObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (textureFilename.empty() == true)
		textureFilename = textureStage;

	if (IsInit == true)
		return true;

	createStage();
	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;

	loadTexture();
	createShader();

	return result;
}

void CStageObject::shutdown()
{
	CMyObject::shutdown();
	shutdownBuffers();

	return;
}

void CStageObject::createStage()
{
	float topRadius = 250.0f;
	float bottomRadius = 400.0f;
	float delta = bottomRadius - topRadius;

	float height = 400.0f;
	int slice = 8;
	int stack = 5;

	Verticies.clear();
	Indices.clear();

	float theta = XM_2PI / slice;
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.0f) });
	
	for (int i = 0; i <= stack; ++i)
	{
		float a = XM_PI / slice * (i % 2);
		float stackHeight = height / stack * i;
		for (int j = 0; j <= slice; ++j)
		{
			int idx = i%slice;
			float radius = topRadius + delta*i;
			theta += sinf(rand());

			Verticies.push_back({ XMFLOAT3(radius*cosf(theta*idx - a), -stackHeight, radius*sinf(theta*idx - a)), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f) });
		}
	}
	
	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.0f) });

////	up
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, 4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.5f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });
//
////	front
//	Verticies.push_back({ DirectX::XMFLOAT3(6.0f, 0.0f,  0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(6.0f,-20.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,-4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });
//
////	left
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,-4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f,-20.0f,0.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,-0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });
//
////	back
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f,-20.0f,0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,4.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3( 0.0f, 0.0f, 4.0f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(-0.4f, 0.0f,0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });
//
////	right
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f, 0.0f, 4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(1.0f, 0.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(0.0f,-20.0f,4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(6.0f,-20.0f,0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(0.5f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(6.0f, 0.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,	DirectX::XMFLOAT3(0.4f, 0.0f, 0.6f), DirectX::XMFLOAT2(0.5f, 0.0f) });
//
////	bottom
//	Verticies.push_back({ DirectX::XMFLOAT3( 6.0f,-20.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.5f) });
//	Verticies.push_back({ DirectX::XMFLOAT3( 0.0f,-20.0f, 4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.5f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3(-6.0f,-20.0f, 0.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) });
//	Verticies.push_back({ DirectX::XMFLOAT3( 0.0f,-20.0f,-4.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f,-1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.5f) });
	
	//!! temp!! 수정이 시급함! 더 안전하게 만들어야 함!
	unsigned long a[] = {
		// top
		2, 3, 0, 2, 0, 1,
		// front
		6, 7, 4, 6, 4, 5,
		// bottom
		10, 11, 8, 10, 8, 9,
		// back
		14, 15, 12, 14, 12, 13,
		// right
		18, 19, 16, 18, 16, 17,
		// left
		22, 23, 20, 22, 20, 21
	}; 
	
	Indices.insert(Indices.end(), a, a + 36);
}