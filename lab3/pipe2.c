#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// 计算阶乘 f(x)
int factorial(int x) {
    if (x == 1) return 1;
    return x * factorial(x - 1);
}

// 计算斐波那契 f(y)
int fibonacci(int y) {
    if (y == 1 || y == 2) return 1;
    return fibonacci(y - 1) + fibonacci(y - 2);
}

int main() {
    int x, y;
    printf("Enter x and y : ");
    scanf("%d %d", &x, &y);

    // 创建两个管道：pipe1[2]（P1→P3），pipe2[2]（P2→P3）
    int pipe1[2], pipe2[2];
    if (pipe(pipe1)) { perror("pipe1 failed"); exit(1); }
    if (pipe(pipe2)) { perror("pipe2 failed"); exit(1); }

    pid_t pid1, pid2;

    // 进程 P1：计算 f(x)
    if ((pid1 = fork()) == 0) {
        close(pipe1[0]);  // 关闭读端
        int fx = factorial(x);
        write(pipe1[1], &fx, sizeof(int));
        close(pipe1[1]);
        exit(0);
    }

    // 进程 P2：计算 f(y)
    if ((pid2 = fork()) == 0) {
        close(pipe2[0]);  // 关闭读端
        int fy = fibonacci(y);
        write(pipe2[1], &fy, sizeof(int));
        close(pipe2[1]);
        exit(0);
    }

    // 进程 P3（主进程）：等待 P1 和 P2 的结果
    close(pipe1[1]);  // 关闭写端
    close(pipe2[1]);

    int fx, fy;
    read(pipe1[0], &fx, sizeof(int));
    read(pipe2[0], &fy, sizeof(int));
    close(pipe1[0]);
    close(pipe2[0]);

    // 输出 fx 和 fy
    printf("f(x) = f(%d) = %d\n", x, fx);
    printf("f(y) = f(%d) = %d\n", y, fy);

    // 计算 f(x, y) = f(x) + f(y)
    printf("f(%d, %d) = %d + %d = %d\n", x, y, fx, fy, fx + fy);

    waitpid(pid1, NULL, 0);  // 等待 P1 结束
    waitpid(pid2, NULL, 0);  // 等待 P2 结束
    return 0;
}
