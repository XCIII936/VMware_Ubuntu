#include "vmrp.h"
#include <cstring>  // 确保包含cstring头文件

Replace::Replace() : ReferenceBits(nullptr), ModifiedBits(nullptr) {
    srand(time(nullptr));
    
    int choice=2;
    //cout << "1. Manual input\n2. Random test\nChoose mode: ";
    //cin >> choice;
    
    if(choice == 1) {
        cout << "Please input page numbers: ";
        cin >> PageNumber;
        ReferencePage = new int[PageNumber];
        EliminatePage = new int[PageNumber];
        
        cout << "Please input reference page string: ";
        for(int i = 0; i < PageNumber; i++)
            cin >> ReferencePage[i];
            
        cout << "Please input page frames: ";
        cin >> FrameNumber;
    } else {
        int pages, frames, length;
        cout << "Input max page number: ";
        cin >> pages;
        cout << "Input frame number: ";
        cin >> frames;
        cout << "Input reference length: ";
        cin >> length;
        RandomTest(pages, frames, length);
        return;
    }
    
    PageFrames = new int[FrameNumber];
}

Replace::~Replace() {
    delete[] ReferencePage;
    delete[] EliminatePage;
    delete[] PageFrames;
    delete[] ReferenceBits;
    delete[] ModifiedBits;
}

void Replace::InitSpace(const char *MethodName) {  // 修改为const char*
    cout << endl << "=== " << MethodName << " ===" << endl;
    FaultNumber = 0;
    
    for(int i = 0; i < PageNumber; i++)
        EliminatePage[i] = -1;
        
    for(int i = 0; i < FrameNumber; i++)
        PageFrames[i] = -1;
        
    if(strstr(MethodName, "Clock")) {
        delete[] ReferenceBits;
        ReferenceBits = new int[FrameNumber];
        for(int i = 0; i < FrameNumber; i++)
            ReferenceBits[i] = 0;
    }
    
    if(strstr(MethodName, "Enhanced")) {
        delete[] ReferenceBits;
        delete[] ModifiedBits;
        ReferenceBits = new int[FrameNumber];
        ModifiedBits = new int[FrameNumber];
        for(int i = 0; i < FrameNumber; i++) {
            ReferenceBits[i] = 0;
            ModifiedBits[i] = 0;
        }
    }
}
void Replace::Report(void) {
    cout << endl << "Eliminate page sequence: ";
    for(int i = 0; i < PageNumber && EliminatePage[i] != -1; i++)
        cout << EliminatePage[i] << " ";
    
    cout << endl << "Number of page faults = " << FaultNumber << endl;
    cout << "Page fault rate = " << fixed << setprecision(2) 
         << 100.0 * FaultNumber / PageNumber << "%" << endl;
}

void Replace::DisplayPages(int replaced) {
    for(int i = 0; i < FrameNumber; i++) {
        if(PageFrames[i] != -1) {
            cout << PageFrames[i];
            // 显示引用位和修改位(如果适用)
            if(ReferenceBits != nullptr) {
                cout << "(" << ReferenceBits[i];
                if(ModifiedBits != nullptr)
                    cout << "," << ModifiedBits[i];
                cout << ")";
            }
            cout << " ";
        }
    }
    if(replaced != -1)
        cout << "-> " << replaced;
    cout << endl;
}

// 先进先出置换算法
void Replace::Fifo(void) {
    InitSpace("FIFO");
    int pointer = 0;
    
    for(int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;
        
        // 检查页是否已在内存中
        for(int i = 0; i < FrameNumber; i++) {
            if(PageFrames[i] == next) {
                found = true;
                break;
            }
        }
        
        if(found) {
            DisplayPages();
            continue;
        }
        
        // 页错误处理
        FaultNumber++;
        int replaced = -1;
        
        if(PageFrames[pointer] != -1) {
            replaced = PageFrames[pointer];
            EliminatePage[k] = replaced;
        }
        
        PageFrames[pointer] = next;
        pointer = (pointer + 1) % FrameNumber;
        
        DisplayPages(replaced);
    }
    
    Report();
}

// 最近最久未使用置换算法
void Replace::Lru(void) {
    InitSpace("LRU");
    int *lastUsed = new int[FrameNumber];
    fill_n(lastUsed, FrameNumber, -1);
    
    for(int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;
        int foundIndex = -1;
        
        // 检查页是否已在内存中
        for(int i = 0; i < FrameNumber; i++) {
            if(PageFrames[i] == next) {
                found = true;
                foundIndex = i;
                break;
            }
        }
        
        if(found) {
            lastUsed[foundIndex] = k;
            DisplayPages();
            continue;
        }
        
        // 页错误处理
        FaultNumber++;
        int replaceIndex = 0;
        
        // 查找最近最久未使用的页
        for(int i = 1; i < FrameNumber; i++) {
            if(lastUsed[i] < lastUsed[replaceIndex]) {
                replaceIndex = i;
            }
        }
        
        int replaced = -1;
        if(PageFrames[replaceIndex] != -1) {
            replaced = PageFrames[replaceIndex];
            EliminatePage[k] = replaced;
        }
        
        PageFrames[replaceIndex] = next;
        lastUsed[replaceIndex] = k;
        
        DisplayPages(replaced);
    }
    
    delete[] lastUsed;
    Report();
}

