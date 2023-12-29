#include "pch.h"
#include "TexturePack.h"
#include "D3DRenderManager.h"
#include "Helper.h"

TextureImage::TextureImage()
{}
TextureImage::~TextureImage()
{
}

void TextureImage::Create(const std::wstring& filePath)
{
	HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice, filePath.c_str(), &m_pTextureRV));
	m_FilePath = filePath;
}
