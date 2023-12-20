//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer_LDH : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

