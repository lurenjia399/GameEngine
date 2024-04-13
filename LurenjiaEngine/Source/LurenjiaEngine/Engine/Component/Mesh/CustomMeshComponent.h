#pragma once

#ifndef CustomMeshComponent_H
#define CustomMeshComponent_H

#include "StaticMeshComponent.h"

class CCustomMeshComponent : public CStaticMeshComponent
{
public:
	CCustomMeshComponent();

	void CreateMeshRenderData(FMeshRenderingData& InRenderingData, const string& InPath);
	void BuildKey(size_t& OutHashKey, const string& InPath);
private:
	bool LoadObjFileBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData& InRenderingData);

	bool LoadFBXFileBuffer(const std::string& InPath, FMeshRenderingData& InRenderingData);
};

#endif
