#ifndef MATERIAL
#define MATERIAL

#include "Common.hlsl"
#include "FunctionLibrary.hlsl"

struct FMaterial
{
    float4 BaseColor; //通用颜色
};

float3 FresnelSchlickMethod(float3 InF0, float3 InVertexNormal, float3 InCameraDirection, float InPow)
{
    return InF0 + (1.f - InF0) * pow(1.0f - saturate(dot(InVertexNormal, InCameraDirection)), InPow);
}

float4 GetFinalColor(MaterialConstantBuffer InMaterial, float2 InTexCoord)
{
    float4 res = InMaterial.BaseColor;
    if (InMaterial.TextureMapIndex >= 0 && InMaterial.TextureMapIndex < MAPCOUNT)
    {
        res = InMaterial.BaseColor * SimpleTexture2DMap[InMaterial.TextureMapIndex].Sample(TextureSampler, InTexCoord);
    }
    
    return res;
}

float3 GetNormal(MaterialConstantBuffer InMaterial, float2 InTexCoord, float3 InTangent, float3 InNormal)
{
    float3 res = InNormal;
    if (InMaterial.NormalMapIndex >= 0 && InMaterial.NormalMapIndex < MAPCOUNT)
    {
        float3 SampleNormal = SimpleTexture2DMap[InMaterial.NormalMapIndex].Sample(AnisotropicSampler, InTexCoord).rgb; //范围都是 0 - 1
        float3x3 TBN = GetTBNMatrix(InTangent, InNormal);
        res = mul(TBN, SampleNormal * 2.f - 1.f);
       // res = mul(SampleNormal * 2.f - 1.f, TBN);
    }
    return normalize(res);

}

// 获取材质高光颜色，贴图或者默认
float4 GetSpecular(MaterialConstantBuffer InMaterial, float2 InTexCoord)
{
    float4 res = float4(InMaterial.SpecularColor, 1.0f);
    if (InMaterial.SpecularMapIndex >= 0 && InMaterial.SpecularMapIndex < MAPCOUNT)
    {
        res = SimpleTexture2DMap[InMaterial.SpecularMapIndex].Sample(TextureSampler, InTexCoord);
    }
    return res;
}

float3 GetReflect(float3 CameraDirection, float3 InUnitWorldNormal)
{
    return reflect(CameraDirection, InUnitWorldNormal);
}

float3 GetReflectionSampleColor(float3 Reflect)
{
    return SampleTextureCubeMap.Sample(TextureSampler, Reflect).xyz;
}

float3 GetShininess(MaterialConstantBuffer MatConstBuffer)
{
    return 1.f - MatConstBuffer.Roughness;
}

float3 FresnelSchlickFactor(MaterialConstantBuffer MatConstBuffer, float3 InUnitWorldNormal, float3 ReflectDirection, float Pow)
{

    return FresnelSchlickMethod(MatConstBuffer.FresnelF0, InUnitWorldNormal, ReflectDirection, Pow);
}

float3 GetReflectionColor(MaterialConstantBuffer MatConstBuffer, float3 InUnitWorldNormal, float3 CameraDirection, float Pow, float3x3 WorldMatrix)
{
    float3 NewReflect = GetReflect(-CameraDirection, InUnitWorldNormal);
    float3 temp = normalize(mul(NewReflect, WorldMatrix)); // 着色器里面都是行向量，所以右乘世界矩阵
    float3 SampleColor = GetReflectionSampleColor(temp);
    float3 Shininess = GetShininess(MatConstBuffer);
    //float3 FresnelFactor = FresnelSchlickFactor(MatConstBuffer, InUnitWorldNormal, temp, Pow);

    return SampleColor * Shininess; // *FresnelFactor
}

#endif