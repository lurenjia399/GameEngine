#include "Common.hlsl"

struct MeshVertexIn
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};
struct MeshVertexOut
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TexCoord;
};

MeshVertexOut VertexShaderMain(MeshVertexIn mv)
{
    MeshVertexOut MV_out= (MeshVertexOut)0;
    
    MaterialConstantBuffer material = AMaterials[MaterialIndex]; // 获取当前物体材质
    
    float4x4 mvp = mul(ViewProjectionMatrix, WorldMatrix);
    MV_out.Position = mul(mvp, float4(mv.Position, 1.0f)); //经过mvp变换到齐次剪裁空间
    
    float4 worldTexTransformation = mul(ObjectTextureTransformation, float4(mv.TexCoord, 0.f, 1.f));
    MV_out.TexCoord = mul(AMaterials[MaterialIndex].TransformInformation, worldTexTransformation).xy;
    
    
    return MV_out;
}

float4 PixelShaderMain(MeshVertexOut mvOut)
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);

}