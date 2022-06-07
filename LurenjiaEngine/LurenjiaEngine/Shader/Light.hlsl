
struct Light
{
    float3  LightIntensity;         //��Դǿ��
    float StartAttenuation;         //���Դ��ʼ˥�����루����Դ����λ�õľ��룩
    
    float3  LightDirection;         //��Դ����
    float   EndAttenuation;         //���Դ�Ľ���˥��������Դ����λ�õľ��룩
    
    float3  Position;               //�ƹ�λ��
    int     LightType;              //�ƹ�����
};

float3 GetLightDirection(Light InLight, float3 InObjectLocation)
{
    if(InLight.LightType == 0)
    {
        return InLight.LightDirection;
    }else if(InLight.LightType == 1)
    {
        return InObjectLocation - InLight.Position;
    }
    return float3(0.f, 0.f, -1.f);
}

float4 CalculateLightStrength(Light InLight, float3 InObjectPointNormal, float3 InObjectLocation, float3 InLightDirection)
{
    
    if (InLight.LightType == 0)          //ƽ�й�
    {
        
    }
    else if (InLight.LightType == 1)    //���Դ
    {
        
        float4 LightStrength = float4(InLight.LightIntensity, 1.f);
        float ObjectLocation2LightLocation = length(InLight.Position - InObjectLocation);
        float lerpRange = 3.f;
        if (ObjectLocation2LightLocation < InLight.EndAttenuation)
        {
            float AttenuationRange = InLight.EndAttenuation - InLight.StartAttenuation;
            return LightStrength * (ObjectLocation2LightLocation / AttenuationRange);
        }

    }
    return float4(0.f, 0.f, 0.f, 1.f);

}