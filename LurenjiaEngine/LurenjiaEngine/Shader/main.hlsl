#include "Material.hlsl"
#include "Light.hlsl"


cbuffer ObjectConstBuffer : register(b0) //模型变换矩阵
{
    float4x4 WorldMatrix;
}
cbuffer ViewportConstBuffer : register(b1)//摄像机变换矩阵和透视投影变换矩阵
{
    float4 cameraPosition;
    float4x4 ViewProjectionMatrix;
}
cbuffer MaterialConstantBufferView : register(b2) //材质
{
    float4 BaseColor;
    uint MaterialType;
    float Roughness;
    float4x4 TransformInformation;
}
cbuffer LightConstantBufferView : register(b3) //灯光
{
    float3 LightIntensity;
    int XX;
    
    float3 LightDirection;
    int XX2;
}
struct MeshVertexIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
    float3 Normal : NORMAL;
    float3 Tangent : UTANGENT;
};
struct MeshVertexOut
{
    float4 Position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float3 Tangent : UTANGENT;
};
MeshVertexOut VertexShaderMain(MeshVertexIn mv)
{
    MeshVertexOut MV_out;
    float4x4 mvp = mul(ViewProjectionMatrix, WorldMatrix);
    MV_out.Position = mul(mvp, float4(mv.Position, 1.0f));//拿到mvp后的顶点坐标
    MV_out.worldPosition = mul(WorldMatrix, float4(mv.Position, 1.0f));
    MV_out.Normal = mul((float3x3) WorldMatrix, mv.Normal); //拿到世界空间下的顶点法向
    MV_out.Tangent = mul((float3x3) WorldMatrix, mv.Tangent); //拿到世界空间下的切线
    MV_out.Color = mv.Color;
    return MV_out;
}
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    float4 AmbientLight = { 0.15f, 0.15f, 0.25f, 1.0f };//环境光
    float3 L = normalize(-LightDirection);
    float3 N = normalize(mvOut.Normal);
    float3 V = normalize(cameraPosition.xyz - mvOut.worldPosition.xyz);
    float LdotN = dot(L, N); //顶点法向和光照方向点积
    float4 Ambient = { 0.f, 0.f, 0.f, 1.f};
    float4 diffuse = { 0.f, 0.f, 0.f, 1.f};
    float4 specular = { 0.f, 0.f, 0.f, 1.f};
    float4 Fresnel = { 0.f, 0.f, 0.f, 1.f };
    
    FMaterial Material;
    Material.BaseColor = BaseColor;
    if(MaterialType == 1)//兰伯特
    {
        Ambient = Material.BaseColor * AmbientLight;
        diffuse = Material.BaseColor * max(LdotN, 0.f);
    }
    else if(MaterialType == 2)//半兰伯特
    {
        Ambient = Material.BaseColor * AmbientLight;
        diffuse = Material.BaseColor * (LdotN * 0.5f + 0.5f);
    }
    else if(MaterialType == 3)//phone
    {
        Ambient = Material.BaseColor * AmbientLight;
        diffuse = Material.BaseColor * (LdotN * 0.5f + 0.5f);
        //float3 reflectDirection = 2.0f * LdotN * N - L;
        float3 reflectDirection = reflect(-L, N);
        float MaterialShininess = 1.f - saturate(Roughness);
        float M = max(MaterialShininess * 100.f, 1.0f);
        specular = Material.BaseColor * pow(max(dot(normalize(reflectDirection), V), 0.f), M);
    }
    else if(MaterialType == 4)//blinnPhone
    {
        Ambient = Material.BaseColor * AmbientLight;
        diffuse = Material.BaseColor * (LdotN * 0.5f + 0.5f);
        float3 H = L + V;//摄像机方向和光线入射方向的半程向量
        float MaterialShininess = 1.f - saturate(Roughness);
        float M = max(MaterialShininess * 100.f, 1.0f);
        specular = Material.BaseColor * pow(saturate(dot(N, normalize(H))), M);

    }
    else if(MaterialType == 5)//wrapLight,类似模拟皮肤
    {   
        float w = 3.f;//w为0 是兰伯特材质，w为1 是半兰伯特
        Ambient = Material.BaseColor * AmbientLight;
        diffuse = Material.BaseColor * saturate(((LdotN + w) / (1.f + w)));
    }
    else if(MaterialType == 6)//minnaertLight
    {
        Ambient = Material.BaseColor * AmbientLight;
        float NdotL = dot(N, L);
        float VdotN = dot(V, N);
        
        /*简单实现dot * dot*/
        //diffuse = Material.BaseColor * saturate(NdotL) * saturate(VdotN);
        
        /*有粗糙度的实现*/
        float MaterialShininess = saturate(Roughness);
        diffuse = Material.BaseColor * saturate(NdotL) * pow(saturate(NdotL) * saturate(VdotN), MaterialShininess);
    }
    else if(MaterialType == 7)//BandedLight
    {
        Ambient = Material.BaseColor * AmbientLight;
        float UpDotValue = (LdotN + 1.0f) * 0.5f;
        float layout = 4.0f;
        UpDotValue = floor(UpDotValue * layout) / layout;
        diffuse = Material.BaseColor * UpDotValue;

    }
    else if(MaterialType == 8)//渐变Banded
    {
        Ambient = Material.BaseColor * AmbientLight;
        float UpDotValue = (LdotN + 1.0f) * 0.5f;
        float layout = 7.0f;
        UpDotValue = floor(UpDotValue * layout) / layout;
        
        float4 color = { 245.f / 255.f, 88.f / 255.f, 0.f, 1.0f };
        Material.BaseColor = lerp(Material.BaseColor, color, 1 - LdotN); //Material.BaseColor * (1 - LdotN) + color * LdotN;
        
        diffuse = Material.BaseColor * UpDotValue;
    }
    else if(MaterialType == 9)//混合了多重效果的Banded
    {
        //环境光
        Ambient = Material.BaseColor * AmbientLight;
        
        //分层Banded
        float UpDotValue = (LdotN + 1.0f) * 0.5f;
        float layout = 4.0f;
        UpDotValue = floor(UpDotValue * layout) / layout;
        diffuse = Material.BaseColor * UpDotValue;
        
        //Fresnel
        float3 F0 = { 0.02f, 0.03f, 0.04f }; //反射率
        float M = 5.0f;
        float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
        Fresnel = Material.BaseColor * float4(FresnelSchlickMethod(F0, normalize(mvOut.Normal), normalize(cameraDirection), M), 1.0f);
        
        //phone
        float3 reflectDirection = reflect(normalize(LightDirection), normalize(mvOut.Normal));
        float MaterialShininess = 1.f - saturate(Roughness);
        M = max(MaterialShininess * 100.f, 1.0f);
        specular = Material.BaseColor * pow(max(dot(normalize(reflectDirection), normalize(cameraDirection)), 0.f), M) / 0.0314;
    }
    else if(MaterialType == 10)//sss
    {
        //环境光
        Ambient = Material.BaseColor * AmbientLight;
        
        //漫反射
        float w = 1.2f; //w为0 是兰伯特材质，w为1 是半兰伯特
        diffuse = Material.BaseColor * saturate(((LdotN + w) / (1.f + w)));
        
        //高光
        float3 reflectDirection = reflect(-L, N);
        float MaterialShininess = 1.f - saturate(Roughness);
        float M = max(MaterialShininess * 100.f, 1.0f);
        specular = Material.BaseColor * pow(max(dot(normalize(reflectDirection), V), 0.f), M);
        
        //次表面散射
        float sssValue = 1.3f;
        float TransmissionIntensity = 2.f;
        float TransmissionScope = 1.5f;
        float3 Half = -normalize(L + N * sssValue);
        LdotN = pow(saturate(dot(V, Half)), TransmissionScope) * TransmissionIntensity;
        diffuse = diffuse + Material.BaseColor * LdotN;
        
        
    }
    else if(MaterialType == 11)//各向异性
    {

        Ambient = Material.BaseColor * AmbientLight;
        
    }
    else if (MaterialType == 12)//OrenNayar Lighting
    {
        Ambient = Material.BaseColor * AmbientLight;
        
        LdotN = saturate(dot(N, L)); //光线与法线夹角
        float VdotN = saturate(dot(N, V));          //视线与法线夹角
        
        float phi_r = length(V - N * VdotN) * length(L - N * LdotN);

        float Alpha_temp = acos(VdotN);
        float Beta_temp = acos(LdotN);
        float Alpha = max(Alpha_temp, Beta_temp);   //在（L 和 N的夹角）和（V 和 N的夹角）中取大角
        float Beta = min(Alpha_temp, Beta_temp);    //在（L 和 N的夹角）和（V 和 N的夹角）中取小角
        
        float iRoughness = pow(Roughness, 2);
        float A = 1.f - 0.5f * (iRoughness / (iRoughness + 0.33f));
        float B = 0.45f * (iRoughness / (iRoughness + 0.09f));
        
        float diffuse_value = LdotN * (A + B * max(0, phi_r) * sin(Alpha) * tan(Beta));
        diffuse = Material.BaseColor * diffuse_value;
        
    }
    else if(MaterialType == 100)//菲涅尔
    {
        float3 F0 = { 0.2f, 0.3f, 0.4f };//反射率
        float M = 1.0f;
        float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
        Fresnel = Material.BaseColor * float4(FresnelSchlickMethod(F0, normalize(mvOut.Normal), normalize(cameraDirection), M), 1.0f);
    }
    else//默认
    {

    }
    mvOut.Color = Ambient + diffuse + specular + Fresnel;
    return mvOut.Color;
}
