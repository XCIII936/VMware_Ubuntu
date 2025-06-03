# 内存页面置换算法实验

## 一、实验目的
加深对于存储管理的了解，掌握虚拟存储器的实现原理；观察和了解重要的页面置换算法及置换过程。练习模拟算法的编程技巧，锻炼分析实验数据的能力。

## 二、示例实验
示例代码有些许错误如:
eplace::Lru() 方法中调用了 InitSpace("FIFO")，这显然不正确

代码中有一些语法错误和格式问题

缺少完整的头文件定义

通过增加头文件，编译查看错误(没下载g++也会报错)等更正。

代码及其运行截图:
![alt text](images\1.png)
![alt text](images\2.png)

## 三、独立实验
四个算法运行截图:
![alt text](images\3.png)
![alt text](images\4.png)
![alt text](images\5.png)
![alt text](images\6.png)
![alt text](images\7.png)

## 四、实验总结
发现下面结果:
Belady异常：FIFO算法在帧数增加时缺页率反而上升（3帧：9次缺页，4帧：10次缺页）

LRU优势：在局部性访问中，LRU比FIFO减少15-20%缺页

Clock算法表现：在随机访问模式下接近LRU，但实现更简单

增强Clock优势：在读写混合负载下（30%写操作）比普通Clock减少8%磁盘写入

FIFO曲线出现反常上升（Belady现象）

LRU和Clock随帧数增加单调递减

增强Clock在帧数≥3时表现最优

增强二次机会算法实现:
双位标记：每个页帧维护(reference_bit, modified_bit)状态

两轮扫描：优先替换干净未引用页，其次脏未引用页

写操作模拟：30%概率设置修改位

指针环形移动：pointer = (pointer + 1) % FrameNumber

结论:
算法性能：Enhanced Clock在多数测试场景下综合表现最佳，平均比FIFO减少22%缺页

实现复杂度：LRU > Enhanced Clock > Clock > FIFO

特殊现象：帧数增加不一定降低缺页率（Belady现象）