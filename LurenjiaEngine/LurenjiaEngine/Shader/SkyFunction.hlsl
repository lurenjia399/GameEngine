#include "Common.hlsl"

float4 GetFogValue(float4 InColor, float3 InPointPosition)
{
    // 如果开启了雾的效果
#if START_UP_FOG
    
    float Distance = length(InPointPosition - cameraPosition.xyz);
    
    float AlphaValue = saturate((Distance - FogStart) / FogRange);
    
    
    // lerp(v1, v2, weight) -> v1 * (1 - weight) + v2 * weight
    float3 Color = lerp(InColor.rgb, FogColor, AlphaValue);//通过距离值插值出雾的颜色
    
    float HeightAlphaValue = saturate((InPointPosition.z - cameraPosition.z) / max(FogHeight, 1.0f));
    
    HeightAlphaValue -= FogTransparentCoefficient;
    
    InColor.rgb = lerp(Color.rgb, InColor.rgb, HeightAlphaValue);//通过高度值插值出雾的颜色
    
#endif
    return InColor;
}
