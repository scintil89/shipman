#include "stdafx.h"
#include "ResultObject.h"
#include "config.h"
#include <string>

bool CResultObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (pTemp_Device != device)
	{
		pTemp_Device = device;
		IsInit = false;
	}

	if (textureFilename.empty() == true)
	{
		//char *playernum = 
		textureFilename = resultTextureDefault;
		textureFilename += std::to_wstring(winPlayerNum);
		textureFilename += L".png";

	}

	if (IsInit == true)
		return true;

	createObject();
	loadTexture();
	createShader();

	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;

	return result;
}


void CResultObject::shutdown()
{
	shutdownBuffers();

	return;
}

void CResultObject::createObject()
{
	Verticies.clear();
	Indices.clear();

	//	top
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, 1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, 1.5f,-1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f,1.5f,-1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f,1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });

	//	front
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, 1.5f, -1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f,-1.5f, -1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f,-1.5f,-1.5f),DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),  DirectX::XMFLOAT2(0.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f,1.5f, -1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });

	//	bottom
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, -1.5f,-1.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, -1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f,-1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f,-1.5f,-1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });

	//	back
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, -1.5f, 1.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f,  1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f, 1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f,-1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f) });

	//  right
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, 1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f,-1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f,-1.5f,-1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(3.0f, 1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });

	//	left
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f, 1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f,-1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f,-1.5f,-1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) });
	Verticies.push_back({ DirectX::XMFLOAT3(-3.0f, 1.5f, 1.5f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) ,DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) });


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

void CResultObject::SetWinPlayerNum(int set)
{
	winPlayerNum = set;
}
