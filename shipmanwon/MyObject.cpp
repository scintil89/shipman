#include "stdafx.h"
#include "MyObject.h"
#include "config.h"
#include "MyChrono.h"

bool CMyObject::initialize(ID3D11Device* device, HWND hWnd)
{
	if (textureFilename.empty() == true)
		textureFilename = textureDefault;
	
	if (IsInit == true)
		return true;

	//bool result = initializeBuffers(device);
	bool result = initializeBuffers(device);
	if (result == true)
		IsInit = true;

	loadTexture();
	createShader();

	return result;
}

void CMyObject::shutdown()  
{
	// Shutdown the vertex and index buffers.
	shutdownBuffers();

	return;
}

bool CMyObject::renderObject(ID3D11DeviceContext* deviceContext, std::function<bool(ID3D11DeviceContext*, CMyObject*)> setShaderfunc)
{
	bool result = true;
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	
	setShaderfunc(deviceContext, this);
	renderBuffers(deviceContext);
	
	return result;
}

int CMyObject::getIndexCount()
{
	return Indices.size();
}

float CMyObject::calcDistanceTwoPoint(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
	//Scintil
	//계산의 정확도를 위해서 pow나 ^2를 쓰지 않고 2번 곱함
	return sqrt((a.x - b.x)*(a.x - b.x) + ((a.y - b.y)*(a.y - b.y)) + ((a.z - b.z)*(a.z - b.z)));
}

//////////////////////////////////////////////////////////////////////////
//Object Moving
//////////////////////////////////////////////////////////////////////////
void CMyObject::move()
{
	if (OuterSpeed <= 0) {
		OuterSpeed = 0;
		OuterVector = ForwardVector;
	}

	moveForward();
	moveTowardByOuter(OuterVector.x, OuterVector.y, OuterVector.z);
}

void CMyObject::moveToward(float x, float y, float z)
{
	float sum = abs(x) + abs(y) + abs(z);
	DirectX::XMMATRIX temp = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		x / sum, y / sum, z / sum, 0.0f
	};
	ObjectTranslate += temp * CurrentSpeed;

	setCurrentPosition(x / sum*CurrentSpeed, y / sum*CurrentSpeed, z / sum*CurrentSpeed);

	SpeedDelta -= 0.01f;
	if (SpeedDelta < 0.0f)
		SpeedDelta = 0.0f;
	CurrentSpeed = MaximumSpeed * sinf(SpeedDelta);

	ObjectWorld = ObjectScale * ObjectRotate * ObjectTranslate;
}

void CMyObject::moveTowardByOuter(float x, float y, float z)
{
	float sum = abs(x) + abs(y) + abs(z);

	DirectX::XMMATRIX temp = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		x / sum, y / sum, z / sum, 0.0f
	};
	ObjectTranslate += temp * OuterSpeed;

	setCurrentPosition(x / sum*OuterSpeed, y / sum*OuterSpeed, z / sum*OuterSpeed);
	if (OuterSpeed > 0) {
		OuterSpeed -= 0.01f;
	}
	ObjectWorld = ObjectScale * ObjectRotate * ObjectTranslate;
}

void CMyObject::moveForward()
{
	moveToward(ForwardVector.x, ForwardVector.y, ForwardVector.z);
}

void CMyObject::moveBackward() //test function. made by scintil
{
	moveToward(-1 * ForwardVector.x, ForwardVector.y, -1 * ForwardVector.z);
}	

void CMyObject::moveStop()
{
	CurrentSpeed = 0.0f;	
	moveBackward();
}

void CMyObject::accelerate()
{
	if (CurrentSpeed < MaximumSpeed)
	{
		SpeedDelta += 0.015f;

		if (SpeedDelta > DirectX::XM_PI / 2)
			SpeedDelta = DirectX::XM_PI / 2;

		CurrentSpeed = MaximumSpeed * sinf(SpeedDelta);
	}
}

