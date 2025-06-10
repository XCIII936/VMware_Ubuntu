#include "dask.h"
#include <climits>

DiskArm::DiskArm() : RequestNumber(0), CurrentCylinder(50), 
                    SeekDirection(1), SeekNumber(0), SeekChang(0) {
    std::cout << "Please input current Cylinder: ";
    std::cin >> CurrentCylinder;
    
    std::cout << "Please input current direction (0: inward/1: outward): ";
    std::cin >> SeekDirection;
    
    std::cout << "Please input request numbers: ";
    std::cin >> RequestNumber;
    
    std::cout << "Please input request cylinder string: ";
    Request.resize(RequestNumber);
    Cylinder.resize(RequestNumber);
    for (int i = 0; i < RequestNumber; i++) {
        std::cin >> Request[i];
        Cylinder[i] = Request[i];
    }
}

DiskArm::~DiskArm() {
    // vector会自动释放内存
}

void DiskArm::InitSpace(const char* MethodName) {
    std::cout << "\n=== " << MethodName << " ===" << std::endl;
    SeekNumber = 0;
    SeekChang = 0;
    Cylinder = Request;
}

void DiskArm::Report() {
    std::cout << "\nSeek Number: " << SeekNumber << std::endl;
    std::cout << "Change Direction: " << SeekChang << std::endl;
}

void DiskArm::GenerateRandomRequests(int count, int maxCylinder) {
    Request.clear();
    Cylinder.clear();
    RequestNumber = count;
    
    srand(time(0));
    for(int i = 0; i < RequestNumber; i++) {
        int req = rand() % maxCylinder + 1;
        Request.push_back(req);
        Cylinder.push_back(req);
    }
    
    std::cout << "Generated random requests: ";
    for(int req : Request) {
        std::cout << req << " ";
    }
    std::cout << std::endl;
}

void DiskArm::Visualize(int currentPos, const std::vector<int>& path) {
    const int width = 50;
    int maxCylinder = *std::max_element(Request.begin(), Request.end());
    if (maxCylinder < width) maxCylinder = width;
    
    double scale = static_cast<double>(width) / maxCylinder;
    
    std::cout << "\nVisualization:\n0";
    for (int i = 1; i <= width; i++) {
        if (i % 5 == 0) std::cout << "|";
        else std::cout << "-";
    }
    std::cout << maxCylinder << std::endl;
    
    for (int pos : path) {
        int scaledPos = static_cast<int>(pos * scale);
        std::cout << std::string(scaledPos, ' ') << "o" << std::endl;
    }
    
    int scaledCurrent = static_cast<int>(currentPos * scale);
    std::cout << std::string(scaledCurrent, ' ') << "H" << std::endl;
}

void DiskArm::FCFS() {
    InitSpace("FCFS");
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    
    std::vector<int> path;
    path.push_back(Current);
    
    std::cout << Current;
    for (int i = 0; i < RequestNumber; i++) {
        bool needChange = ((Cylinder[i] >= Current) && !Direction) ||
                         ((Cylinder[i] < Current) && Direction);
        if (needChange) {
            Direction = !Direction;
            SeekChang++;
            std::cout << "\n" << Current << "->" << Cylinder[i];
        } else {
            std::cout << "->" << Cylinder[i];
        }
        
        SeekNumber += abs(Current - Cylinder[i]);
        Current = Cylinder[i];
        path.push_back(Current);
    }
    
    Visualize(CurrentCylinder, path);
    Report();
}

void DiskArm::SSTF() {
    InitSpace("SSTF");
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    
    std::vector<int> temp = Request;
    std::vector<int> path;
    path.push_back(Current);
    
    std::cout << Current;
    for (int i = 0; i < RequestNumber; i++) {
        int minDist = INT_MAX;
        int index = -1;
        
        for (int j = 0; j < temp.size(); j++) {
            if (temp[j] != -1) {
                int dist = abs(Current - temp[j]);
                if (dist < minDist) {
                    minDist = dist;
                    index = j;
                }
            }
        }
        
        bool needChange = ((temp[index] >= Current) && !Direction) ||
                         ((temp[index] < Current) && Direction);
        if (needChange) {
            Direction = !Direction;
            SeekChang++;
            std::cout << "\n" << Current << "->" << temp[index];
        } else {
            std::cout << "->" << temp[index];
        }
        
        SeekNumber += minDist;
        Current = temp[index];
        temp[index] = -1;
        path.push_back(Current);
    }
    
    Visualize(CurrentCylinder, path);
    Report();
}

