#include "Material.hlsl"
#include "Light.hlsl"


cbuffer ObjectConstBuffer : register(b0) //ģ�ͱ任����
{
    float4x4 WorldMatrix;
}
cbuffer ViewportConstBuffer : register(b1)//������任�����͸��ͶӰ�任����
{
    float4x4 ViewProjectionMatrix;
}
cbuffer MaterialConstantBufferView : register(b2) //����
{
    float4 BaseColor;
    float4x4 TransformInformation;
}
cbuffer LightConstantBufferView : register(b3) //�ƹ�
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
    MV_out.Position = mul(mvp, float4(mv.Position, 1.0f));//�õ�mvp��Ķ�������
    MV_out.Normal = mul((float3x3) WorldMatrix, mv.Normal); //�õ�����ռ��µĶ��㷨��
    MV_out.Color = mv.Color;
    return MV_out;
}
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    float4 AmbientLight = { 0.15f, 0.15f, 0.25f, 1.0f };//������
    float3 aa = { 1.0f, 0.f, 0.f };
    float DotValue = max(dot(normalize(mvOut.Normal), normalize(-LightDirection)), 0.f); //���㷨��͹��շ�����
    
    FMaterial Material;
    Material.BaseColor = BaseColor;
    mvOut.Color = clamp(Material.BaseColor * DotValue + Material.BaseColor * AmbientLight, 0.f, 1.0f);
    //mvOut.Color = Material.BaseColor * DotValue + Material.BaseColor * AmbientLight;
    return mvOut.Color;
}