void CMyObject::boost()
{
	//setboostSpeed();
	setMaximumSpeed(1.5f);
	accelerate();
	//moveForward();

	//일정 시간 후에 호출
	
	//std::function<void(void)> bind_resetSpeed(std::bind(&CMyObject::resetSpeed, this));
	//CMyChrono::afterTime(1.0f, bind_resetSpeed);
	//resetSpeed();
}

void CMyObject::resetSpeed()
{
	MaximumSpeed = 0.1f;
}

//////////////////////////////////////////////////////////////////////////
//Set
//////////////////////////////////////////////////////////////////////////
void CMyObject::setCurrentPosition(float x, float y, float z)
{
	currentPosition.x += x;
	currentPosition.y += y;
	currentPosition.z += z;
}

void CMyObject::setRotate(float x, float y, float z)
{
	x /= DirectX::XM_2PI;
	y /= DirectX::XM_2PI;
	z /= DirectX::XM_2PI;

	ForwardTheta.y += y;
	ForwardTheta.y = fmod(ForwardTheta.y, DirectX::XM_2PI);
	ForwardVector.x = 1.0f * cosf(-ForwardTheta.y);
	ForwardVector.z = 1.0f * sinf(-ForwardTheta.y);

	DirectX::XMMATRIX temp = DirectX::XMMatrixIdentity();
	temp = DirectX::XMMatrixRotationX(x);
	temp *= DirectX::XMMatrixRotationY(y);
	temp *= DirectX::XMMatrixRotationZ(z);

	ObjectRotate *= temp;

	ObjectWorld = ObjectScale * ObjectRotate * ObjectTranslate;
}

void CMyObject::setScale(float x, float y, float z)
{
	ObjectScale = DirectX::XMMatrixScaling(x, y, z);

	ObjectWorld = ObjectScale * ObjectRotate * ObjectTranslate;
}

void CMyObject::setTranslate(float x, float y, float z)
{
	setCurrentPosition(x, y, z);

	ObjectTranslate = DirectX::XMMatrixTranslation(x, y, z);

	ObjectWorld = ObjectScale * ObjectRotate * ObjectTranslate;
}

void CMyObject::setColorRGBA(float red, float green, float blue, float alpha)
{
	if (Verticies.size() < 1)
		return;

	for (int i = 0; i < Verticies.size(); ++i)
	{
		Verticies[i].color = DirectX::XMFLOAT4(red, green, blue, alpha);
	}

	// 임시 코드
	if (pTemp_Device == nullptr)
		return;

	initializeBuffers(pTemp_Device);
}

void CMyObject::setForwardVector(float x, float y, float z)
{
	ForwardVector = { x, y, z };
}

void CMyObject::setObjectName(std::string objName)
{
	ObjectName = objName;

}

void CMyObject::setTexture(std::wstring texName)
{
	textureFilename = texName;
	loadTexture();
}

void CMyObject::setCOR(float cor)
{
	ObjectCOR = cor;
}

void CMyObject::setCurrentSpeed(float speed)
{
	CurrentSpeed = speed;
}

void CMyObject::setMaximumSpeed(float speed)
{
	MaximumSpeed = speed;
	CurrentSpeed = speed * 3 / 4;
}

void CMyObject::setForwardTheta(DirectX::XMFLOAT3 theta)
{
	ForwardTheta = theta;
}

void CMyObject::setOuterTheta(DirectX::XMFLOAT3 theta)
{
	OuterTheta = theta;
}

void CMyObject::setOuterVector(float x, float y, float z)
{
	OuterVector.x = x;
	OuterVector.y = y;
	OuterVector.z = z;
}

void CMyObject::setOuterSpeed(float speed)
{
	OuterSpeed = speed;
}

//////////////////////////////////////////////////////////////////////////
//Get
//////////////////////////////////////////////////////////////////////////
std::string CMyObject::getObjectName() const
{
	return ObjectName;
}

float CMyObject::getCurrentSpeed() const
{
	return CurrentSpeed;
}

float CMyObject::getMass() const
{
	return ObjectMass;
}

float CMyObject::getCOR() const
{
	return ObjectCOR;
}

float CMyObject::getOuterSpeed() const
{
	return OuterSpeed;
}

