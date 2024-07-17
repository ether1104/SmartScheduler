#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>

// 矩阵大小
#define N 2000
#define NUM_THREADS 8

// 矩阵A、B和结果矩阵C
double A[N][N], B[N][N], C[N][N];

// 线程参数结构体
typedef struct {
    int thread_id;
    int num_threads;
} thread_param_t;

// 初始化矩阵
void initialize_matrices() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
            C[i][j] = 0.0;
        }
    }
}

// 矩阵乘法线程函数
void* multiply_matrices(void* param) {
    thread_param_t* p = (thread_param_t*) param;
    int thread_id = p->thread_id;
    int num_threads = p->num_threads;

    // 设置实时调度优先级
    struct sched_param sched_param;
    sched_param.sched_priority = 99 - thread_id; // 根据线程ID设置不同优先级
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &sched_param);

    for (int i = thread_id; i < N; i += num_threads) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

// 获取当前时间（秒）
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

// 主函数
int main() {
    pthread_t threads[NUM_THREADS];
    thread_param_t thread_params[NUM_THREADS];
    double start_time, end_time;

    // 初始化矩阵
    initialize_matrices();

    // 记录开始时间
    start_time = get_time();

    // 创建线程
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_params[i].thread_id = i;
        thread_params[i].num_threads = NUM_THREADS;
        pthread_create(&threads[i], NULL, multiply_matrices, (void*)&thread_params[i]);
    }

    // 等待所有线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 记录结束时间
    end_time = get_time();

    // 打印结果矩阵的前5个元素（用于验证结果）
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%f ", C[i][j]);
        }
        printf("\n");
    }

    // 打印执行时间
    printf("Execution time: %f seconds\n", end_time - start_time);

    return 0;
}

