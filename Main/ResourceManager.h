#pragma once
#include "Mesh.h"
#include "SkeletalMesh.h"
#include"Animation.h"
#include<string>

class ResourceManager
{
	vector<SkeletalMesh*> m_resouce_SkeletalMesh;
	vector<pair<vector<Animation*>, string>> m_resouce_Animation;

};

