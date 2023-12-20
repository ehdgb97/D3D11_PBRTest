#include "TextureTest_BH.hlsli"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_TARGET
{

float4 finalColor = txDiffuse.Sample(samLinear, input.tex);
float3 vLightDirect = -normalize(vLightDir.xyz);

finalColor *= saturate(dot(vLightDirect, input.normal) * vLightColor);


finalColor.a = 1;
return finalColor;
return txDiffuse.Sample(samLinear, input.tex);
}
