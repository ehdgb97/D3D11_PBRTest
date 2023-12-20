//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------


Texture2D txDiffuse : register(t0);
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
    float3 Materialpad0;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 PosModel : VS_POSITION;
    float4 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 PosProj : SV_POSITION;
    float3 PosWorld : POSITION;
    float4 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

