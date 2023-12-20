#include "pch.h"
#include "Helper.h"
#include <comdef.h>
#include <d3dcompiler.h>
#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/WICTextureLoader.h>

LPCWSTR GetComErrorString(HRESULT hr)
{
	_com_error err(hr);
	LPCWSTR errMsg = err.ErrorMessage();
	return errMsg;
}

HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "CompileShaderFromFile", MB_OK);
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

HRESULT CreateTextureFromFile(ID3D11Device* d3dDevice, const wchar_t* szFileName, ID3D11ShaderResourceView** textureView)
{
	HRESULT hr = S_OK;

	// Load the Texture
	hr = DirectX::CreateDDSTextureFromFile(d3dDevice, szFileName, nullptr, textureView);
	if (FAILED(hr))
	{
		hr = DirectX::CreateWICTextureFromFile(d3dDevice, szFileName, nullptr, textureView);
		if (FAILED(hr))
		{
			MessageBoxW(NULL, GetComErrorString(hr), szFileName, MB_OK);
			return hr;
		}
	}
	return S_OK;
}
HRESULT CreateTextureFromColor(ID3D11Device* device, const DirectX::XMFLOAT3& color, ID3D11ShaderResourceView** textureView)
{
	// 텍스처 데이터
	DWORD textureData = 0xFF000000 | // Alpha 값 1로 설정
		((DWORD)(color.z * 255.0f) << 16) | // Blue
		((DWORD)(color.y * 255.0f) << 8) |  // Green
		(DWORD)(color.x * 255.0f);          // Red

	// 텍스처 생성 및 초기화
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 1;
	textureDesc.Height = 1;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32 비트 RGBA
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = &textureData;
	initData.SysMemPitch = sizeof(DWORD);

	ID3D11Texture2D* pTexture = nullptr;
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initData, &pTexture);
	if (FAILED(hr)) {
		return hr;
	}

	// 텍스처 리소스 뷰 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(pTexture, &srvDesc, textureView);

	// 텍스처 사용이 끝났으면 메모리를 해제
	pTexture->Release();

	return hr;
}

DirectX::SimpleMath::Vector3 CalculateTangent(Vector3 v0, Vector3 v1, Vector3 v2, Vector2 tex0, Vector2 tex1, Vector2 tex2)
{
	Vector3 e0 = v1 - v0;
	Vector3 e1 = v2 - v0;
	Vector2 deltaUV1 = tex1 - tex0;
	Vector2 deltaUV2 = tex2 - tex0;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	Vector3 tangent;
	tangent.x = f * (deltaUV2.y * e0.x - deltaUV1.y * e1.x);
	tangent.y = f * (deltaUV2.y * e0.y - deltaUV1.y * e1.y);
	tangent.z = f * (deltaUV2.y * e0.z - deltaUV1.y * e1.z);

	tangent.Normalize();

	return tangent;
}

std::wstring ToWString(const std::string& s)
{
	std::wstring wsTmp(s.begin(), s.end());
	return wsTmp;
}
