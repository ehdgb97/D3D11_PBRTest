#include "PhongTest_BH.hlsli"
// ¡§¡° ºŒ¿Ã¥ı.


//PS_INPUT main(float4 PosWorld : POSITION, float4 color : COLOR)
//{
//    PS_INPUT output;
//    output.PosWorld = mul(PosWorld, World);
//    output.PosWorld = mul(output.PosWorld, View);
//    output.PosWorld = mul(output.PosWorld, Projection);
//    output.color = color;
//    return output;
//}

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output= (PS_INPUT)0;
    output.PosProj = mul(input.PosModel, World);
    output.PosWorld = output.PosProj.xyz;
    output.PosProj = mul(output.PosProj, View);
    output.PosProj = mul(output.PosProj, Projection);
    output.tex = input.tex;
    output.normal = normalize(mul(input.normal, (float3x3) World));
    return output;
}
////    output.normal = mul(float4(input.normal, 1), World).xyz;
///
//output.normal = mul(input.normal, (float3x3)World).xyz;
///