#include "ConstructionComponents.h"

#include "../CoreObject/CoreMinimalObject.h"
#include "../Component/Component.h"

namespace ConstructionComponent
{
	void UpdateConstructionComponents(CComponent* InComponent, CComponent* InParentComponent)
	{
		if (InParentComponent != InComponent)//不能Attach自己
		{
			//设置到子类
			InParentComponent->AddChildren(InComponent);

			//设置父类是谁
			InComponent->SetParentComponent(InParentComponent);
		}
	}

	void ConstructionComponents(CCoreMinimalObject* InOuter, CCoreMinimalObject* NewObject)
	{
		if (CComponent* InComponent = dynamic_cast<CComponent*>(NewObject))
		{
			if (InOuter)
			{
				//Outer是一个组件
				if (CComponent* InOuterComponent = dynamic_cast<CComponent*>(InOuter))
				{
					UpdateConstructionComponents(InComponent, InOuterComponent);
				}
				//Outer是一个对象
				//else if (GActorObject* InOuterActor = dynamic_cast<GActorObject*>(InOuter))
				//{
				//	if (InOuterActor->GetRootComponent())
				//	{
				//		UpdateConstructionComponents(InComponent, InOuterActor->GetRootComponent());
				//	}
				//}
			}
		}
	}
}