DirectX::XMMATRIX CMyObject::getWorldMatrix() const
{
	return ObjectWorld;
}

DirectX::XMFLOAT3 CMyObject::getMoveTheta() const
{
	return OuterTheta;
}

DirectX::XMFLOAT3 CMyObject::getForwardVector() const
{
	return ForwardVector;
}

DirectX::XMFLOAT3 CMyObject::getForwardTheta() const
{
	return ForwardTheta;
}

//////////////////////////////////////////////////////////////////////////
//private 
//////////////////////////////////////////////////////////////////////////
HRESULT CMyObject::loadTexture()
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(pTemp_Device, textureFilename.c_str(), &Resource, &pTextureRV, NULL);

	//텍스쳐를 생성한 D3D device, 텍스쳐 경로, 추가 이미지 정보(보통 NULL), 자원 적재 스레드 ( 보통 NULL) 생성된 이미지 뷰, 자원 적재 스레드가 NULL 이면 NULL

	if (FAILED(hr))
		return hr;

	D3D11_SAMPLER_DESC	sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = pTemp_Device->CreateSamplerState(&sampDesc, &pSamplerLinear);	// SamplerState 생성
	if (FAILED(hr))
		return hr;

	return S_OK;
}

void CMyObject::createShader()
{
	ID3DBlob* pErrorBlob = NULL;
	ID3DBlob* pVSBlob = NULL;
	HRESULT hr = D3DCompileFromFile(
		FxFile.c_str(), 0, 0,
		"VS", "vs_5_0",
		0, 0,
		&pVSBlob, &pErrorBlob);

	//버텍스 쉐이더 생성
	hr = pTemp_Device->CreateVertexShader(
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		0, &pVertexShader);

	//	해당 D3D_INPUT_ELEMENT_DESC 를 vertex의 struct에 맞춰서 수정해야 함

	D3D11_INPUT_ELEMENT_DESC	layout[] =
	{
		//4번째 인자는 시작 index
		//D3D11_APPEND_ALIGHNED_ELEMENT의 경우 시작 offset 자동 계산

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }

		//	D3D11_APPEND_ALIGNED_ELEMENT 사용시 offset 자동 계산이 됩니다.
	};

	UINT numElements = ARRAYSIZE(layout);

	hr = pTemp_Device->CreateInputLayout(layout, numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&pVertexLayout);

	pVSBlob->Release();

	ID3DBlob* pPSBlob = NULL;

	hr = D3DCompileFromFile(FxFile.c_str(), 0, 0,			// Shader 파일 설정
		"PS", "ps_5_0",											// 컴파일 설정
		0, 0,												// 쉐이더 옵션
		&pPSBlob, &pErrorBlob);								// 리턴

	hr = pTemp_Device->CreatePixelShader(pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		0, &pPixelShader);

	pPSBlob->Release();
}

bool CMyObject::initializeBuffers(ID3D11Device* device)
{
	if (Verticies.size() < 1)
		return true;

	VertexType* vertices = &Verticies[0];
	unsigned long* indices = &Indices[0];

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;

	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * Verticies.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * Indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.

	return true;
}

void CMyObject::shutdownBuffers()
{
	// Release the index buffer.
	if (pIndexBuffer)
	{
		pIndexBuffer->Release();
		pIndexBuffer = 0;
	}

	// Release the vertex buffer.
	if (pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = 0;
	}

	return;
}

void CMyObject::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexType);;
	unsigned int offset = 0;

	deviceContext->IASetInputLayout(pVertexLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(pVertexShader, NULL, 0);
	deviceContext->PSSetShader(pPixelShader, NULL, 0);
	
	//텍스쳐!
	deviceContext->PSSetShaderResources(0, 1, &pTextureRV);
	deviceContext->PSSetSamplers(0, 1, &pSamplerLinear);

	// 생성된 버퍼의 정점들을 실제로 파이프라인으로 공급하려면 버퍼를 장치의 한 입력 슬롯에 묶어야 함
	// 1번째 인자 : 

	deviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.

	deviceContext->DrawIndexed(Indices.size(), 0, 0);

	return;
}