#ifndef _MEM_LEAK_DETECTOR_H
#define _MEM_LEAK_DETECTOR_H

#ifdef _DEBUG
	/**/
	#define _CRTDBG_MAP_ALLOC

	#include <stdlib.h>
	#include "crtdbg.h"
	#define DEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define DEBUG_NEW
#endif

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif  

//检测器，放在main()函数中的任何地方（建议放在main入口处）
void memoryLeakDetector();

//在分配第几块内存的时候断点中断调试，在内存泄露不返回行号的时候，用该方法调试一样管用
void setBreakPoint(int alloc_num);


#endif