#include "Component.h"

CComponent::CComponent()
{
	Parent = nullptr;
}

void CComponent::AddChildren(CComponent* InChildren)
{
	Children->push_back(InChildren);
}