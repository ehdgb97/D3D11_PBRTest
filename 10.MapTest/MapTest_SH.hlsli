//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------


Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);

SamplerState samLinear : register(s0);


cbuffer ConstantBuffer_LDH : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;

}

cbuffer LightBuffer_LDH : register(b1)
{
    float3 LightDir;
    float LDPad0;
    float4 LightAmbient;
    float4 LightDiffuse;
    float4 LightSpecular;
    float3 EyePosition;
    bool UseBlinnPhong;

}

cbuffer MarterialBuffer_LDH:register(b2)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float  MaterialSpecularPower;
    bool UseNormalMap;
    bool UseSpecularMap;
    float Materialpad0;

};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 PosModel : VS_POSITION;
    float4 TexCoord : TEXCOORD0;
    float3 NormalModel : NORMAL;
    float3 TangentModel : TANGENT;
};

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 PosProj : SV_POSITION;
    float3 PosWorld : POSITION;
    float4 TexCoord : TEXCOORD0;
    float3 NormalWorld : NORMAL;
    float3 TangentWorld : TANGENT;

};

