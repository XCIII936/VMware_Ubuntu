#ifndef DISK_ARM_H
#define DISK_ARM_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

class DiskArm {
public:
    DiskArm();
    ~DiskArm();
    
    void InitSpace(const char* MethodName);
    void Report();
    void GenerateRandomRequests(int count, int maxCylinder);
    
    void FCFS();
    void SSTF();
    void SCAN();
    void CSCAN();
    void LOOK();
    
    void Visualize(int currentPos, const std::vector<int>& path);

private:
    std::vector<int> Request;
    std::vector<int> Cylinder;
    int RequestNumber;
    int CurrentCylinder;
    int SeekDirection;  // 0: inward, 1: outward
    int SeekNumber;
    int SeekChang;
};

#endif
