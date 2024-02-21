#pragma once

#if (EDITOR_ENGINE == 1)
#include "../../../Editor/LogEditor/Core/LogSystem.h"
#endif

#if defined(_WIN32)
#include "../../../simple_library/public/simple_library.h"


#if (EDITOR_ENGINE == 1)

#define Engine_Log(format, ...)\
	log_log(format, __VA_ARGS__)\
	FEditorLogSystem::GetEditorLogSystem()->AddLog(format, __VA_ARGS__);
	
#define Engine_Log_Success(format, ...)\
	log_success(format, __VA_ARGS__)\
	FEditorLogSystem::GetEditorLogSystem()->AddLogSuccess(format, __VA_ARGS__);

#define Engine_Log_Error(format, ...)\
	log_error(format, __VA_ARGS__)\
	FEditorLogSystem::GetEditorLogSystem()->AddLogError(format, __VA_ARGS__);

#define Engine_Log_Warning(format, ...)\
	log_warning(format, __VA_ARGS__)\
	FEditorLogSystem::GetEditorLogSystem()->AddLogWarning(format, __VA_ARGS__);
#else

#define Engine_Log(format, ...)				log_log(format, __VA_ARGS__)
#define Engine_Log_Success(format, ...)		log_success(format, __VA_ARGS__)
#define Engine_Log_Error(format, ...)		log_error(format, __VA_ARGS__)
#define Engine_Log_Warning(format, ...)		log_warning(format, __VA_ARGS__)

#endif




#elif 0

#define Engine_log(format, ...)
#define Engine_Log_Success(format, ...)
#define Engine_Log_Error(format, ...)
#define Engine_Log_Warning(format, ...)

#endif


#define _TU_CHECK_VALUE(_m)  (1 == (_m))
