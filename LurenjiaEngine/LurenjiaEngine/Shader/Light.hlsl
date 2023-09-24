#ifndef LIGHT
#define LIGHT

struct Light
{
    float3 LightIntensity; //光源强度
    float StartAttenuation; //范围光源开始衰减距离（离光源中心位置的距离）
    
    float3 LightDirection; //光源方向
    float EndAttenuation; //范围光源的结束衰减（离光源中心位置的距离）
    
    float3 Position; //灯光位置
    int LightType; //灯光类型
    
    float ConicalInnerCorner; //聚光灯锥形内角，弧度制
    float ConicalOuterCorner; //聚光灯锥形外角，弧度制
    int XX; //占位
    int XX2; //占位
    
    float4x4 ViewProjectionMatrix; //灯光的视口投影矩阵，用来算shadowmap的
};

float3 GetLightDirection(Light InLight, float3 InObjectLocation)
{
    if (InLight.LightType == 0)
    {
        return InLight.LightDirection;
    }
    else if (InLight.LightType == 1)
    {
        return InObjectLocation - InLight.Position;
    }
    else if (InLight.LightType == 2)
    {
        return InObjectLocation - InLight.Position;
    }
    return float3(0.f, 0.f, -1.f);
}

float4 CalculateLightStrength(Light InLight, float3 InObjectPointNormal, float3 InObjectLocation, float3 InLightDirection)
{
    
    if (InLight.LightType == 0)          //平行光
    {
        return float4(InLight.LightIntensity, 1.f);
    }
    else if (InLight.LightType == 1)    //点光源
    {
        
        float4 LightStrength = float4(InLight.LightIntensity, 1.f);
        float ObjectLocation2LightLocation = length(InLight.Position - InObjectLocation);
        if (ObjectLocation2LightLocation <= InLight.EndAttenuation && ObjectLocation2LightLocation >= InLight.StartAttenuation)
        {
            float AttenuationRange = InLight.EndAttenuation - InLight.StartAttenuation;
            return LightStrength * ((AttenuationRange - (ObjectLocation2LightLocation - InLight.StartAttenuation)) / AttenuationRange);
        }

    }
    else if (InLight.LightType == 2)    //聚光灯
    {
        if (InLight.ConicalInnerCorner == InLight.ConicalOuterCorner)
            return float4(0.f, 0.f, 0.f, 1.f);
        float4 LightStrength = float4(InLight.LightIntensity, 1.f);
        float Object2LightAngle = acos(pow(dot(InLightDirection, -InLight.LightDirection), 1));
        if (Object2LightAngle <= InLight.ConicalInnerCorner)  //如果角度 < 内角
        {
            float ObjectLocation2LightLocation = length(InLight.Position - InObjectLocation);
            if (ObjectLocation2LightLocation <= InLight.EndAttenuation && ObjectLocation2LightLocation >= InLight.StartAttenuation)
            {
                float AttenuationRange = InLight.EndAttenuation - InLight.StartAttenuation;
                return LightStrength * ((AttenuationRange - (ObjectLocation2LightLocation - InLight.StartAttenuation)) / AttenuationRange);
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
                return LightStrength * StrengthRatio * ((AttenuationRange - (ObjectLocation2LightLocation - InLight.StartAttenuation)) / AttenuationRange);
            }
        }
    }
    return float4(0.f, 0.f, 0.f, 1.f);
}

#endif


