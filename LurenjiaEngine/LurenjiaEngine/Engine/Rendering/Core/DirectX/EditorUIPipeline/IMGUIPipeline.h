#include "../../../../Interface/DirectXDeviceInterface.h"


struct FIMGUIPipeline : public IDirectXDeviceInterface_struct
{
public:
	FIMGUIPipeline();

	void Init(ID3D12DescriptorHeap* InHeap, UINT InOffset);

	void Draw(float DeltaTime);
	void Tick(float DeltaTime);

	void Exit();

private:
		bool bClicked = false;
};