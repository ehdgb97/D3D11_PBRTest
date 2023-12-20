#include "FBXTest_SH.hlsli"
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

    output.TexCoord = input.TexCoord;
    output.NormalWorld = normalize(mul(input.NormalModel, (float3x3) World));
    output.TangentWorld = normalize(mul(input.TangentModel, (float3x3) World));
    return output;
}
