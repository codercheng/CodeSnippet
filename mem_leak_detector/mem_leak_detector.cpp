#include "mem_leak_detector.h"

void memoryLeakDetector() {
#ifdef _DEBUG
	_CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void setBreakPoint(int alloc_num) {
#ifdef _DEBUG
	_CrtSetBreakAlloc(alloc_num);
#endif
}