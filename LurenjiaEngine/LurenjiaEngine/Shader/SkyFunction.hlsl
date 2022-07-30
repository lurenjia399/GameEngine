#include "Common.hlsl"

float4 GetFogValue(float4 InColor, float3 InPointPosition)
{
    // 如果开启了雾的效果
#if START_UP_FOG
    
    float Distance = length(InPointPosition - cameraPosition.xyz);
    
    float AlphaValue = saturate((Distance - FogStart) / FogRange);
    
    InColor.rgb = lerp(InColor.rgb, FogColor, AlphaValue);
#endif
    return InColor;
}
