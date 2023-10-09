#ifndef ShadowFunction
#define ShadowFunction

#include "Common.hlsl"

float UseShadowMap(float4 InWorldPosition, float4x4 InShadowMatrix)
{
    // 每个像素点 * 灯光的vp矩阵 = 齐次剪裁空间下的坐标
    float4 position = mul(InShadowMatrix, InWorldPosition);
    // 流程上来说是透视除法，然后进入ndc空间，但正交投影不需要透视除法
    position = float4(position.x / position.w, position.y / position.w, position.z / position.w, 1.0f);
    // ndc空间下是[-1， 1]的立方体，而采样点坐标是[0, 1]，所以还需要转化
    // dx的uv空间是屏幕的左上角，所以y轴需要在乘上负号
    // 注意这里和gams202作业的不同，在ndc空间下opengl的z轴范围是[-1,1]而dx的z轴范围是[0,1]
    float position_x = position.x * 0.5f + 0.5f;
    float position_y = position.y * -0.5f + 0.5f;
    float position_z = position.z;
    position = float4(position_x, position_y, position_z, position.w);
    
    
    
    // 采样器的比较状态
    // 采样的uv坐标
    // 比较的深度
    // 就是一个比较方法，将贴图中的值取出来和第三个参数进行比较
    return SimpleShadowMap.SampleCmpLevelZero(ShadowSampler, position.xy, position.z);

}

#endif