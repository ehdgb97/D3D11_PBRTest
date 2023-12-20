//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer_LDH : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 vLightDir;
    float4 vLightColor;
    float4 vOutputColor;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 pos : VS_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

