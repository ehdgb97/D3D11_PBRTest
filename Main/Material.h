#pragma once
#include "ConstantBuffers.h"

struct CB_Transform;
using namespace DirectX::SimpleMath;
using namespace std;
struct aiMaterial;
class TextureImage;

class Material
{
public:
	Material();
	~Material();
	CB_Material m_MaterialCB;

	
	void Create(aiMaterial* pMaterial);
	void Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB);
	//std::shared_ptr<TextureImage> m_pBaseColor;	// 텍스처 리소스 뷰.
	std::shared_ptr<TextureImage> m_pDiffuseRV;
	std::shared_ptr<TextureImage> m_pNormalRV;
	std::shared_ptr<TextureImage> m_pSpecularRV;
	std::shared_ptr<TextureImage> m_pEmissiveRV;
	std::shared_ptr<TextureImage> m_pOpacityRV;
	std::shared_ptr<TextureImage> m_pMetalnessRV;
	std::shared_ptr<TextureImage> m_pRoughnessRV;


};

