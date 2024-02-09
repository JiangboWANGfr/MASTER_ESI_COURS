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
}

void task2(void *pdata)
{
    INT8U err;
    while (1)
    {
        OSSemPend(sem, 0, &err); // wait for semaphore
        compteur++;
        printf("TASK 2: compteur = %d\n", compteur);
        OSSemPost(sem); // release semaphore
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

int main(void)
{
    OSInit();
    sem = OSSemCreate(1); // create a semaphore

    OSTaskCreate(task1, NULL, (void *)&task1_stk[TASK_STK_SIZE - 1], TASK1_PRIO);
    OSTaskCreate(task2, NULL, (void *)&task2_stk[TASK_STK_SIZE - 1], TASK2_PRIO);
    OSStart();
}


// 9
#include "includes.h" // 所有必要的包含文件

#define TASK_STK_SIZE 1024
OS_STK TaskStk1[TASK_STK_SIZE];
OS_STK TaskStk2[TASK_STK_SIZE];
OS_EVENT *sem1;
OS_EVENT *sem2;

void Task11(void *pdata)
{
    INT8U err;
    INT8U i = 1;
    i = 0;
    printf("T1A ");

    for (;;)
    {
        OSSemPend(sem1, 0, &err);
        printf("T1B ");
        printf("T1C i=%d \n", i);
        if (i != 0)
        {
            OSSemPost(sem2);
            i--;
        }
    }
}

void Task22(void *pdata)
{
    INT8U err;
    printf("T2A ");
    for (;;)
    {
        printf("T2B ");
        OSSemPend(sem2, 0, &err);
        printf("T2C ");
        OSSemPost(sem1);
    }
}
int main(void)
{
    OSInit();     /* Initialize uC/OS-II                                  */

    sem1 = OSSemCreate(0);
    sem2 = OSSemCreate(1);

    OSTaskCreate(Task11, (void *)0, &TaskStk1[TASK_STK_SIZE - 1], 11);
    OSTaskCreate(Task22, (void *)0, &TaskStk2[TASK_STK_SIZE - 1], 12);
    // OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 1);

    OSStart();
    return 0;
}