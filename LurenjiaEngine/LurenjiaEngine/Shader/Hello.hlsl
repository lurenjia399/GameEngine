
cbuffer ConstBuffer : register(b0)//b0->b14
{
	float4x4 World;
}
struct MeshVertex
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};
struct MeshVertexOut
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};
MeshVertexOut VertexShader(MeshVertex mv)
{
    MeshVertexOut MV_out;
    MV_out.Position = mul(float4(mv.Position, 1.0f), World); //ÐÐÏòÁ¿ ×ó³Ë ¾ØÕó
    MV_out.Color = mv.Color;
    return MV_out;
}
float4 PixelShader(MeshVertexOut mvOut) : SV_Target
{
    return mvOut.Color;
}