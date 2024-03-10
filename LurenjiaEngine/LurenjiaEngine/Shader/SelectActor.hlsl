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
    
    MV_out.WorldPosition = mul(WorldMatrix, float4(mv.Position, 1.0f));
    float3 ViewDirection = normalize(cameraPosition.xyz - MV_out.WorldPosition.xyz);
    MV_out.WorldPosition.xyz += ViewDirection;
    
    
    
    float4x4 mvp = mul(ViewProjectionMatrix, WorldMatrix);
    //MV_out.Position = mul(mvp, float4(mv.Position, 1.0f)); //经过mvp变换到齐次剪裁空间
    MV_out.Position = mul(ViewProjectionMatrix, MV_out.WorldPosition);
    
    //法线
    //Out.Normal = mul(mv.Normal, (float3x3) WorldMatrix);
    
    return MV_out;
}

float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{

    return float4(1.f, 0.f, 0.f, 1.f);
}