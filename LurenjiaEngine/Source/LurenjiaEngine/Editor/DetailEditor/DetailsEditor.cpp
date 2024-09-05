#include "DetailsEditor.h"
#include "Mapping/Property/BoolDetailsMapping.h"
#include "Mapping/Property/FloatDetailsMapping.h"
#include "Mapping/Property/IntDetailsMapping.h"
#include "Mapping/Property/StringDetailsMapping.h"
#include "Mapping/Property/MapDetailsMapping.h"
#include "Mapping/Property/ArrayDetailsMapping.h"
#include "Mapping/Property/Vector3DetailsMapping.h"
#include "Mapping/Class/ActorDetailsMapping.h"
#include "Core/RegisterDetailsMapping.h"
#include "../../Engine/Actor/Core/Actor.h"
#include "../SelectEditor/OperationHandle/Core/OperationHandleBase.h"
#include "CoreObject/Property.h"

void FDetailsEditor::BuildEditor()
{

	FRegisterDetailsMapping::RegisterPropertyDetails("bool", FBoolDetailsMapping::MakeDetilsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("float", FFloatDetailsMapping::MakeDetilsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("int", FIntDetailsMapping::MakeDetilsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("string", FStringDetailsMapping::MakeDetilsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("map", FMapDetailsMapping::MakeDetilsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("vector", FArrayDetailsMapping::MakeDetilsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("XMFLOAT3", FVector3DetailsMapping::MakeDetilsMapping());

	FRegisterDetailsMapping::RegisterClassDetails("AActor", FActorDetailsMapping::MakeDetailsMapping());
}

void FDetailsEditor::DrawEditor(float DeltaTime)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("DetailsEditor");

	{
		ImGui::BeginChild("DetailsEditor panel", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

		if (AOperationHandleBase::SelectedActor)
		{
			UProperty* InProperty = AOperationHandleBase::SelectedActor->GetNativeClass().Property;
			while (InProperty)
			{
				auto Iter = FRegisterDetailsMapping::PropertyDetailsMap.find(InProperty->GetType());
				if (Iter != FRegisterDetailsMapping::PropertyDetailsMap.end())
				{
					Iter->second->UpdateDetailsWidget(InProperty);
				}

				InProperty = dynamic_cast<UProperty*>(InProperty->Next);
			}

		}

		ImGui::EndChild();
	}
	ImGui::End();

}

void FDetailsEditor::ExitEditor()
{
}
