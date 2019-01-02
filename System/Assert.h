#pragma once

#include <System/Log.h>

namespace Columbus
{

	#define COLUMBUS_ASSERT(EXP) { EXP ? (void)0 : Log::Fatal("%s:%d Assertation: %s", __FILE__, __LINE__, #EXP); }
	#define COLUMBUS_ASSERT_MESSAGE(EXP, MES) { EXP ? (void)0 : Log::Fatal("%s:%d Assertation: %s", __FILE__, __LINE__, #MES); }

}


