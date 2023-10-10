#ifndef ShadowFunction
#define ShadowFunction

#include "Common.hlsl"

float UseShadowMap(float4 InWorldPosition, float4x4 InShadowMatrix)
{
    // ÿ�����ص� * �ƹ��vp���� = ��μ��ÿռ��µ�����
    float4 position = mul(InShadowMatrix, InWorldPosition);
    // ��������˵��͸�ӳ�����Ȼ�����ndc�ռ䣬������ͶӰ����Ҫ͸�ӳ���
    position = float4(position.x / position.w, position.y / position.w, position.z / position.w, 1.0f);
    // ndc�ռ�����[-1�� 1]�������壬��������������[0, 1]�����Ի���Ҫת��
    // dx��uv�ռ�����Ļ�����Ͻǣ�����y����Ҫ�ڳ��ϸ���
    // ע�������gams202��ҵ�Ĳ�ͬ����ndc�ռ���opengl��z�᷶Χ��[-1,1]��dx��z�᷶Χ��[0,1]
    float position_x = position.x * 0.5f + 0.5f;
    float position_y = position.y * -0.5f + 0.5f;
    float position_z = position.z;
    position = float4(position_x, position_y, position_z, position.w);
    
    
    
    // �������ıȽ�״̬
    // ������uv����
    // �Ƚϵ����
    // ����һ���ȽϷ���������ͼ�е�ֵȡ�����͵������������бȽ�
    return SimpleShadowMap.SampleCmpLevelZero(ShadowSampler, position.xy, position.z);

}

float pcf(float4 InWorldPosition, float4x4 InShadowMatrix)
{
    // ���ͼ�Ĵ�С����߾�ֱ�Ӹ���ֵ��Ӧ���Ǵ�dynamicshadowmap���洫����
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
            // ����Ӱ��
            sum = sum + 1.0f;
        }
    }
    
    return 1 - sum / 4.0f;
}

#endif