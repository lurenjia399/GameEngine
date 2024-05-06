#include "Common.hlsl"

struct MeshVertexIn
{
    float3 Position : POSITION;
};
struct MeshVertexOut
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION;
};

MeshVertexOut VertexShaderMain(MeshVertexIn mv)
{
    MeshVertexOut MV_out;
    
    // ��������
    MV_out.WorldPosition = mul(WorldMatrix, float4(mv.Position, 1.0f));
    
    // ndc�ռ�
    MV_out.Position = mul(ViewProjectionMatrix, MV_out.WorldPosition);
    
    return MV_out;
}

float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    MaterialConstantBuffer currMaterial = AMaterials[MaterialIndex]; //��ǰģ�͵Ĳ���
    
    return currMaterial.BaseColor;
}