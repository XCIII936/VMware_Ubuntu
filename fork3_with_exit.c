#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>   //wait隐式声明
#include <stdlib.h>
#include <sys/wait.h> //添加该文件来声明wait()
int value=5;     //全局变量 数据段，父子进程独立修改
int main(){
    int i;       //局部变量 栈，父子进程独立
    pid_t pid;
    for(i=0;i<2;i++){//无 exit(0)：4 进程，3 对输出（P0 未为 P2 打印 "PARENT"）。
        pid=fork();//有 exit(0)：3 进程，2 对输出。
        if(pid==0){
            value += 15;
            printf("Child: value = %d\n",value);
        }
        else if(pid>0){
            wait(NULL);
            printf("PARENT: value = %d\n",value);
            exit(0);   // 有 exit(0)：父进程立即退出，减少子进程数量。
			//无 exit(0)：父进程继续循环，可能创建更多子进程。
        }
    }
}
