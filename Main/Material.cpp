#include "pch.h"
#include "Helper.h"
#include "Material.h"
#include "ConstantBuffers.h"
#include "D3DRenderManager.h"



Material::Material()
{

}

Material::~Material()
{
	SAFE_RELEASE(m_pDiffuseRV);
	SAFE_RELEASE(m_pNormalRV);
	SAFE_RELEASE(m_pSpecularRV);
	SAFE_RELEASE(m_pEmissiveRV);
	SAFE_RELEASE(m_pOpacityRV);
	SAFE_RELEASE(m_pMetalnessRV);
	SAFE_RELEASE(m_pRoughnessRV);

}

void Material::Create(aiMaterial* pMaterial)
{
	// Diffuse
	aiString texturePath;
	wstring basePath=L"../Resource/";
	std::filesystem::path path;
	wstring finalPath;
	string name = pMaterial->GetName().C_Str();

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		std::string currentExtension = path.extension().string();
		if (currentExtension == ".tga")
		{
			// 확장자 변경
			path.replace_extension(".png");
		}

		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice, finalPath.c_str(), &m_pDiffuseRV));
	}// 텍스처 로딩 시도
	else
	{
		// 텍스처가 없을 경우 기본 재질 설정
		aiColor3D color;
		if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) 
		{
		DirectX::XMFLOAT3 defaultColor = DirectX::XMFLOAT3(color.r, color.g, color.b);
		HR_T(CreateTextureFromColor(D3DRenderManager::m_pDevice, defaultColor, &m_pDiffuseRV));
		}
		// 기본 색상으로 텍스처를 생성
	}
	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		std::string currentExtension = path.extension().string();
		if (currentExtension == ".tga") 
		{
			// 확장자 변경
			path.replace_extension(".png");
		}
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice, finalPath.c_str(), &m_pNormalRV));
	}

	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice, finalPath.c_str(), &m_pSpecularRV));
	}


	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice, finalPath.c_str(), &m_pEmissiveRV));
	}


	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_OPACITY, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice, finalPath.c_str(), &m_pOpacityRV));
	}
	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_METALNESS, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice, finalPath.c_str(), &m_pMetalnessRV));
	}
	if (AI_SUCCESS == pMaterial->GetTexture(aiTextureType_SHININESS, 0, &texturePath))
	{
		path = ToWString(string(texturePath.C_Str()));
		finalPath = basePath + path.filename().wstring();
		HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice, finalPath.c_str(), &m_pRoughnessRV));
	}

	/// TextureType 확인용도!!!

	/*for (aiTextureType textureType = aiTextureType_NONE; textureType <= aiTextureType_UNKNOWN; textureType = aiTextureType(textureType + 1))
		if (AI_SUCCESS == pMaterial->GetTexture(textureType, 0, &texturePath))
			path = ToWString(string(texturePath.C_Str()));*/




	return;
}

void Material::Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB)
{
	m_MaterialCB.Use_DiffuseMap = m_pDiffuseRV != nullptr ? true : false;
	m_MaterialCB.Use_NormalMap = m_pNormalRV != nullptr ? true : false;
	m_MaterialCB.Use_SpecularMap = m_pSpecularRV != nullptr ? true : false;
	m_MaterialCB.Use_EmissiveMap = m_pEmissiveRV != nullptr ? true : false;
	m_MaterialCB.Use_OpacityMap = m_pOpacityRV != nullptr ? true : false;
	m_MaterialCB.Use_MetalnessMap = m_pMetalnessRV != nullptr ? true : false;
	m_MaterialCB.Use_RoughnessMap = m_pRoughnessRV != nullptr ? true : false;
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pDiffuseRV);
	m_pDeviceContext->PSSetShaderResources(1, 1, &m_pNormalRV);
	m_pDeviceContext->PSSetShaderResources(2, 1, &m_pSpecularRV);
	m_pDeviceContext->PSSetShaderResources(3, 1, &m_pEmissiveRV);
	m_pDeviceContext->PSSetShaderResources(4, 1, &m_pOpacityRV);
	m_pDeviceContext->PSSetShaderResources(5, 1, &m_pMetalnessRV);
	m_pDeviceContext->PSSetShaderResources(6, 1, &m_pRoughnessRV);






	if (m_MaterialCB.Use_OpacityMap)
		m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState, nullptr, 0xffffffff); // 알파블렌드 상태설정 , 다른옵션은 기본값 
	else
		m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);	// 설정해제 , 다른옵션은 기본값

	m_pDeviceContext->UpdateSubresource(m_pMaterialCB, 0, nullptr, &m_MaterialCB, 0, 0);


}
