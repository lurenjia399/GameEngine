
struct Light
{
    float3  LightIntensity;         //��Դǿ��
    float StartAttenuation;         //���Դ��ʼ˥�����루����Դ����λ�õľ��룩
    
    float3  LightDirection;         //��Դ����
    float   EndAttenuation;         //���Դ�Ľ���˥��������Դ����λ�õľ��룩
    
    float3  Position;               //�ƹ�λ��
    int     LightType;              //�ƹ�����
};