#ifndef COMMON
#define COMMON

#include "Light.hlsl"

SamplerState TextureSampler : register(s0); //��̬������
SamplerState AnisotropicSampler : register(s1); //�������Բ�����

cbuffer ObjectConstBuffer : register(b0) //ģ��CBV
{
    float4x4 WorldMatrix;
    float4x4 ObjectTextureTransformation;
    
    uint MaterialIndex;
    int xx1;
    int xx2;
    int xx3;

}
cbuffer ViewportConstBuffer : register(b1) //�ӿ�CBV
{
    float4 cameraPosition;
    float4x4 ViewProjectionMatrix;
}
cbuffer LightConstantBufferView : register(b2) //�ƹ�CBV
{
    Light SceneLight[16];
}
cbuffer FogConstantBufferView : register(b3) //��CBV
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
    int TextureMapIndex; //ʹ�õ�������ͼ��ţ���ʹ��Ϊ-1
    int NormalMapIndex; //ʹ�÷�����ͼ�����ţ� ��ʹ��Ϊ-1
    
    int SpecularMapIndex; //ʹ�õĸ߹���ͼ���, ������Ϊ-1
    float3 SpecularColor; //Ĭ��ʹ�õĸ߹���ɫ����
    
    float4x4 TransformInformation;
};
StructuredBuffer<MaterialConstantBuffer> AMaterials : register(t0, space1); //���в�������
Texture2D SimpleTexture2DMap[MAPCOUNT] : register(t1); //������ͼ����

TextureCube SampleTextureCubeMap : register(t0);

#endif