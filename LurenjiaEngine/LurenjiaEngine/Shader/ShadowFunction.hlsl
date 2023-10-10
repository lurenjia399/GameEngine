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

float pcf(float4 InWorldPosition, float4x4 InShadowMatrix)
{
    // 深度图的大小，这边就直接给了值，应该是从dynamicshadowmap里面传进来
    float width = 2048.f;
    
    float4 position = mul(InShadowMatrix, InWorldPosition);
    position = float4(position.x / position.w, position.y / position.w, position.z / position.w, 1.0f);
    float position_x = position.x * 0.5f + 0.5f;
    float position_y = position.y * -0.5f + 0.5f;
    float position_z = position.z;
    position = float4(position_x, position_y, position_z, position.w);
    
    float2 temp[4] =
    {
        float2(0.f, 0.f), float2(1.f / width, 0.f), 
        float2(0.f, 1.f / width), float2(1.0f / width, 1.0f / width)
    };
    int sum = 0;
    for (int i = 0; i < 4; i++)
    {
        float textureDepth = SimpleShadowMap.Sample(TextureSampler, position.xy + temp[i]).r;
        if (textureDepth < position.z)
        {
            // 在阴影中
            sum = sum + 1.0f;
        }
    }
    
    return 1 - sum / 4.0f;
}

#endif