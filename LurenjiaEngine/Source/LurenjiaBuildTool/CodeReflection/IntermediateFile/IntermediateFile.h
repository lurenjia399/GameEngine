#pragma once

#include "../CodeType.h"
#include "../Tool/HelperTool.h"


namespace IntermediateFile
{
	bool Builder(const FClassAnalysis& InClassAnalysis, vector<string>& OutAnalysisRawH, vector<string>& OutAnalysisRawCPP);

	void GenerateFile_H(const FClassAnalysis& InClassAnalysis, vector<string>& StaticRegistration, vector<string>& OutAnalysisRaw);
	void GenerateFile_CPP(const FClassAnalysis& InClassAnalysis, vector<string>& StaticRegistration, vector<string>& OutAnalysisRaw);
}