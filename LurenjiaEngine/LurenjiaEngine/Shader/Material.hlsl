#include "Common.hlsl"

struct FMaterial
{
    float4 FinalColor;//ͨ����ɫ
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

float3 GetNormal(MaterialConstantBuffer InMaterial, float2 InTexCoord, float3x3 TBN, float3 InNormal)
{
    float3 res = InNormal;
    if(InMaterial.NormalMapIndex >= 0 && InMaterial.NormalMapIndex < MapCount)
    {
        float3 SampleNormal = SimpleTexture2DMap[InMaterial.NormalMapIndex].Sample(AnisotropicSampler, InTexCoord).rgb; //��Χ���� 0 - 1
        //res = mul(TBN, SampleNormal * 2.f - 1.f);
        res = mul(SampleNormal * 2.f - 1.f, TBN);
    }
    return normalize(res);

}