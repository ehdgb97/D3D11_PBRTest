#pragma once
struct aiAnimation;
struct aiNodeAnim;
class Actor;
using namespace std;
using namespace DirectX::SimpleMath;

struct AnimationKey
{
	double Time;
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

	Actor* m_owner;
	bool IsPlayed=true;

public:
	Animation(Actor* _owner);
	~Animation();

public:
	float m_Duration=0;
	double m_AnimeinitDuration=0;
	vector<pair<vector<AnimationKey>,NodeAnimationinfo>> m_Animation;
	void Create(aiAnimation* Animation);
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