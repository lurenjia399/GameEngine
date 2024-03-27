#include "Common.hlsl"

float4 GetFogValue(float4 InColor, float3 InPointPosition)
{
    // ������������Ч��
#if START_UP_FOG
    
    float Distance = length(InPointPosition - cameraPosition.xyz);
    
    float AlphaValue = saturate((Distance - FogStart) / FogRange);
    
    
    // lerp(v1, v2, weight) -> v1 * (1 - weight) + v2 * weight
    float3 Color = lerp(InColor.rgb, FogColor, AlphaValue);//ͨ������ֵ��ֵ�������ɫ
    
    float HeightAlphaValue = saturate((InPointPosition.z - cameraPosition.z) / max(FogHeight, 1.0f));
    
    HeightAlphaValue -= FogTransparentCoefficient;
    
    InColor.rgb = lerp(Color.rgb, InColor.rgb, HeightAlphaValue);//ͨ���߶�ֵ��ֵ�������ɫ
    
#endif
    return InColor;
}
