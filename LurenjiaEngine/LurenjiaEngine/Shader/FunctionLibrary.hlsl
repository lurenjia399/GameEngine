
float3x3 GetTBNMatrix(float3 Tangent, float3 Normal)
{
    
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal); //矫正，若切线和法线不垂直，则需要
    float3 Bitangent = cross(Tangent, Normal);
    Bitangent = normalize(Bitangent);
    
    //return float3x3(Tangent.x, Bitangent.x, Normal.x,Tangent.y, Bitangent.y, Normal.y,Tangent.z, Bitangent.z, Normal.z);
    return float3x3(Tangent, Bitangent, Normal);
}