#pragma once
class FullSkeletonMesh;
class Animation;
class TextureImage;
class StaticMesh;
class ResourceManager
{
public:
	std::map<std::string, std::weak_ptr<FullSkeletonMesh>> m_resource_FullSkeletonMesh;
	std::map<std::string, std::weak_ptr<Animation>> m_resource_Animation;
	std::map<std::string, std::weak_ptr<StaticMesh>>m_resource_StaticMesh;
	std::map<std::wstring, std::weak_ptr<TextureImage>> m_TextureImageMap;
	ResourceManager();
	~ResourceManager();
	static ResourceManager* Instance;
	std::shared_ptr<FullSkeletonMesh> Search_FullSkeletonMesh(std::string _Filename);
	std::shared_ptr<Animation> Search_Animation(std::string _Filename);
	std::shared_ptr<TextureImage> Search_TextureImage(std::wstring _Filename);
	std::shared_ptr<StaticMesh> Search_StaticMesh(std::string _Filename);
};
