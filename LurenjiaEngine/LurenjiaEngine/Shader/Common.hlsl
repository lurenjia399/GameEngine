#include "Light.hlsl"

SamplerState SimpleTextureState : register(s0); //静态采用器
Texture2D SimpleTexture2DMap[MapCount] : register(t3); //所有贴图数组

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
struct MaterialConstantBuffer
{
    float4 BaseColor;
    
    uint MaterialType;
    float Roughness;
    int TextureMapIndex; //使用的纹理贴图序号，不使用为-1
    int NormalMapIndex; //使用法线贴图序号序号， 不使用为-1
    
    float4x4 TransformInformation;
};
StructuredBuffer<MaterialConstantBuffer> AMaterials : register(t0, space1); //所有材质数组