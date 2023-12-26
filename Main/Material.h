#pragma once
#include "ConstantBuffers.h"


struct CB_Transform;
using namespace DirectX::SimpleMath;
using namespace std;

struct aiMaterial;


class Material
{
public:
	Material();
	~Material();
	CB_Material m_MaterialCB;

	
	void Create(aiMaterial* pMaterial);
	void Render(ID3D11DeviceContext* m_pDeviceContext, ID3D11BlendState* m_pAlphaBlendState, ID3D11Buffer* m_pMaterialCB);
	ID3D11ShaderResourceView* m_pDiffuseRV = nullptr;	// ≈ÿΩ∫√≥ ∏Æº“Ω∫ ∫‰.
	ID3D11ShaderResourceView* m_pNormalRV = nullptr;	// ≥Î∏ª∏  ∏Æº“Ω∫ ∫‰.
	ID3D11ShaderResourceView* m_pSpecularRV = nullptr;	// Ω∫∆‰≈ß∑Ø∏  ∏Æº“Ω∫ ∫‰.
	ID3D11ShaderResourceView* m_pEmissiveRV = nullptr;	// ¿ÃπÃΩ√∫Í∏  ∏Æº“Ω∫ ∫‰.	
	ID3D11ShaderResourceView* m_pOpacityRV = nullptr;	// ≈ı∏Ì∏  ∏Æº“Ω∫ ∫‰.	
	ID3D11ShaderResourceView* m_pMetalnessRV = nullptr;	// ∏ﬁ≈ª∏Ø ∏Æº“Ω∫ ∫‰.	
	ID3D11ShaderResourceView* m_pRoughnessRV = nullptr;	// ∏ﬁ≈ª∏Ø ∏Æº“Ω∫ ∫‰.	

};

