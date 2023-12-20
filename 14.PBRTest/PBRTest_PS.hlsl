#include "PBRTest_SH.hlsli"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

static const float PI = 3.141592;
static const float Epsilon = 0.00001;
static const float3 Fdielectric = 0.04;


// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Returns number of mipmap levels for specular IBL environment map.
uint querySpecularTextureLevels()
{
    uint width, height, levels;
    txSpecular.GetDimensions(0, width, height, levels);
    return levels;
}






float4 main(PS_INPUT input) : SV_TARGET
{
    float3 vNormal = normalize(input.NormalWorld);
    float3 vTangent = normalize(input.TangentWorld);
    float3 vBiTanget = cross(vNormal, vTangent);
    

    
    
    if (UseNormalMap)
    {
        // Get current fragment's normal and transform to world space.
        float3 vNormalTangentSpace = txNormal.Sample(samLinear, input.TexCoord).rgb * 2.0f - 1.0f;
        float3x3 WorldTransform = float3x3(vTangent, vBiTanget, vNormal);
        
        
        vNormal = mul(vNormalTangentSpace, WorldTransform);
        vNormal = normalize(vNormal);
    }
    
    
    float4 fTxDiffuse = txDiffuse.Sample(samLinear, input.TexCoord);
    float fNDotL = max(dot(vNormal, -LightDir), 0);
    float3 vView = normalize(EyePosition - input.PosWorld.xyz);
    float4 Ambient = LightAmbient * MaterialAmbient;
    float4 Diffuse = LightDiffuse * MaterialDiffuse * fNDotL;
    if (UseDiffuseMap)
    {
        Diffuse *= fTxDiffuse;
        Ambient *= fTxDiffuse;
    }
    
    float3 albedo;
    float Metalness;
    float Roughness;
    float4 Emissive = 0;
    float Opacity = 1.0f;
    float4 Specular_tx = { 1, 1, 1, 1 };
    
    if (UseDiffuseMap)
        albedo = txDiffuse.Sample(samLinear, input.TexCoord).rgb;
    if (UseEmissiveMap)
        Emissive = txEmissive.Sample(samLinear, input.TexCoord);
    if (UseOpacityMap)
        Opacity = txOpacity.Sample(samLinear, input.TexCoord).a;
    if (UseSpecularMap)
        Specular_tx = txSpecular.Sample(samLinear, input.TexCoord);
    if (UseMetalnessMap)
        Metalness = txMetalness.Sample(samLinear, input.TexCoord).r;
    if (UseRoughnessMap)
        Roughness = txRoughness.Sample(samLinear, input.TexCoord).r;
    
 
    //==========================================
    //==============             |==============
    //==============     PBR     |==============
    //============== ____________|==============
    //==========================================
    
    // Angle between surface normal and outgoing light direction.
    float cosLo /*= max(0.0, dot(vNormal, vView))*/;
    if (UseBlinnPhong)      //BlinnPhong
    {
        float3 HalfVector = normalize(-LightDir + vView);
        cosLo = max(dot(HalfVector, vNormal), 0);
    }
    else //Pong 
    {
        float3 vReflection = reflect(LightDir, vNormal);
        cosLo = max(dot(vReflection, vView), 0);
    }
    
	//// Specular reflection vector.
 //   float3 Lr = 2.0 * cosLo * vNormal - vView;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
    float3 F0 = lerp(Fdielectric, albedo, Metalness);
    float3 directLighting = 0.0;
    
    float3 Li = -LightDir;
    float3 Lradiance = LightDiffuse.rgb;

	// Half-vector between Li and vView.
    float3 Lh = normalize(Li + vView);

		// Calculate angles between surface normal and various light vectors.
    float cosLi = max(0.0, dot(vNormal, Li));
    float cosLh = max(0.0, dot(vNormal, Lh));

	// Calculate Fresnel term for direct lighting. 
    float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, vView)));
	// Calculate normal distribution for specular BRDF.
    float D = ndfGGX(cosLh, Roughness);
	// Calculate geometric attenuation for specular BRDF.
    float G = gaSchlickGGX(cosLi, cosLo, Roughness);

	// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
	// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
	// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), Metalness);

	// Lambert diffuse BRDF.
	// We don't scale by 1/PI for lighting & material units to be more convenient.
	// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
    float3 diffuseBRDF = kd * albedo;

	// Cook-Torrance specular microfacet BRDF.
    float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

	// Total contribution for this light.
    directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
    
    //==========================================
    //==============             |==============
    //==============   PBR END   |==============
    //============== ____________|==============
    //==========================================
    
  
    //================================Specular=========================================
    //float4 Specular = pow(fSDot, MaterialSpecularPower) * LightSpecular * MaterialSpecular;
    //Specular *= Specular_tx;
    

    float4 FinalColor = Diffuse + /*Specular +*/Ambient + Emissive;
    FinalColor = float4(directLighting, 1);
    FinalColor = float4(FinalColor.rgb, Opacity);
    return FinalColor;
}
