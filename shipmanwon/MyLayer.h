#pragma once

#include <d3d11.h>
#include <vector>
#include <functional>
#include "MyObject.h"

class CMyLayer
{
public:
	CMyLayer() = default;
	CMyLayer(const CMyLayer&) = default;
	~CMyLayer() 
	{
		for (auto& iter : ObjectVector)
		{
			delete iter;
		}
	};

	bool initLayer(ID3D11Device* device, HWND hWnd);

	bool pushBack(CMyObject* object);
	bool renderLayer(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc);
	int	getObjectsSize();

private:
	std::vector<CMyObject*> ObjectVector;
};