void DiskArm::SCAN() {
    InitSpace("SCAN");
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    
    std::vector<int> sorted = Request;
    std::sort(sorted.begin(), sorted.end());
    
    std::vector<int> path;
    path.push_back(Current);
    
    auto start = std::lower_bound(sorted.begin(), sorted.end(), Current);
    
    if (Direction == 1) { // Outward
        for (auto it = start; it != sorted.end(); ++it) {
            path.push_back(*it);
            SeekNumber += abs(Current - *it);
            Current = *it;
        }
        
        if (start != sorted.begin()) {
            SeekChang++;
            path.push_back(*(start-1));
            SeekNumber += abs(Current - *(start-1));
            Current = *(start-1);
            
            for (auto it = start-1; it != sorted.begin(); --it) {
                path.push_back(*(it-1));
                SeekNumber += abs(Current - *(it-1));
                Current = *(it-1);
            }
        }
    } else { // Inward
        for (auto it = start-1; it >= sorted.begin(); --it) {
            path.push_back(*it);
            SeekNumber += abs(Current - *it);
            Current = *it;
        }
        
        if (start != sorted.end()) {
            SeekChang++;
            path.push_back(*start);
            SeekNumber += abs(Current - *start);
            Current = *start;
            
            for (auto it = start+1; it != sorted.end(); ++it) {
                path.push_back(*it);
                SeekNumber += abs(Current - *it);
                Current = *it;
            }
        }
    }
    
    std::cout << "Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) std::cout << "->";
        std::cout << path[i];
    }
    std::cout << std::endl;
    
    Visualize(CurrentCylinder, path);
    Report();
}

void DiskArm::CSCAN() {
    InitSpace("C-SCAN");
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    
    std::vector<int> sorted = Request;
    std::sort(sorted.begin(), sorted.end());
    
    std::vector<int> path;
    path.push_back(Current);
    
    auto start = std::lower_bound(sorted.begin(), sorted.end(), Current);
    
    if (Direction == 1) { // Outward
        for (auto it = start; it != sorted.end(); ++it) {
            path.push_back(*it);
            SeekNumber += abs(Current - *it);
            Current = *it;
        }
        
        if (!sorted.empty()) {
            SeekChang++;
            path.push_back(sorted.front());
            SeekNumber += abs(Current - sorted.front());
            Current = sorted.front();
            
            for (auto it = sorted.begin(); it != start; ++it) {
                path.push_back(*it);
                SeekNumber += abs(Current - *it);
                Current = *it;
            }
        }
    } else { // Inward
        for (auto it = start-1; it >= sorted.begin(); --it) {
            path.push_back(*it);
            SeekNumber += abs(Current - *it);
            Current = *it;
        }
        
        if (!sorted.empty()) {
            SeekChang++;
            path.push_back(sorted.back());
            SeekNumber += abs(Current - sorted.back());
            Current = sorted.back();
            
            for (auto it = sorted.end()-1; it != start; --it) {
                path.push_back(*it);
                SeekNumber += abs(Current - *it);
                Current = *it;
            }
        }
    }
    
    std::cout << "Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) std::cout << "->";
        std::cout << path[i];
    }
    std::cout << std::endl;
    
    Visualize(CurrentCylinder, path);
    Report();
}

void DiskArm::LOOK() {
    InitSpace("LOOK");
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    
    std::vector<int> sorted = Request;
    std::sort(sorted.begin(), sorted.end());
    
    std::vector<int> path;
    path.push_back(Current);
    
    auto start = std::lower_bound(sorted.begin(), sorted.end(), Current);
    
    if (Direction == 1) { // Outward
        for (auto it = start; it != sorted.end(); ++it) {
            path.push_back(*it);
            SeekNumber += abs(Current - *it);
            Current = *it;
        }
        
        if (start != sorted.begin()) {
            SeekChang++;
            path.push_back(*(start-1));
            SeekNumber += abs(Current - *(start-1));
            Current = *(start-1);
            
            for (auto it = start-1; it != sorted.begin(); --it) {
                path.push_back(*(it-1));
                SeekNumber += abs(Current - *(it-1));
                Current = *(it-1);
            }
        }
    } else { // Inward
        for (auto it = start-1; it >= sorted.begin(); --it) {
            path.push_back(*it);
            SeekNumber += abs(Current - *it);
            Current = *it;
        }
        
        if (start != sorted.end()) {
            SeekChang++;
            path.push_back(*start);
            SeekNumber += abs(Current - *start);
            Current = *start;
            
            for (auto it = start+1; it != sorted.end(); ++it) {
                path.push_back(*it);
                SeekNumber += abs(Current - *it);
                Current = *it;
            }
        }
    }
    
    std::cout << "Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i != 0) std::cout << "->";
        std::cout << path[i];
    }
    std::cout << std::endl;
    
    Visualize(CurrentCylinder, path);
    Report();
}

int main() {
    DiskArm diskArm;
    
    // 测试所有算法
    diskArm.FCFS();
    diskArm.SSTF();
    diskArm.SCAN();
    diskArm.CSCAN();
    diskArm.LOOK();
    
    // 测试随机请求生成
    std::cout << "\n=== Testing Random Request Generation ===" << std::endl;
    diskArm.GenerateRandomRequests(10, 200);
    
    // 用随机请求再次测试
    diskArm.FCFS();
    diskArm.SSTF();
    diskArm.SCAN();
    diskArm.CSCAN();
    diskArm.LOOK();
    
    return 0;
}
