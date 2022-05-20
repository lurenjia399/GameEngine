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
    float DotValue = dot(normalize(-LightDirection), normalize(mvOut.Normal)); //���㷨��͹��շ�����
    float4 Ambient = { 0.f, 0.f, 0.f, 1.f};
    float4 diffuse = { 0.f, 0.f, 0.f, 1.f};
    float4 specular = { 0.f, 0.f, 0.f, 1.f};
    float4 Fresnel = { 0.f, 0.f, 0.f, 1.f };
    
    FMaterial Material;
    Material.BaseColor = BaseColor;
    if(MaterialType == 1)//������
    {
        Ambient = Material.BaseColor * AmbientLight;
        diffuse = Material.BaseColor * max(DotValue, 0.f);
    }else if(MaterialType == 2)//��������
    {
        Ambient = Material.BaseColor * AmbientLight;
        diffuse = Material.BaseColor * (DotValue * 0.5f + 0.5f);
    }else if(MaterialType == 3)//phone
    {
        Ambient = Material.BaseColor * AmbientLight;
        diffuse = Material.BaseColor * (DotValue * 0.5f + 0.5f);
        //float3 reflectDirection = 2.0f * DotValue * normalize(mvOut.Normal) - normalize(-LightDirection);
        float3 reflectDirection = reflect(normalize(LightDirection), normalize(mvOut.Normal));
        float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
        float MaterialShininess = 1.f - saturate(Roughness);
        float M = max(MaterialShininess * 100.f, 1.0f);
        specular = Material.BaseColor * pow(max(dot(normalize(reflectDirection), normalize(cameraDirection)), 0.f), M);
    }else if(MaterialType == 4)//blinnPhone
    {
        Ambient = Material.BaseColor * AmbientLight;
        diffuse = Material.BaseColor * (DotValue * 0.5f + 0.5f);
        float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
        float3 H = normalize(-LightDirection) + normalize(cameraDirection);//���������͹������䷽��İ������
        float MaterialShininess = 1.f - saturate(Roughness);
        float M = max(MaterialShininess * 100.f, 1.0f);
        specular = Material.BaseColor * pow(saturate(dot(normalize(mvOut.Normal), normalize(H))), M);

    }else if(MaterialType == 5)//wrapLight,����ģ��Ƥ��
    {   
        float w = 3.f;//wΪ0 �������ز��ʣ�wΪ1 �ǰ�������
        Ambient = Material.BaseColor * AmbientLight;
        diffuse = Material.BaseColor * saturate(((DotValue + w) / (1.f + w)));
    }else if(MaterialType == 6)//minnaertLight
    {
        Ambient = Material.BaseColor * AmbientLight;
        float dotNL = dot(normalize(mvOut.Normal), normalize(-LightDirection));
        float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
        float dotVN = dot(normalize(cameraDirection), normalize(mvOut.Normal));
        
        /*��ʵ��dot * dot*/
        //float minnaert = Material.BaseColor * saturate(dotNL) * saturate(dotVN);
        
        /*�дֲڶȵ�ʵ��*/
        float MaterialShininess = saturate(Roughness);
        diffuse = Material.BaseColor * saturate(dotNL) * pow(saturate(dotNL) * saturate(dotVN), MaterialShininess);
    }else if(MaterialType == 7)//BandedLight
    {
        Ambient = Material.BaseColor * AmbientLight;
        float UpDotValue = (DotValue + 1.0f) * 0.5f;
        float layout = 4.0f;
        UpDotValue = floor(UpDotValue * layout) / layout;
        diffuse = Material.BaseColor * UpDotValue;

    }else if(MaterialType == 8)//����Banded
    {
        Ambient = Material.BaseColor * AmbientLight;
        float UpDotValue = (DotValue + 1.0f) * 0.5f;
        float layout = 7.0f;
        UpDotValue = floor(UpDotValue * layout) / layout;
        
        float4 color = { 245.f / 255.f, 88.f / 255.f, 0.f, 1.0f };
        Material.BaseColor = lerp(Material.BaseColor, color, 1 - DotValue); //Material.BaseColor * (1 - DotValue) + color * DotValue;
        
        diffuse = Material.BaseColor * UpDotValue;
    }else if(MaterialType == 9)//����˶���Ч����Banded
    {
        //������
        Ambient = Material.BaseColor * AmbientLight;
        
        //�ֲ�Banded
        float UpDotValue = (DotValue + 1.0f) * 0.5f;
        float layout = 4.0f;
        UpDotValue = floor(UpDotValue * layout) / layout;
        diffuse = Material.BaseColor * UpDotValue;
        
        //Fresnel
        float3 F0 = { 0.02f, 0.03f, 0.04f }; //������
        float M = 5.0f;
        float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
        Fresnel = Material.BaseColor * float4(FresnelSchlickMethod(F0, normalize(mvOut.Normal), normalize(cameraDirection), M), 1.0f);
        
        //phone
        float3 reflectDirection = reflect(normalize(LightDirection), normalize(mvOut.Normal));
        float MaterialShininess = 1.f - saturate(Roughness);
        M = max(MaterialShininess * 100.f, 1.0f);
        specular = Material.BaseColor * pow(max(dot(normalize(reflectDirection), normalize(cameraDirection)), 0.f), M) / 0.0314;
    }else if(MaterialType == 10)//sss
    {
        //������
        Ambient = Material.BaseColor * AmbientLight;
        
        //������
        float w = 1.2f; //wΪ0 �������ز��ʣ�wΪ1 �ǰ�������
        diffuse = Material.BaseColor * saturate(((DotValue + w) / (1.f + w)));
        
        //�߹�
        float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
        float3 reflectDirection = reflect(normalize(LightDirection), normalize(mvOut.Normal));
        float MaterialShininess = 1.f - saturate(Roughness);
        float M = max(MaterialShininess * 100.f, 1.0f);
        specular = Material.BaseColor * pow(max(dot(normalize(reflectDirection), normalize(cameraDirection)), 0.f), M);
        
        //�α���ɢ��
        float sssValue = 1.3f;
        float TransmissionIntensity = 2.f;
        float TransmissionScope = 1.5f;
        float3 Half = -normalize(normalize(-LightDirection) + normalize(mvOut.Normal) * sssValue);
        float angle = dot(normalize(cameraDirection.xyz), Half);
        DotValue = pow(saturate(dot(normalize(cameraDirection.xyz), Half)), TransmissionScope) * TransmissionIntensity;
        diffuse = diffuse + Material.BaseColor * DotValue;
        
        
    }
    else if(MaterialType == 100)//������
    {
        float3 F0 = { 0.2f, 0.3f, 0.4f };//������
        float M = 1.0f;
        float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
        Fresnel = Material.BaseColor * float4(FresnelSchlickMethod(F0, normalize(mvOut.Normal), normalize(cameraDirection), M), 1.0f);
    }
    else//Ĭ��
    {

    }
    mvOut.Color = Ambient + diffuse + specular + Fresnel;
    return mvOut.Color;
}
