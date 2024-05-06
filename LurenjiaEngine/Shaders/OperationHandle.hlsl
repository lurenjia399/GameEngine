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
    
    // 世界坐标
    MV_out.WorldPosition = mul(WorldMatrix, float4(mv.Position, 1.0f));
    
    // ndc空间
    MV_out.Position = mul(ViewProjectionMatrix, MV_out.WorldPosition);
    
    return MV_out;
}

float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    MaterialConstantBuffer currMaterial = AMaterials[MaterialIndex]; //当前模型的材质
    
    return currMaterial.BaseColor;
}