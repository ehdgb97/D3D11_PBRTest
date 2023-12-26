#pragma once
class TextureImage
{
public:
	TextureImage();
	~TextureImage();

	std::wstring m_FilePath;
	ComPtr<ID3D11ShaderResourceView> m_pTextureRV = nullptr;	// �ؽ�ó ���ҽ� ��.
	void Create(const std::wstring& filePath);
};

