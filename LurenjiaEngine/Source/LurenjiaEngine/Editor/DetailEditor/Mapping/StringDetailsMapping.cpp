#include "StringDetailsMapping.h"
#include "CoreObject/Property.h"

void FStringDetailsMapping::UpdateDetailsWidget(UProperty* InProperty)
{
	char Buff[1024] = { 0 };
	string* BuffChar = InProperty->GetData<string>();
	strcpy_s(Buff, BuffChar->c_str());
	ImGui::InputText(InProperty->GetName().c_str(), 
		Buff, 
		1024, 
		ImGuiInputTextFlags_CallbackEdit,
		FStringDetailsMapping::callback,
		InProperty->GetData());
}

int FStringDetailsMapping::callback(ImGuiInputTextCallbackData* InData)
{
	string* userdataStr = (string*)InData->UserData;//反射保存的字符串
	*userdataStr = InData->Buf;//将 用户ui输入字符串 保存到反射中
	return 0;
}

std::shared_ptr<FStringDetailsMapping> FStringDetailsMapping::MakeDetilsMapping()
{
	return std::make_shared<FStringDetailsMapping>();
}
