#include "pch.h"
#include "Animation.h"
#include"GameObject.h"
#include "TimeSystem.h"

Animation::Animation(GameObject* _owner, float Duration)
{
	m_owner = _owner;
	m_AnimeinitDuration =Duration;
	m_Duration = 10;
}

Animation::~Animation()
{
}

void Animation::Create(aiNodeAnim* nodeAnimation)
{

	assert(nodeAnimation != nullptr);
	assert(nodeAnimation->mNumPositionKeys == nodeAnimation->mNumRotationKeys);
	assert(nodeAnimation->mNumRotationKeys == nodeAnimation->mNumScalingKeys);


	vector<AnimationKey> m_AnimationKeys;
	NodeAnimationinfo animeinfo = { };
	animeinfo.CurrentKey = 0;
	animeinfo.NumKey= nodeAnimation->mNumPositionKeys;
	m_AnimationKeys.resize(animeinfo.NumKey);
	for (size_t i = 0;i < animeinfo.NumKey;i++)
	{		
		aiVectorKey& pos = nodeAnimation->mPositionKeys[i];
		aiQuatKey& rot = nodeAnimation->mRotationKeys[i];
		aiVectorKey& scl = nodeAnimation->mScalingKeys[i];

		assert(pos.mTime == rot.mTime);
		assert(rot.mTime == scl.mTime);

		m_AnimationKeys[i].Time = pos.mTime;
		m_AnimationKeys[i].Position = Vector3(pos.mValue.x,pos.mValue.y,pos.mValue.z);
		m_AnimationKeys[i].Rotation = Quaternion(rot.mValue.x, rot.mValue.y, rot.mValue.z, rot.mValue.w);
		m_AnimationKeys[i].Scaling = Vector3(scl.mValue.x,scl.mValue.y,scl.mValue.z);
	}
	
	animeinfo.NodeName = nodeAnimation->mNodeName.C_Str();
	m_Animation.push_back(make_pair(m_AnimationKeys,animeinfo));


}

void Animation::Render()
{

	if (IsPlayed)
	{
		const float deltatime = GameTimer::m_Instance->DeltaTime();
		for (auto& animation : m_Animation)
		{
			NodeAnimationinfo nodeAniinfo = animation.second;
			nodeAniinfo.CurrentTime += deltatime * m_AnimeinitDuration / m_Duration;

			///deltatime 오류 수정 용도
			if (nodeAniinfo.CurrentTime > m_AnimeinitDuration)
			{
				nodeAniinfo.CurrentTime = 0;
				nodeAniinfo.CurrentKey = 0;
			}

			if (nodeAniinfo.CurrentTime > animation.first[nodeAniinfo.CurrentKey + 1].Time)
			{
				nodeAniinfo.CurrentKey++;
				if (nodeAniinfo.CurrentKey == (nodeAniinfo.NumKey - 1))
				{
					nodeAniinfo.CurrentKey = 0;
					nodeAniinfo.CurrentTime = 0;
				}
			}
			int inter = animation.first[nodeAniinfo.CurrentKey + 1].Time - animation.first[nodeAniinfo.CurrentKey].Time;
			nodeAniinfo.Ratio =
				(nodeAniinfo.CurrentTime - animation.first[nodeAniinfo.CurrentKey].Time) / inter;


			animation.second = nodeAniinfo;
		}
	}
}

void Animation::Update()
{
	
	for (size_t i = 0; i < m_owner->GetNodes().size(); i++)
	{
		for (size_t j = 0; j < m_Animation.size(); j++)
		{
			if (m_owner->GetNodes()[i]->GetName() == m_Animation[j].second.NodeName)
			{

				NodeAnimationinfo nodeAniinfo = m_Animation[j].second;
				Vector3 scale, pos;
				Quaternion rot;
				scale = Vector3::Lerp(
					m_Animation[j].first[nodeAniinfo.CurrentKey].Scaling,
					m_Animation[j].first[nodeAniinfo.CurrentKey + 1].Scaling,
					nodeAniinfo.Ratio);
				pos = Vector3::Lerp(
					m_Animation[j].first[nodeAniinfo.CurrentKey].Position,
					m_Animation[j].first[nodeAniinfo.CurrentKey + 1].Position,
					nodeAniinfo.Ratio);
				rot = Quaternion::Slerp(
					m_Animation[j].first[nodeAniinfo.CurrentKey].Rotation,
					m_Animation[j].first[nodeAniinfo.CurrentKey + 1].Rotation,
					nodeAniinfo.Ratio);
				m_owner->GetNodes()[i]->SetAnimationMatrix(Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rot) * Matrix::CreateTranslation(pos));

			}
		}
	}  
}
