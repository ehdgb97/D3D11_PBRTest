#include "pch.h"
#include "Animation.h"
#include "Node.h"
#include "TimeSystem.h"

Animation::Animation(Actor* _owner)
{
	m_owner = _owner;

}

Animation::~Animation()
{
}
void Animation::Create(aiAnimation* Animation)
{
	assert(Animation != nullptr);
	m_AnimeinitDuration = Animation->mDuration;
	m_Duration = Animation->mDuration / Animation->mTicksPerSecond;

	for (size_t i = 0; i < Animation->mNumChannels; i++)
	{
		assert(Animation->mChannels[i] != nullptr);
		assert(Animation->mChannels[i]->mNumPositionKeys == Animation->mChannels[i]->mNumRotationKeys);
		assert(Animation->mChannels[i]->mNumRotationKeys == Animation->mChannels[i]->mNumScalingKeys);
		vector<AnimationKey> m_AnimationKeys;
		NodeAnimationinfo animeinfo = { };
		animeinfo.CurrentKey = 0;
		animeinfo.NumKey = Animation->mChannels[i]->mNumPositionKeys;
		m_AnimationKeys.resize(animeinfo.NumKey);
		for (size_t j = 0; j < animeinfo.NumKey; j++)
		{
			aiVectorKey& pos = Animation->mChannels[i]->mPositionKeys[j];
			aiQuatKey& rot = Animation->mChannels[i]->mRotationKeys[j];
			aiVectorKey& scl = Animation->mChannels[i]->mScalingKeys[j];
			assert(pos.mTime == rot.mTime);
			assert(rot.mTime == scl.mTime);

			m_AnimationKeys[j].Time = pos.mTime;
			m_AnimationKeys[j].Position = Vector3(pos.mValue.x, pos.mValue.y, pos.mValue.z);
			m_AnimationKeys[j].Rotation = Quaternion(rot.mValue.x, rot.mValue.y, rot.mValue.z, rot.mValue.w);
			m_AnimationKeys[j].Scaling = Vector3(scl.mValue.x, scl.mValue.y, scl.mValue.z);
		}
		animeinfo.NodeName = Animation->mChannels[i]->mNodeName.C_Str();
		m_Animation.push_back(make_pair(m_AnimationKeys, animeinfo));
	}
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
	//for (size_t i = 0; i < m_Animation.size(); i++)
	//{
	//	if (m_owner->GetRootNode()->FindNode(m_Animation[i].second.NodeName))
	//	{
	//		NodeAnimationinfo nodeAniinfo = m_Animation[i].second;
	//		Vector3 scale, pos;
	//		Quaternion rot;
	//		scale = Vector3::Lerp(
	//			m_Animation[i].first[nodeAniinfo.CurrentKey].Scaling,
	//			m_Animation[i].first[nodeAniinfo.CurrentKey + 1].Scaling,
	//			nodeAniinfo.Ratio);
	//		pos = Vector3::Lerp(
	//			m_Animation[i].first[nodeAniinfo.CurrentKey].Position,
	//			m_Animation[i].first[nodeAniinfo.CurrentKey + 1].Position,
	//			nodeAniinfo.Ratio);
	//		rot = Quaternion::Slerp(
	//			m_Animation[i].first[nodeAniinfo.CurrentKey].Rotation,
	//			m_Animation[i].first[nodeAniinfo.CurrentKey + 1].Rotation,
	//			nodeAniinfo.Ratio);
	//		m_owner->GetRootNode()->FindNode(m_Animation[i].second.NodeName)->SetAnimationMatrix(Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rot) * Matrix::CreateTranslation(pos));
	//	}
	//}
	/*for (size_t i = 0; i < m_owner->GetNodes().size(); i++)
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
	}*/  
}
