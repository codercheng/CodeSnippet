### mem_leak_detector
   该程序是针对windows下面的project进行内存泄露检测的, 主要是借助 Visual Studio 和 C 运行时 (CRT) 库，检测和识别内存泄漏。
   我们知道linux下内存泄漏检测会用到`valgrind`，简单好用，那么windows下程序内存泄漏和定位怎么比较简单？
   其实VS自己本来就提供这种功能，只需要添加*`几行代码`*即可，下面对这几行代码做了封装，以备后续使用：
   * 添加`mem_leak_detector.h mem_leak_detector.cpp`(或者.c)到要检测的(C/C++)项目中
   * VS调到debug模式
   * `memoryLeakDetector()`放到程序的入口出(其实放在程序中main函数任何可执行到的地方都可以，不过建议放在main入口出)，另：如果调用dll，要检测dll中的内存泄漏，需要把`memoryLeakDetector()`放入到dll里的入口
   * 需要在每个cpp中添加`#include "mem_leak_detector.h"`,如果项目文件过多，为了方便，可以在VS中设置：`右键->Property->C/C++->Advanced->Forced include file`把mem_leak_detector.h包含进去即可。
   正常情况程序检测到泄露后，VS的outpu里面会出现：
    ```
    Detected memory leaks!
    Dumping objects ->
    C:\PROGRAM FILES\VISUAL STUDIO\MyProjects\leaktest\leaktest.cpp(20) : {18}
    normal block at 0x00780E80, 64 bytes long.
     Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
    Object dump complete.
    ```
    如果没有出现文件名和行号，如下：
    ```
    Detected memory leaks!
    Dumping objects ->
    {18} normal block at 0x00780E80, 64 bytes long.
    Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
    Object dump complete.
    ```
    只出现了`18`normal block号，表示第18次分配new/malloc的时候出现泄露，可在程序中添加`setBreakPoint(18)`再次调试程序，这次，程序会在第18次申请内存的时候中断，通过call stack可以很好的排查到这次泄露的位置。
    
    demo:
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include "mem_leak_detector.h"

    int main()
    {
	    memoryLeakDetector();
	    char *p1 = new char [100];
	    char *p2 = (char *)malloc(100* sizeof(char));
	    return 0;
    }
    ```
