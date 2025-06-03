#ifndef VMRP_H
#define VMRP_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>  // 添加cstring头文件
using namespace std;

class Replace {
public:
    Replace();
    ~Replace();
    void InitSpace(const char *MethodName);  // 修改为const char*
    void Report(void);
    void Fifo(void);
    void Lru(void);
    void Clock(void);
    void EnhancedClock(void);
    void RandomTest(int pages, int frames, int length);
    
private:
    int *ReferencePage;
    int *EliminatePage;
    int *PageFrames;
    int *ReferenceBits;
    int *ModifiedBits;
    int PageNumber;
    int FrameNumber;
    int FaultNumber;
    
    void GenerateRandomReference(int length);
    void DisplayPages(int replaced = -1);
};

#endif
