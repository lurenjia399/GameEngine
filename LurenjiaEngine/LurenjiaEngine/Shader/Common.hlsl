#include "Light.hlsl"

SamplerState SimpleTextureState : register(s0); //��̬������
Texture2D SimpleTexture2DMap[MapCount] : register(t3); //������ͼ����

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
struct MaterialConstantBuffer
{
    float4 BaseColor;
    
    uint MaterialType;
    float Roughness;
    int TextureMapIndex; //ʹ�õ�������ͼ��ţ���ʹ��Ϊ-1
    int NormalMapIndex; //ʹ�÷�����ͼ�����ţ� ��ʹ��Ϊ-1
    
    float4x4 TransformInformation;
};
StructuredBuffer<MaterialConstantBuffer> AMaterials : register(t0, space1); //���в�������