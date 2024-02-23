#pragma once

#define ANALYSIS_HRESULT(InValue) \
{\
	HRESULT Value = InValue;\
	if(FAILED(Value))\
	{\
		Engine_Log_Error("Error = %i", (int)Value);\
		assert(0);\
	}\
	else if(SUCCEEDED(Value))\
	{\
	}\
}
//Engine_Log_Success("Success !"); \