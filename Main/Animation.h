#pragma once

using namespace std;
using namespace DirectX::SimpleMath;

struct aiNodeAnim;
class GameObject;
struct AnimationKey
{
	float Time;
	Vector3 Position;
	Quaternion Rotation;
	Vector3 Scaling;
};

struct NodeAnimationinfo
{
	string NodeName;
	int CurrentKey=0;
	int NumKey=0;
	float CurrentTime = 0;
	float Ratio=0;

};

class Animation
{
private:
public:

	GameObject* m_owner;
	bool IsPlayed=true;

public:
	Animation(GameObject* _owner, float Duration);
	~Animation();

public:
	float m_Duration;
	float m_AnimeinitDuration;
	vector<pair<vector<AnimationKey>,NodeAnimationinfo>> m_Animation;
	void Create(aiNodeAnim* nodeAnimation);
	void Render();
	void Update();
	float GetDuration();
	void SetDuration(float time);
};
inline float Animation::GetDuration()
{
	return m_Duration;
}
inline void Animation::SetDuration(float time)
{
	m_Duration = time;
}