
float3x3 GetTBNMatrix(float3 Tangent, float3 Normal)
{
    
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal); //�����������ߺͷ��߲���ֱ������Ҫ
    float3 Bitangent = cross(Tangent, Normal);
    Bitangent = normalize(Bitangent);
    
    //return float3x3(Tangent.x, Bitangent.x, Normal.x,Tangent.y, Bitangent.y, Normal.y,Tangent.z, Bitangent.z, Normal.z);
    return float3x3(Tangent, Bitangent, Normal);
}