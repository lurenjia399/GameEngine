#ifndef LIGHT
#define LIGHT

struct Light
{
    float3 LightIntensity; //��Դǿ��
    float StartAttenuation; //��Χ��Դ��ʼ˥�����루���Դ����λ�õľ��룩
    
    float3 LightDirection; //��Դ����
    float EndAttenuation; //��Χ��Դ�Ľ���˥�������Դ����λ�õľ��룩
    
    float3 Position; //�ƹ�λ��
    int LightType; //�ƹ�����
    
    float ConicalInnerCorner; //�۹��׶���ڽǣ�������
    float ConicalOuterCorner; //�۹��׶����ǣ�������
    int XX; //ռλ
    int XX2; //ռλ
    
    float4x4 ViewProjectionMatrix; //�ƹ���ӿ�ͶӰ����������shadowmap��
};

float3 GetLightDirection(Light InLight, float3 InObjectLocation)
{
    float3 result = float3(0.f, 0.f, -1.f);
    if (InLight.LightType == 0)
    {
        result = InLight.LightDirection;
    }
    else if (InLight.LightType == 1)
    {
        result = InObjectLocation - InLight.Position;
    }
    else if (InLight.LightType == 2)
    {
        result = InObjectLocation - InLight.Position;
    }
    
    return result;
}

float4 CalculateLightStrength(Light InLight, float3 InObjectPointNormal, float3 InObjectLocation, float3 InLightDirection)
{
    float4 result = float4(0.f, 0.f, 0.f, 1.f);
    
    if (InLight.LightType == 0)          //ƽ�й�
    {
        result = float4(InLight.LightIntensity, 1.f);
    }
    else if (InLight.LightType == 1)    //���Դ
    {
        
        float4 LightStrength = float4(InLight.LightIntensity, 1.f);
        float ObjectLocation2LightLocation = length(InLight.Position - InObjectLocation);
        if (ObjectLocation2LightLocation <= InLight.EndAttenuation && ObjectLocation2LightLocation >= InLight.StartAttenuation)
        {
            float AttenuationRange = InLight.EndAttenuation - InLight.StartAttenuation;
            result = LightStrength * ((AttenuationRange - (ObjectLocation2LightLocation - InLight.StartAttenuation)) / AttenuationRange);
        }

    }
    else if (InLight.LightType == 2)    //�۹��
    {
        if (InLight.ConicalInnerCorner != InLight.ConicalOuterCorner)
        {
            float4 LightStrength = float4(InLight.LightIntensity, 1.f);
            float Object2LightAngle = acos(pow(dot(InLightDirection, -InLight.LightDirection), 1));
            if (Object2LightAngle <= InLight.ConicalInnerCorner)  //����Ƕ� < �ڽ�
            {
                float ObjectLocation2LightLocation = length(InLight.Position - InObjectLocation);
                if (ObjectLocation2LightLocation <= InLight.EndAttenuation && ObjectLocation2LightLocation >= InLight.StartAttenuation)
                {
                    float AttenuationRange = InLight.EndAttenuation - InLight.StartAttenuation;
                    result = LightStrength * ((AttenuationRange - (ObjectLocation2LightLocation - InLight.StartAttenuation)) / AttenuationRange);
                }
            }
            else if (Object2LightAngle < InLight.ConicalOuterCorner && Object2LightAngle > InLight.ConicalInnerCorner)
            {
                float InnerOuterDistance = InLight.ConicalOuterCorner - InLight.ConicalInnerCorner;
                float CurrDistance = InLight.ConicalOuterCorner - Object2LightAngle;
                float StrengthRatio = CurrDistance / InnerOuterDistance;
            
                float ObjectLocation2LightLocation = length(InLight.Position - InObjectLocation);
                if (ObjectLocation2LightLocation <= InLight.EndAttenuation && ObjectLocation2LightLocation >= InLight.StartAttenuation)
                {
                    float AttenuationRange = InLight.EndAttenuation - InLight.StartAttenuation;
                    result = LightStrength * StrengthRatio * ((AttenuationRange - (ObjectLocation2LightLocation - InLight.StartAttenuation)) / AttenuationRange);
                }
            }
        }
    }
    
    return result;
}

#endif