// 二次机会(Clock)置换算法
void Replace::Clock(void) {
    InitSpace("Clock");
    int pointer = 0;
    
    for(int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;
        
        // 检查页是否已在内存中
        for(int i = 0; i < FrameNumber; i++) {
            if(PageFrames[i] == next) {
                found = true;
                ReferenceBits[i] = 1; // 设置引用位
                break;
            }
        }
        
        if(found) {
            DisplayPages();
            continue;
        }
        
        // 页错误处理
        FaultNumber++;
        int replaced = -1;
        bool replacedFlag = false;
        
        // 查找替换页
        while(!replacedFlag) {
            if(ReferenceBits[pointer] == 0) {
                // 找到替换页
                if(PageFrames[pointer] != -1) {
                    replaced = PageFrames[pointer];
                    EliminatePage[k] = replaced;
                }
                PageFrames[pointer] = next;
                ReferenceBits[pointer] = 1;
                replacedFlag = true;
            } else {
                // 给第二次机会
                ReferenceBits[pointer] = 0;
            }
            pointer = (pointer + 1) % FrameNumber;
        }
        
        DisplayPages(replaced);
    }
    
    Report();
}

// 增强型二次机会(Enhanced Clock)置换算法
void Replace::EnhancedClock(void) {
    InitSpace("Enhanced Clock");
    int pointer = 0;
    
    for(int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;
        
        // 检查页是否已在内存中
        for(int i = 0; i < FrameNumber; i++) {
            if(PageFrames[i] == next) {
                found = true;
                ReferenceBits[i] = 1; // 设置引用位
                // 有50%概率设置修改位(模拟实际场景)
                if(rand() % 2 == 0) ModifiedBits[i] = 1;
                break;
            }
        }
        
        if(found) {
            DisplayPages();
            continue;
        }
        
        // 页错误处理
        FaultNumber++;
        int replaced = -1;
        bool replacedFlag = false;
        int rounds = 0;
        
        // 查找替换页(最多扫描两轮)
        while(!replacedFlag && rounds < 2) {
            // 第一类: (0,0)
            for(int i = 0; i < FrameNumber && !replacedFlag; i++) {
                int idx = (pointer + i) % FrameNumber;
                if(ReferenceBits[idx] == 0 && ModifiedBits[idx] == 0) {
                    replaced = PageFrames[idx];
                    if(replaced != -1) EliminatePage[k] = replaced;
                    PageFrames[idx] = next;
                    ReferenceBits[idx] = 1;
                    ModifiedBits[idx] = 0;
                    pointer = (idx + 1) % FrameNumber;
                    replacedFlag = true;
                }
            }
            
            // 第二类: (0,1)
            if(!replacedFlag) {
                for(int i = 0; i < FrameNumber && !replacedFlag; i++) {
                    int idx = (pointer + i) % FrameNumber;
                    if(ReferenceBits[idx] == 0 && ModifiedBits[idx] == 1) {
                        replaced = PageFrames[idx];
                        if(replaced != -1) EliminatePage[k] = replaced;
                        PageFrames[idx] = next;
                        ReferenceBits[idx] = 1;
                        ModifiedBits[idx] = 0;
                        pointer = (idx + 1) % FrameNumber;
                        replacedFlag = true;
                    } else {
                        // 给第二次机会
                        ReferenceBits[idx] = 0;
                    }
                }
            }
            
            rounds++;
        }
        
        DisplayPages(replaced);
    }
    
    Report();
}

void Replace::GenerateRandomReference(int length) {
    PageNumber = length;
    ReferencePage = new int[PageNumber];
    EliminatePage = new int[PageNumber];
    
    // 生成随机引用串(基于局部性原理)
    int base = rand() % 5 + 1; // 1-5
    for(int i = 0; i < PageNumber; i++) {
        // 80%概率访问附近页面，20%概率随机访问
        if(rand() % 100 < 80) {
            ReferencePage[i] = base + rand() % 3 - 1; // -1,0,+1
            if(ReferencePage[i] < 1) ReferencePage[i] = 1;
        } else {
            ReferencePage[i] = rand() % 10 + 1; // 1-10
        }
        base = ReferencePage[i];
    }
    
    cout << "Generated reference string: ";
    for(int i = 0; i < PageNumber; i++)
        cout << ReferencePage[i] << " ";
    cout << endl;
}

void Replace::RandomTest(int pages, int frames, int length) {
    FrameNumber = frames;
    PageFrames = new int[FrameNumber];
    GenerateRandomReference(length);
    
    // 运行所有算法
    Fifo();
    Lru();
    Clock();
    EnhancedClock();
}

int main() {
    Replace *vmpr = new Replace();
    delete vmpr;
    return 0;
}
