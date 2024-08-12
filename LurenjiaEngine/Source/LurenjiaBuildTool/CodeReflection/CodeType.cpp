#include "CodeType.h"
#include "Tool/HelperTool.h"

bool bIsCheckAllowCodeReflection(const std::vector<std::string>& InContent)
{
    int count = 0;
    for (const std::string& Temp : InContent)
    {
        if (helper_tool_files::string_contain(Temp, "GENERATED_BODY") ||
            helper_tool_files::string_contain(Temp, ".generated.h"))
        {
            count++;
        }
    }

    return count >= 2;
}
