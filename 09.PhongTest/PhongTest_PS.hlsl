#include "PhongTest_BH.hlsli"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_TARGET
{


	float3 vNormal = normalize(input.normal);
	float fNDotL = max(dot(vNormal, -LightDir), 0);
	float3 vView = normalize(EyePosition - input.PosWorld.xyz);
	float4 Ambient = LightAmbient * MaterialAmbient;
	float4 Diffuse = LightDiffuse * MaterialDiffuse * (txDiffuse.Sample(samLinear, input.tex) * fNDotL)/*finalColor*/;

	//float4 finalColor = txDiffuse.Sample(samLinear, input.tex);

	////finalColor *= saturate(fNDotL * LightColor);
	//finalColor.a = 1;

    float fSDot = 0.0f;
    if (UseBlinnPhong)      //BlinnPhong
    {   
        float3 HalfVector = normalize(-LightDir + vView);
        fSDot = max(dot(HalfVector, vNormal), 0);
    }
    else                    //Pong 
    {
        float3 vReflection = reflect(LightDir, vNormal);
        fSDot = max(dot(vReflection, vView), 0);
    }
    float4 Specular = pow(fSDot, MaterialSpecularPower) * LightSpecular * MaterialSpecular;

    float4 FinalColor = Diffuse + Specular + Ambient;


	return FinalColor;
}
