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
    float position_x = position.x * 0.5f + 0.5f;
    float position_y = position.y * 0.5f + 0.5f;
    float position_z = position.z * -0.5f +  0.5f;
    position = float4(position_x, position_y, position_z, position.w);
    
    // �������ıȽ�״̬
    // ������uv����
    // �Ƚϵ����
    // ����һ���ȽϷ���������ͼ�е�ֵȡ�����͵������������бȽ�
    return SimpleShadowMap.SampleCmpLevelZero(ShadowSampler, position.yz, position.x);
    
    //float res = 0.0f;
    
    //if (SimpleShadowMap.Sample(TextureSampler, position.xz).r > position.y)
    //    res = 1.0f;
    //else
    //    res = 0.0f;
    
   // res = position.y;
    
    //return res;

}

#endif