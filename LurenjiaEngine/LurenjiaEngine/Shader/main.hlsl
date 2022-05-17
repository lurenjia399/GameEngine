#include "Material.hlsl"
#include "Light.hlsl"


cbuffer ObjectConstBuffer : register(b0) //模型变换矩阵
{
    float4x4 WorldMatrix;
}
cbuffer ViewportConstBuffer : register(b1)//摄像机变换矩阵和透视投影变换矩阵
{
    float4x4 ViewProjectionMatrix;
}
cbuffer MaterialConstantBufferView : register(b2) //材质
{
    float4 BaseColor;
    float4x4 TransformInformation;
}
cbuffer LightConstantBufferView : register(b3) //灯光
{
    float3 LightIntensity;
    float3 LightDirection;
}
struct MeshVertexIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
    float3 Normal : NORMAL;
};
struct MeshVertexOut
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
};
MeshVertexOut VertexShaderMain(MeshVertexIn mv)
{
    MeshVertexOut MV_out;
    float4x4 mvp = mul(ViewProjectionMatrix, WorldMatrix);
    MV_out.Position = mul(mvp, float4(mv.Position, 1.0f));//拿到mvp后的顶点坐标
    MV_out.Normal = mul((float3x3) WorldMatrix, mv.Normal); //拿到世界空间下的顶点法向
    MV_out.Color = mv.Color;
    return MV_out;
}
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    float4 AmbientLight = { 0.15f, 0.15f, 0.25f, 1.0f };//环境光
    float3 aa = { 1.0f, 0.f, 0.f };
    float DotValue = max(dot(normalize(mvOut.Normal), normalize(-LightDirection)), 0.f); //顶点法向和光照方向点积
    
    FMaterial Material;
    Material.BaseColor = BaseColor;
    mvOut.Color = clamp(Material.BaseColor * DotValue + Material.BaseColor * AmbientLight, 0.f, 1.0f);
    //mvOut.Color = Material.BaseColor * DotValue + Material.BaseColor * AmbientLight;
    return mvOut.Color;
}
