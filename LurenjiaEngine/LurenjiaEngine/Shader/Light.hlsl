
struct Light
{
    float3  LightIntensity;         //光源强度
    float StartAttenuation;         //点光源开始衰减距离（离点光源中心位置的距离）
    
    float3  LightDirection;         //光源方向
    float   EndAttenuation;         //点光源的结束衰减（离点光源中心位置的距离）
    
    float3  Position;               //灯光位置
    int     LightType;              //灯光类型
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
    
    if (InLight.LightType == 0)          //平行光
    {
        
    }
    else if (InLight.LightType == 1)    //点光源
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