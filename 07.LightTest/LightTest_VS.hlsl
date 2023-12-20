#include "LightTest_BH.hlsli"
// ¡§¡° ºŒ¿Ã¥ı.


//PS_INPUT main(float4 pos : POSITION, float4 color : COLOR)
//{
//    PS_INPUT output;
//    output.pos = mul(pos, World);
//    output.pos = mul(output.pos, View);
//    output.pos = mul(output.pos, Projection);
//    output.color = color;
//    return output;
//}

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = mul(input.pos, World);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
    output.color = input.color;
    output.normal = mul(float4(input.normal,0), World).xyz;
    output.normal = normalize(output.normal);
    return output;
}
////    output.normal = mul(float4(input.normal, 1), World).xyz;
///
//output.normal = mul(input.normal, (float3x3)World).xyz;
///