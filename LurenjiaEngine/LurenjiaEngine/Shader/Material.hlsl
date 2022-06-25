#include "Common.hlsl"
#include "FunctionLibrary.hlsl"

struct FMaterial
{
    float4 FinalColor;//通用颜色
};

float3 FresnelSchlickMethod(float3 InF0, float3 InVertexNormal, float3 InCameraDirection, float InPow)
{
    return InF0 + (1.f - InF0) * pow(1.0f - saturate(dot(InVertexNormal, InCameraDirection)), InPow);
}

float4 GetFinalColor(MaterialConstantBuffer InMaterial, float2 InTexCoord)
{
    float4 res = InMaterial.BaseColor;
    if (InMaterial.TextureMapIndex >= 0 && InMaterial.TextureMapIndex < MapCount)
    {
        res = InMaterial.BaseColor * SimpleTexture2DMap[InMaterial.TextureMapIndex].Sample(TextureSampler, InTexCoord);
    }
    
    return res;
}

float3 GetNormal(MaterialConstantBuffer InMaterial, float2 InTexCoord, float3 InTangent, float3 InNormal)
{
    float3 res = InNormal;
    if(InMaterial.NormalMapIndex >= 0 && InMaterial.NormalMapIndex < MapCount)
    {
        float3 SampleNormal = SimpleTexture2DMap[InMaterial.NormalMapIndex].Sample(AnisotropicSampler, InTexCoord).rgb; //范围都是 0 - 1
        float3x3 TBN = GetTBNMatrix(InTangent, InNormal);
        res = mul(TBN, SampleNormal * 2.f - 1.f);
       // res = mul(SampleNormal * 2.f - 1.f, TBN);
    }
    return normalize(res);

}

float4 GetSpecular(MaterialConstantBuffer InMaterial, float2 InTexCoord )
{
    float4 res = float4(InMaterial.SpecularColor, 1.0f);
    if (InMaterial.SpecularMapIndex >= 0 && InMaterial.SpecularMapIndex < MapCount)
    {
        res = SimpleTexture2DMap[InMaterial.SpecularMapIndex].Sample(TextureSampler, InTexCoord);
    }
    return res;
}