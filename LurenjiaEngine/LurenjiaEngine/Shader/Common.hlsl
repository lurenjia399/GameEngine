#ifndef COMMON
#define COMMON

#include "Light.hlsl"

SamplerState TextureSampler : register(s0); //静态采样器
SamplerState AnisotropicSampler : register(s1); //各向异性采样器

cbuffer ObjectConstBuffer : register(b0) //模型CBV
{
    float4x4 WorldMatrix;
    float4x4 ObjectTextureTransformation;
    
    uint MaterialIndex;
    int xx1;
    int xx2;
    int xx3;

}
cbuffer ViewportConstBuffer : register(b1) //视口CBV
{
    float4 cameraPosition;
    float4x4 ViewProjectionMatrix;
}
cbuffer LightConstantBufferView : register(b2) //灯光CBV
{
    Light SceneLight[16];
}
cbuffer FogConstantBufferView : register(b3) //雾CBV
{
    float3 FogColor;
    float FogStart;

    float FogRange;
    float FogHeight;
    float xx4;
    float xx5;
}

struct MaterialConstantBuffer
{
    float4 BaseColor;
    
    uint MaterialType;
    float Roughness;
    int TextureMapIndex; //使用的纹理贴图序号，不使用为-1
    int NormalMapIndex; //使用法线贴图序号序号， 不使用为-1
    
    int SpecularMapIndex; //使用的高光贴图序号, 不适用为-1
    float3 SpecularColor; //默认使用的高光颜色基数
    
    float4x4 TransformInformation;
};
StructuredBuffer<MaterialConstantBuffer> AMaterials : register(t0, space1); //所有材质数组
Texture2D SimpleTexture2DMap[MAPCOUNT] : register(t1); //所有贴图数组

TextureCube SampleTextureCubeMap : register(t0);

#endif