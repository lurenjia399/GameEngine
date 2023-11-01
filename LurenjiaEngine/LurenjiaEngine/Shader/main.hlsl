#include "Material.hlsl"
#include "SkyFunction.hlsl"
#include "ShadowFunction.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
    float3 Normal : NORMAL;
    float3 Tangent : UTANGENT;
    float2 TexCoord : TEXCOORD;
};
struct MeshVertexOut
{
    float4 Position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float3 Tangent : UTANGENT;
    float2 TexCoord : TEXCOORD;
};
MeshVertexOut VertexShaderMain(MeshVertexIn mv)
{
    MeshVertexOut MV_out;
    float4x4 mvp = mul(ViewProjectionMatrix, WorldMatrix);
    MV_out.Position = mul(mvp, float4(mv.Position, 1.0f));//�õ�mvp��Ķ�������
    MV_out.worldPosition = mul(WorldMatrix, float4(mv.Position, 1.0f));
    
    if (AMaterials[MaterialIndex].MaterialType == 97)
    {
        MV_out.Normal = mv.Normal;
    }
    else
    {
        MV_out.Normal = mul((float3x3) WorldMatrix, mv.Normal); //�õ�����ռ��µĶ��㷨������Ӧ�ó�����ת�þ���
    }
    
    MV_out.Tangent = mul((float3x3) WorldMatrix, mv.Tangent); //�õ�����ռ��µ�����
    MV_out.Color = mv.Color;
    
    float4 worldTexTransformation = mul(ObjectTextureTransformation, float4(mv.TexCoord, 0.f, 1.f));
    MV_out.TexCoord = mul(AMaterials[MaterialIndex].TransformInformation, worldTexTransformation).xy;
    
    return MV_out;
}
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    MaterialConstantBuffer currMaterial = AMaterials[MaterialIndex]; //��ǰģ�͵Ĳ���
    
    
    
    int MaterialType = currMaterial.MaterialType;
    if(MaterialType == 99)//Ĭ�ϣ�ʹ�ò��ʱ������ɫ
    {
        mvOut.Color = currMaterial.BaseColor;
        return mvOut.Color;
    }else if(MaterialType == 98)
    {
        return float4(mvOut.Normal, 1.0f);
    }else if(MaterialType == 97)
    {
        return float4(mvOut.Normal, 1.0f);
    }else if(MaterialType == 16)
    {   // ��Ӱ�����ͼ
        return float4(SimpleShadowMap.Sample(TextureSampler, mvOut.TexCoord).rrr, 1.0f);
    }
    
    
    
    float4 AmbientLight = { 0.15f, 0.15f, 0.25f, 1.0f };//������
   
    
    float3 N = GetNormal(currMaterial, mvOut.TexCoord, normalize(mvOut.Tangent), normalize(mvOut.Normal));
    float3 V = normalize(cameraPosition.xyz - mvOut.worldPosition.xyz);
    float4 Ambient = { 0.f, 0.f, 0.f, 1.f};
    float4 diffuse = { 0.f, 0.f, 0.f, 1.f};
    float4 specular = GetSpecular(currMaterial, mvOut.TexCoord);
    float4 Fresnel = { 0.f, 0.f, 0.f, 1.f};
    
    
    
    FMaterial Material;
    Material.BaseColor = GetFinalColor(currMaterial, mvOut.TexCoord);
    
    float DotValue = 0;
    float4 FinalColor = { 0.f, 0.f, 0.f, 1.f };
    
    float4 LightStrengths = { 0.f, 0.f, 0.f, 1.f };
    for (int i = 0; i < 16; i++)
    {
        //�����ǰ��ʼ����ǿ��Ϊ0
        if (length(SceneLight[i].LightIntensity.xyz) == 0.f) continue;
        
        if (length(SceneLight[i].LightIntensity.xyz) > 0.f)
        {
            float4 Specular = GetSpecular(currMaterial, mvOut.TexCoord);
            //���㵱ǰ���շ���
            float3 L = normalize(-GetLightDirection(SceneLight[i], mvOut.worldPosition.xyz));
            //��������ڵ�ǰƬԪλ�õĹ���ǿ��
            float4 LightStrength = CalculateLightStrength(SceneLight[i], N, mvOut.worldPosition.xyz, L);
        
        
            float LdotN = dot(L, N); //���㷨��͹��շ�����
        
        
            //������
            if (MaterialType == 1)
            {
                DotValue = pow(max(dot(L, N), 0.0), 2.f);
            }
            //��������
            else if (MaterialType == 2)
            {
                DotValue = max((dot(L, N) * 0.5f + 0.5f), 0.0); //[-1,1] => [0,1]
            }
            //phone
            else if (MaterialType == 3)
            {
            
                DotValue = pow(max(LdotN, 0.f), 2.f);
            
                //float3 reflectDirection = 2.0f * LdotN * N - L;
                float3 reflectDirection = reflect(-L, N);
                float MaterialShininess = 1.f - saturate(currMaterial.Roughness);
                float M = max(MaterialShininess * 100.f, 1.0f);
                specular *= saturate((M + 2.0f) * pow(max(dot(V, reflectDirection), 0.f), M) / 3.1415926);
            }
            //blinnPhone
            else if (MaterialType == 4)
            {
                DotValue = pow(max(dot(N, L), 0.0), 2.f);
            
                float3 H = normalize(L + V); //���������͹������䷽��İ������
                float MaterialShininess = 1.f - saturate(currMaterial.Roughness);
                float M = MaterialShininess * 100.f;
                specular *= saturate((M + 2.0f) * pow(max(dot(N, H), 0.f), M) / 3.1415926);

            }
            //wrapLight,����ģ��Ƥ��
            else if (MaterialType == 5)
            {
                float w = 3.f; //wΪ0 �������ز��ʣ�wΪ1 �ǰ�������
                DotValue = saturate(((LdotN + w) / (1.f + w)));
            }
            //minnaertLight,˿��
            else if (MaterialType == 6)
            {
                float NdotL = dot(N, L);
                float VdotN = dot(V, N);
        
                /*��ʵ��dot * dot*/
                //diffuse = Material.BaseColor * saturate(NdotL) * saturate(VdotN);
        
                /*�дֲڶȵ�ʵ��*/
                float MaterialShininess = saturate(currMaterial.Roughness);
                DotValue = pow(saturate(NdotL), 2.f) * pow(pow(saturate(NdotL), 2.f) * saturate(VdotN), MaterialShininess);
            }
            //BandedLight,��ͨ
            else if (MaterialType == 7)
            {
                if (i == 0)
                {
                    float UpDotValue = (LdotN + 1.0f) * 0.5f;
                    float layout = 4.0f;
                    DotValue = floor(UpDotValue * layout) / layout;
                }
            }
            //����Banded
            else if (MaterialType == 8)
            {
                if(i == 0 )
                {
                    float UpDotValue = (LdotN + 1.0f) * 0.5f;
                    float layout = 7.0f;
                    DotValue = floor(UpDotValue * layout) / layout;
                }
                
            
                //float4 color = { 245.f / 255.f, 88.f / 255.f, 0.f, 1.0f };
                //Material.FinalColor = lerp(Material.FinalColor, color, 1 - LdotN); //Material.BaseColor * (1 - LdotN) + color * LdotN;
            }
            else if (MaterialType == 9)//����˶���Ч����Banded
            {
                //������
                Ambient = AmbientLight;
        
                //�ֲ�Banded
                float UpDotValue = (LdotN + 1.0f) * 0.5f;
                float layout = 4.0f;
                LightStrengths = floor(UpDotValue * layout) / layout;
            
                //Fresnel
                float3 F0 = { 0.02f, 0.03f, 0.04f }; //������
                float M = 5.0f;
                float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
                Fresnel = float4(FresnelSchlickMethod(F0, N, V, M), 1.0f);
        
                //phone
                float3 reflectDirection = reflect(-L, N);
                float MaterialShininess = 1.f - saturate(currMaterial.Roughness);
                M = max(MaterialShininess * 100.f, 1.0f);
                specular = pow(max(dot(normalize(reflectDirection), V), 0.f), M) / 0.0314;
            
                continue;
            }
            else if (MaterialType == 10)//sss
            {
                //������
                Ambient = AmbientLight;
        
                //������
                float w = 1.8f; //wΪ0 �������ز��ʣ�wΪ1 �ǰ�������
                diffuse = saturate(((LdotN + w) / (1.f + w)));
        
                //�߹�
                float3 reflectDirection = reflect(-L, N);
                float MaterialShininess = 1.f - saturate(currMaterial.Roughness);
                float M = max(MaterialShininess * 100.f, 1.0f);
                specular = saturate(pow(max(dot(normalize(reflectDirection), V), 0.f), M));
        
                //�α���ɢ��
                float sssValue = 1.3f;
                float TransmissionIntensity = 2.f;
                float TransmissionScope = 1.5f;
                float3 Half = -normalize(L + N * sssValue);
                LdotN = pow(saturate(dot(V, Half)), TransmissionScope) * TransmissionIntensity;
                diffuse = diffuse + LdotN;
        
        
            }
            else if (MaterialType == 11)//��������
            {

                Ambient = AmbientLight;
        
            }
            else if (MaterialType == 12)//OrenNayar Lighting
            {
                Ambient = AmbientLight;
        
            
                LdotN = pow(saturate(dot(N, L)), 2.f); //�����뷨�߼н�
            
                float VdotN = saturate(dot(N, V)); //�����뷨�߼н�
        
                float phi_r = length(V - N * VdotN) * length(L - N * LdotN);

                float Alpha_temp = acos(VdotN);
                float Beta_temp = acos(LdotN);
                float Alpha = max(Alpha_temp, Beta_temp); //�ڣ�L �� N�ļнǣ��ͣ�V �� N�ļнǣ���ȡ���
                float Beta = min(Alpha_temp, Beta_temp); //�ڣ�L �� N�ļнǣ��ͣ�V �� N�ļнǣ���ȡС��
        
                float iRoughness = pow(currMaterial.Roughness, 2);
                float A = 1.f - 0.5f * (iRoughness / (iRoughness + 0.33f));
                float B = 0.45f * (iRoughness / (iRoughness + 0.09f));
        
                diffuse = LdotN * (A + B * max(0, phi_r) * sin(Alpha) * tan(Beta));
        
            }
            else if (MaterialType == 100)//������
            {
                float3 F0 = { 0.2f, 0.3f, 0.4f }; //������
                float M = 1.0f;
                float3 cameraDirection = cameraPosition.xyz - mvOut.worldPosition.xyz;
                Fresnel = float4(FresnelSchlickMethod(F0, normalize(mvOut.Normal), normalize(cameraDirection), M), 1.0f);
            }
            else //Ĭ��
            {

            }
        
            Specular = saturate(Specular);
            
            // ʹ��shadowmap�ķ���������Ӱ,�����������Ȱ���Ӱ�ص�
            float visiable = 1.0f;
            //float visiable = UseShadowMap(mvOut.worldPosition, SceneLight[i].ViewProjectionMatrix);
            //float visiable = pcf(mvOut.worldPosition, SceneLight[i].ViewProjectionMatrix);
        
            Material.BaseColor = visiable * saturate(Material.BaseColor);
            
            float4 Diffuse = Material.BaseColor;
            FinalColor += visiable * (saturate((Diffuse + Specular) * LightStrength * DotValue));
        }
    }
        
    //������ɫ����
    mvOut.Color = FinalColor + AmbientLight * Material.BaseColor; //��ӹ�
    mvOut.Color.a = Material.BaseColor.a;
    
    switch (currMaterial.MaterialType)
    {
        case 3:  //Phone
        case 4:  //BlinnPhone
        case 9:  //Back
        case 15: //Transparency
		{
			//���㷴��
            float3 ReflectColor = GetReflectionColor(currMaterial, N, V, 5, (float3x3) WorldMatrix);
            mvOut.Color.xyz += ReflectColor;
            break;
        }
    }
    
    
    
    // ������
    //mvOut.Color = GetFogValue(mvOut.Color, mvOut.worldPosition.xyz);
    return mvOut.Color;
}
