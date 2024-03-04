#include "RenderingLayer.h"
#include "../RenderLayerManage.h"
#include "../../../../../../Mesh/Core/ObjectTransformation.h"

FRenderingLayer::FRenderingLayer()
	: RenderingLayerPriority(0)
	, GeometryMap(nullptr)
	, DirectXPiepelineState(nullptr)
{
}

void FRenderingLayer::Init(FGeometryMap* InGeometryMap, FDirectXPiepelineState* InDirectXPiepelineState)
{
	GeometryMap = InGeometryMap;
	DirectXPiepelineState = InDirectXPiepelineState;
}

void FRenderingLayer::RegisterRenderLayer()
{
	FRenderLayerManage::RenderingLayers.emplace_back(shared_from_this());
}

UINT FRenderingLayer::GetPriority()
{
	return RenderingLayerPriority;
}

std::vector<std::weak_ptr<FGeometryDescData>>* FRenderingLayer::GetGeometryDescData()
{
	return &GeometryDescDatas;
}

void FRenderingLayer::RestorePSO()
{
	//恢复pso状态
	DirectXPiepelineState->isTemporaryResetPSO(-1, false);
}

void FRenderingLayer::BuildShaderMacro(std::vector<ShaderType::FShaderMacro>& OutShaderMacro)
{
	{
		char buffer[10] = { 0 };
		ShaderType::FShaderMacro ShaderMacro;
		ShaderMacro.Name = "MAPCOUNT";
		ShaderMacro.Definition = _itoa(GeometryMap->GetDrawTextureObjectCount(), buffer, 10);
		OutShaderMacro.emplace_back(ShaderMacro);
	}
	{
		ShaderType::FShaderMacro ShaderMacro;
		ShaderMacro.Name = "START_UP_FOG";
		ShaderMacro.Definition = GeometryMap->IsStartUpFog() ? "1" : "0";
		OutShaderMacro.emplace_back(ShaderMacro);
	}
}

void FRenderingLayer::UpdateObjectConstantBuffer()
{
	for (auto& data_weak : GeometryDescDatas)
	{
		if (data_weak.expired())
		{
			Engine_Log_Error("FRenderingLayer::UpdateObjectConstantBuffer data_weak is nullptr");
			continue;
		}

		std::shared_ptr<FGeometryDescData> data = data_weak.lock();

		const XMFLOAT3& Position = data->MeshComponet->GetPosition();
		const XMFLOAT3& Scale = data->MeshComponet->GetScale();
		const XMFLOAT3& RightVector = data->MeshComponet->GetRight();
		const XMFLOAT3& UpVector = data->MeshComponet->GetUp();
		const XMFLOAT3& ForwardVector = data->MeshComponet->GetForward();

		XMMATRIX scaleMatrix =
		{
			Scale.y,	0.0f,		0.0f,		0.0f,
			0.0f,		Scale.z,	0.0f,		0.0f,
			0.0f,		0.0f,		Scale.x,	0.0f,
			0.0f,		0.0f,		0.0f,		1.0f
		};

		XMMATRIX rotateMatrix =
		{
			RightVector.x,	UpVector.x, ForwardVector.x,	0.f,
			RightVector.y,	UpVector.y, ForwardVector.y,	0.f,
			RightVector.z,	UpVector.z, ForwardVector.z,	0.f,
			0,				0,			0,					1.f
		};
		XMMATRIX TranslateMatrix =
		{
			1.f, 0.f, 0.f, Position.x,
			0.f, 1.f, 0.f, Position.y,
			0.f, 0.f, 1.f, Position.z,
			0.f, 0.f, 0.f, 1.f
		};
		XMStoreFloat4x4(&data->WorldMatrix, XMMatrixTranspose(TranslateMatrix * rotateMatrix * scaleMatrix));
		XMMATRIX MatrixWorld = XMLoadFloat4x4(&data->WorldMatrix);
		XMMATRIX MatrixTextureTransform = XMLoadFloat4x4(&data->TextureTransform);

		//更新shader中的世界变换 常量缓冲区
		FObjectTransformation ObjectTransformation;
		XMStoreFloat4x4(&ObjectTransformation.World, MatrixWorld);
		XMStoreFloat4x4(&ObjectTransformation.TextureTransformation, XMMatrixTranspose(MatrixTextureTransform));
		const shared_ptr<CMaterial> material = data->MeshComponet->GetMaterials()[0];
		ObjectTransformation.MaterialIndex = material->GetMaterialTextureMapIndex();
		GeometryMap->MeshConstantBufferView.Update(data->MeshObjectOffset, &ObjectTransformation);
	}
}

void FRenderingLayer::PreDraw(float DeltaTime)
{
}

void FRenderingLayer::Draw(float DeltaTime)
{
	//for (auto& data : GeometryDescDatas)
	//{
	//	DrawObject(DeltaTime, data);
	//}
	DrawAllObjectsByLayer(DeltaTime);
}

