//#include <stdafx.h>
#include "DL_StackWalker.h"
#include "DL_Debug.h"

using namespace DL_Debug;

StackWalker::StackWalker()
:BaseStackWalker()
{
}

StackWalker::StackWalker(unsigned int aProcessId, HANDLE aProcess)
:BaseStackWalker(aProcessId,aProcess)
{
}
void StackWalker::OnOutput(char* aString)
{
	DL_PRINT(aString);
}


