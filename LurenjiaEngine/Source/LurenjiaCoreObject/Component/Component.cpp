#include "Component.h"

CComponent::CComponent(): Parent(nullptr), Children(nullptr)
{
}

void CComponent::AddChildren(CComponent* InChildren)
{
	Children->push_back(InChildren);
}