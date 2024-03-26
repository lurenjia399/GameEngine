cbuffer cbSettings
{
    // ������ɫ���ܷ��ʵĳ�������������
};

// ����Դ����ɫ�������
Texture2D gInputA;
Texture2D gInputB;
RWTexture2D<float4> gOutput;

// �߳����е��߳��������е��߳̿��Ա�����Ϊ1D��2D��3D�����񲼾�
[numthreads(16, 16, 1)]
void ComputeShder(int3 dispatchThreadId : SV_DispatchThreadID)
{
    // ������ԭ�����к�������ֱ�Ϊx,y����ƬԪ���
    // ����������浽��Ӧ��gOutPut��
    gOutput[dispatchThreadId.xy] = gInputA[dispatchThreadId.xy] + gInputB[dispatchThreadId.xy];

}