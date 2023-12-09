cbuffer cbSettings
{
    // 计算着色器能访问的常量缓冲区数据
};

// 数据源及着色器的输出
Texture2D gInputA;
Texture2D gInputB;
RWTexture2D<float4> gOutput;

// 线程组中的线程数。组中的线程可以被设置为1D、2D或3D的网格布局
[numthreads(16, 16, 1)]
void ComputeShder(int3 dispatchThreadId : SV_DispatchThreadID)
{
    // 对两种原像素中横纵坐标分别为x,y处的片元求和
    // 并将结果保存到相应的gOutPut中
    gOutput[dispatchThreadId.xy] = gInputA[dispatchThreadId.xy] + gInputB[dispatchThreadId.xy];

}