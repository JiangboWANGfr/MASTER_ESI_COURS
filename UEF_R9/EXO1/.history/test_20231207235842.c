#include "stdio.h"
#include "includes.h"

#define TASK_STK_SIZE 2048
OS_STK task1_stk[TASK_STK_SIZE];
OS_STK task2_stk[TASK_STK_SIZE];
#define TASK1_PRIO 30
#define TASK2_PRIO 40
int compteur = 0;

OS_EVENT *sem; // declare a semaphore

void task1(void *pdata)
{
    INT8U err;
    while (1)
    {
        OSSemPend(sem, 0, &err); // wait for semaphore
        compteur++;
        printf("TASK 1: compteur = %d\n", compteur);
        OSSemPost(sem); // release semaphore
        OSTimeDlyHMSM(0, 0, 3, 0);
    }
    // 这里不需要再次调用OSSemPost，因为task1不会再次尝试访问compteur
}

void task2(void *pdata)
{
    INT8U err;
    while (1)
    {
        OSSemPend(sem, 0, &err); // 等待信号量
        compteur++;
        printf("TASK 2: compteur = %d\n", compteur);
        OSSemPost(sem); // 释放信号量
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

int main(void)
{
    OSInit();
    sem = OSSemCreate(1); // 初始化信号量为1

    OSTaskCreate(task1, NULL, (void *)&task1_stk[TASK_STK_SIZE - 1], TASK1_PRIO);
    OSTaskCreate(task2, NULL, (void *)&task2_stk[TASK_STK_SIZE - 1], TASK2_PRIO);
    OSStart();
}
