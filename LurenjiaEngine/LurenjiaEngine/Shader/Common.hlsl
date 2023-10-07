#ifndef COMMON
#define COMMON

#include "Light.hlsl"

SamplerState TextureSampler : register(s0); //��̬������             rootsignature��StaticSampler 0����
SamplerState AnisotropicSampler : register(s1); //�������Բ�����       rootsignature��StaticSampler 1����
SamplerComparisonState ShadowSampler : register(s2); // ��Ӱ������

cbuffer ObjectConstBuffer : register(b0) //ģ��CBV    rootsignature��0����
{
    float4x4 WorldMatrix;
    float4x4 ObjectTextureTransformation;
    
    uint MaterialIndex;
    int xx1;
    int xx2;
    int xx3;

}
cbuffer ViewportConstBuffer : register(b1) //�ӿ�CBV  rootsignature��1����
{
    float4 cameraPosition;
    float4x4 ViewProjectionMatrix;
}
cbuffer LightConstantBufferView : register(b2) //�ƹ�CBV rootsignature��2����
{
    Light SceneLight[16];
}
cbuffer FogConstantBufferView : register(b3) //��CBV rootsignature��3����
{
    float3 FogColor;
    float FogStart;

    float FogRange;
    float FogHeight;
    float FogTransparentCoefficient;
    float xx5;
}

struct MaterialConstantBuffer  
{
    float4 BaseColor;
    
    uint MaterialType;
    float Roughness;
    int TextureMapIndex; //ʹ�õ�������ͼ��ţ���ʹ��Ϊ-1
    int NormalMapIndex; //ʹ�÷�����ͼ�����ţ� ��ʹ��Ϊ-1
    
    int SpecularMapIndex; //ʹ�õĸ߹���ͼ���, ������Ϊ-1
    float3 SpecularColor; //Ĭ��ʹ�õĸ߹���ɫ����
    
    float3 FresnelF0;
    float xx6;
    
    float4x4 TransformInformation;
};
StructuredBuffer<MaterialConstantBuffer> AMaterials : register(t0, space1); //���в������� //rootsignature��4����
Texture2D SimpleTexture2DMap[MAPCOUNT] : register(t2); //������ͼ����     //rootsignature��5����
Texture2D SimpleShadowMap : register(t1); //��Ӱ��ͼ     //rootsignature��5����

TextureCube SampleTextureCubeMap : register(t0); //rootsignature��6����

#endif