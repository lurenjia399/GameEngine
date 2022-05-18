#include "Material.hlsl"
#include "Light.hlsl"


cbuffer ObjectConstBuffer : register(b0) //ģ�ͱ任����
{
    float4x4 WorldMatrix;
}
cbuffer ViewportConstBuffer : register(b1)//������任�����͸��ͶӰ�任����
{
    float4 cameraPosition;
    float4x4 ViewProjectionMatrix;
}
cbuffer MaterialConstantBufferView : register(b2) //����
{
    float4 BaseColor;
    uint MaterialType;
    float Roughness;
    float4x4 TransformInformation;
}
cbuffer LightConstantBufferView : register(b3) //�ƹ�
{
    //float3 LightIntensity;
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
    float4 worldPosition : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
};
MeshVertexOut VertexShaderMain(MeshVertexIn mv)
{
    MeshVertexOut MV_out;
    float4x4 mvp = mul(ViewProjectionMatrix, WorldMatrix);
    MV_out.Position = mul(mvp, float4(mv.Position, 1.0f));//�õ�mvp��Ķ�������
    MV_out.worldPosition = mul(WorldMatrix, float4(mv.Position, 1.0f));
    MV_out.Normal = mul((float3x3) WorldMatrix, mv.Normal); //�õ�����ռ��µĶ��㷨��
    MV_out.Color = mv.Color;
    return MV_out;
}
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    float4 AmbientLight = { 0.15f, 0.15f, 0.25f, 1.0f };//������
    float3 aa = { 0.0f, -1.f, 0.f };
    float DotValue = dot(normalize(-LightDirection), normalize(mvOut.Normal)); //���㷨��͹��շ�����
    if(MaterialType == 1)//������
    {
        float4 Ambient = BaseColor * AmbientLight;
        float4 diffuse = BaseColor * max(DotValue, 0.f);
        mvOut.Color = Ambient + diffuse;
    }else if(MaterialType == 2)//��������
    {
        float4 Ambient = BaseColor * AmbientLight;
        float4 Halfdiffuse = BaseColor * (DotValue * 0.5f + 0.5f);
        mvOut.Color = Ambient + Halfdiffuse;
    }else if(MaterialType == 3)//phone
    {
        float4 Ambient = BaseColor * AmbientLight;
        float4 Halfdiffuse = BaseColor * (DotValue * 0.5f + 0.5f);
        //float3 reflectDirection = 2.0f * DotValue * normalize(mvOut.Normal) - normalize(-LightDirection);
        float3 reflectDirection = reflect(normalize(LightDirection), normalize(mvOut.Normal));
        float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
        float MaterialShininess = 1.f - saturate(Roughness);
        float M = max(MaterialShininess * 256.f, 1.0f);
        float4 specular = BaseColor * pow(max(dot(normalize(reflectDirection), normalize(cameraDirection)), 0.f), M);
        
        mvOut.Color = Ambient + Halfdiffuse + specular;
    }
    else//Ĭ��
    {

    }
    
    return mvOut.Color;
}