void FRenderingLayer::PostDraw(float DeltaTime)
{
	// 删除弱指针
	std::vector<std::vector<std::weak_ptr<FGeometryDescData>>::const_iterator> RemoveGeometryDescData = {};
	for (std::vector<std::weak_ptr<FGeometryDescData>>::const_iterator it = GeometryDescDatas.begin(); it != GeometryDescDatas.end(); ++it)
	{
		if (it->expired())
		{
			RemoveGeometryDescData.push_back(it);
		}
		
	}
	for (auto& data : RemoveGeometryDescData)
	{
		GeometryDescDatas.erase(data);
	}
}

void FRenderingLayer::BuildPSO()
{
	BuildShader();
	DirectXPiepelineState->BuildParam(D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE::D3D12_CULL_MODE_BACK);
}

void FRenderingLayer::DrawObject(float DeltaTime, const std::weak_ptr<FGeometryDescData>& weak_data)
{
	if (weak_data.expired())
	{
		Engine_Log_Error("FRenderingLayer::DrawObject weak_data is nullptr");
		return;
	}

	std::shared_ptr<FGeometryDescData> data = weak_data.lock();

	UINT MeshObjectConstantViewSize = GeometryMap->MeshConstantBufferView.GetBufferByteSize();
	const D3D12_GPU_VIRTUAL_ADDRESS virtualAddressBegin = GeometryMap->MeshConstantBufferView.GetBuffer()->GetGPUVirtualAddress();

	//获取geometry的顶点缓冲区描述
	D3D12_VERTEX_BUFFER_VIEW VBV = GeometryMap->Geometrys[data->GeometryKey].GetVertexBufferView();
	//获取geometry的索引缓冲区描述
	D3D12_INDEX_BUFFER_VIEW IBV = GeometryMap->Geometrys[data->GeometryKey].GetIndexBufferView();
	//向命令列表中 添加顶点缓冲区描述(描述geometry的顶点信息)
	GetGraphicsCommandList()->IASetVertexBuffers(0, 1, &VBV);
	//向命令列表中 添加索引缓冲区描述(描述geometry的索引信息)
	GetGraphicsCommandList()->IASetIndexBuffer(&IBV);
	//向命令列表中 添加图元拓扑 命令(决定geometry的拓扑方式)
	const shared_ptr<CMaterial> material = data->MeshComponet->GetMaterials()[0];
	EMaterialDisplayStatusType TopologyType = material->GetMaterialDisplayStatusType();
	GetGraphicsCommandList()->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)TopologyType);
	//设置当前meshobject的gpu地址
	D3D12_GPU_VIRTUAL_ADDRESS Address = virtualAddressBegin + data->MeshObjectOffset * MeshObjectConstantViewSize;
	//将meshObjectConstantView中的数据放到0号寄存器中
	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(0, Address);

	// Draw Call !!!
	GetGraphicsCommandList()->DrawIndexedInstanced(
		data->IndexSize,				//绘制的实例所需的索引数量
		1,							//绘制的实例个数
		data->IndexoffsetPosition,	//索引缓冲数据的起始偏移
		data->VertexoffsetPostion,	//顶点缓冲数据的起始偏移
		0);							//从顶点缓冲区读取每个实例数据之前添加到每个索引的值
}

void FRenderingLayer::DrawObjectByLayer(float DeltaTime, const CMeshComponent* InKey)
{
	for (auto& GeometryDescData_weak : GeometryDescDatas)
	{
		if (GeometryDescData_weak.expired())
		{
			Engine_Log_Error("FRenderingLayer::DrawObjectByLayer GeometryDescData_weak is nullptr");
			continue;
		}
		std::shared_ptr<FGeometryDescData> GeometryDescData = GeometryDescData_weak.lock();
		if (GeometryDescData->MeshComponet->GetGuid() == InKey->GetGuid())
		{
			DrawObject(DeltaTime, GeometryDescData);
			break;
		}
		
	}
}

void FRenderingLayer::ResetPSO()
{
	// 父类这里先是空的
}

void FRenderingLayer::AddGeometryDescData(std::weak_ptr<FGeometryDescData> InGeometryDescData)
{
	GeometryDescDatas.emplace_back(InGeometryDescData);
}

void FRenderingLayer::RemoveGeometryDescData(std::weak_ptr<FGeometryDescData> InGeometryDescData)
{
	if (InGeometryDescData.expired())
	{
		Engine_Log_Error("FRenderingLayer::RemoveGeometryDescData InGeometryDescData is nullptr");
		return ;
	}

	auto DescData = InGeometryDescData.lock();

	for (std::vector<std::weak_ptr<FGeometryDescData>>::iterator Iter = GeometryDescDatas.begin(); Iter != GeometryDescDatas.end(); ++Iter)
	{
		if (Iter->lock() == DescData)
		{
			GeometryDescDatas.erase(Iter);
			break;
		}
	}
}

void FRenderingLayer::ClearGeometryDescData()
{
	GeometryDescDatas.clear();
}

void FRenderingLayer::DrawAllObjectsByLayer(float DeltaTime)
{
	for (auto& data_weak : GeometryDescDatas)
	{
		if (data_weak.expired())
		{
			Engine_Log_Error("FRenderingLayer::DrawAllObjectsByLayer data_weak is nullptr");
			continue;
		}
		std::shared_ptr<FGeometryDescData> data = data_weak.lock();
		DrawObject(DeltaTime, data);
	}
}
