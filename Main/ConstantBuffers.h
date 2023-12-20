#pragma once
#include <directxtk/SimpleMath.h>

using namespace DirectX::SimpleMath;
struct CB_Material
{
	Vector4 Ambient = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Emissive = { 1.0f,1.0f,1.0f,1.0f };
	float  SpecularPower = 128.f; //4
	bool Use_DiffuseMap = false;
	bool pad0[3] = {};					//8			
	bool Use_NormalMap = false;
	bool pad1[3] = {};//12
	bool Use_SpecularMap = false;
	bool pad2[3] = {};//16
	bool Use_EmissiveMap = false;
	bool pad3[3] = {};//4
	bool Use_OpacityMap = false;
	bool pad4[3] = {};//8
	bool Use_MetalnessMap = false;
	bool pad5[3] = {};//8	
	bool Use_RoughnessMap = false;
	bool pad6[3] = {};//8

};
struct CB_Transform
{
	Matrix mWorld;
	Matrix mView;
	Matrix mProjection;
};
struct CB_DirectionLight
{
	Vector3 Direction = { 0.f,0.f,1.f };
	float pad0 = 0;
	Vector4 Ambient = { 0.f,0.f,0.f,1.f };
	Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
	Vector3 EyePosition;
	bool UseBlinnPhong = false;
	bool pad1[3] = {};
};

struct CB_MatrixPallete
{
	Matrix Array[128];
};