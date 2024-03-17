#include "simple_library/public/simple_library.h"
#include "CodeReflection/Collect/CollectClassInfo.h"


int main()
{
	char path[] = "D:/GitHub/GameEngine/LurenjiaEngine/LurenjiaEngine/Engine/Test/CodeReflection/CodeReflectionTest.h";
	//normalization_path(path);
	FClassAnalysis ClassAnalysis = {};
	CollectClassInfo::Collection(path, ClassAnalysis);
	return 0;
}