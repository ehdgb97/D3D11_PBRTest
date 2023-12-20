#include "LightTest_BH.hlsli"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_TARGET
{

float4 finalColor = input.color;

finalColor += saturate(dot((float3) vLightDir, input.normal) * vLightColor);


finalColor.a = 1;
return finalColor;
}
