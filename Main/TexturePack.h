#pragma once
class TextureImage
{
public:
	TextureImage();
	~TextureImage();

	std::wstring m_FilePath;
	ComPtr<ID3D11ShaderResourceView> m_pTextureRV = nullptr;	// 텍스처 리소스 뷰.
	void Create(const std::wstring& filePath);
};

