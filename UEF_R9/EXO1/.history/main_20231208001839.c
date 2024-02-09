/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*
*                                              uC/OS-II
*                                             EXAMPLE CODE
*
* Filename : main.c
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "cpu.h"
#include "lib_mem.h"
#include "os.h"
#include "includes.h"
#include "app_cfg.h"
#include "pc.h";
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
/*
****************************************************************************************
* CONSTANTS
******************************************************************************************
*/
#define TASK_STK_SIZE 512 /* Size of each task's stacks (# of WORDs) */
#define N_TASKS 10
/*
******************************************************************************************
* VARIABLES
******************************************************************************************
*/
OS_STK TaskStk[N_TASKS][TASK_STK_SIZE]; /* Tasks stacks */
OS_STK TaskStartStk[TASK_STK_SIZE];
char TaskData[N_TASKS]; /* Parameters to pass to each task */
OS_EVENT *RandomSem;
/*
******************************************************************************************
* FUNCTION PROTOTYPES
******************************************************************************************
*/
void Task(void *data); /* Function prototypes of tasks */
void Task2(void *pdata);
void TaskStart(void *data); /* Function prototypes of Startup task */
static void TaskStartCreateTasks(void);
static void TaskStartDispInit(void);
static void TaskStartDisp(void);
/*
******************************************************************************************
* MAIN
******************************************************************************************
*/
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
static OS_STK StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static void StartupTask(void *p_arg);
/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : none
*********************************************************************************************************
*/
#include "includes.h" // 所有必要的包含文件

#define TASK_STK_SIZE 1024
OS_STK TaskStk1[TASK_STK_SIZE];
OS_STK TaskStk2[TASK_STK_SIZE];
OS_EVENT *sem1;
OS_EVENT *sem2;

void Task1(void *pdata);
void Task2(void *pdata);

int main(void)
{
    sem1 = OSSemCreate(0);
    sem2 = OSSemCreate(1);

    OSTaskCreate(Task11, (void *)0, &TaskStk1[TASK_STK_SIZE - 1], 11);
    OSTaskCreate(Task12, (void *)0, &TaskStk2[TASK_STK_SIZE - 1], 12);

    OSStart();
    return 0;
}

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
        printf("T1C i=%d\n", i);
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

int main1(void)
{
    APP_TRACE_DBG(("                Master 2 ESI 2020/2021 RTOS uCOS II           \n\n\n\r"));
#if OS_TASK_NAME_EN > 0u
    CPU_INT08U os_err;
#endif
    CPU_IntInit();
    Mem_Init();   /* Initialize Memory Managment Module                   */
    CPU_IntDis(); /* Disable all Interrupts                               */
    CPU_Init();   /* Initialize the uC/CPU services                       */
    OSInit();     /* Initialize uC/OS-II                                  */

    OSTaskCreateExt(StartupTask, 0,
                    &StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE - 1u],
                    APP_CFG_STARTUP_TASK_PRIO,
                    APP_CFG_STARTUP_TASK_PRIO,
                    &StartupTaskStk[0u],
                    APP_CFG_STARTUP_TASK_STK_SIZE,
                    0u,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    RandomSem = OSSemCreate(1); /* */
    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 1);

#if OS_TASK_NAME_EN > 0u
    OSTaskNameSet(APP_CFG_STARTUP_TASK_PRIO,
                  (INT8U *)"Startup Task",
                  &os_err);
#endif
    OSStart(); /* Start multitasking (i.e. give control to uC/OS-II)   */

    while (DEF_ON)
    { /* Should Never Get Here.                               */
        ;
    }
}

static void StartupTask(void *p_arg)
{
    (void)p_arg;

    OS_TRACE_INIT(); /* Initialize the uC/OS-II Trace recorder               */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err); /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    APP_TRACE_DBG(("uCOS-II is Running...\n\r"));

    while (DEF_TRUE)
    { /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(0u, 0u, 1u, 0u);
        APP_TRACE_DBG(("Time: %d\n\r", OSTimeGet()));
    }
}

/*
******************************************************************************************
* STARTUP TASK
******************************************************************************************
*/
void TaskStart(void *pdata)
{
#if OS_CRITICAL_METHOD == 3 /* Allocate storage for CPU status register */
    OS_CPU_SR cpu_sr;
#endif
    char s[100];
    INT16S key;
    pdata = pdata; /* Prevent compiler warning */

    TaskStartDispInit();    /* Initialize the display */
    OSStatInit();           /* Initialize uC/OSII's statistics */
    TaskStartCreateTasks(); /* */

    for (;;)
    {
        TaskStartDisp(); /* Update the display */
        if (PC_GetKey(&key) == 1)
        { /* See if key has been pressed */
            if (key == 0x1B)
            {                   /* Yes, see if it's the ESCAPE key */
                PC_DOSReturn(); /* */
            }
        }
        OSCtxSwCtr = 0;                /* Clear context switch counter */
        OSTimeDlyHMSM(0u, 0u, 3u, 0u); /* */
    }
}
/*
***************************************************************************************
* CREATE TASKS
*****************************************************************************************
*/
static void TaskStartCreateTasks(void)
{
    INT8U i;
    for (i = 0; i < N_TASKS; i++)
    {                          /* Create N_TASKS identical tasks */
        TaskData[i] = '0' + i; /* Each task will display its own letter */
        printf("TaskData[%d]=%c\t", i, TaskData[i]);
        OSTaskCreate(Task2, (void *)&TaskData[i], &TaskStk[i][TASK_STK_SIZE - 1],
                     i + 5);
    }
}
/*
******************************************************************************************
* TASKS
******************************************************************************************
*/
void Task(void *pdata)
{
    INT8U x;
    INT8U y;
    INT8U err;
    for (;;)
    {
        OSSemPend(RandomSem, 0, &err); /* Acquire semaphore to perform random numbers */
        x = rand() % 80;               /* Find X position where task number will appear */
        y = rand() % 16;               /* Find Y position where task number will appear */
        OSSemPost(RandomSem);          /* Release semaphore */

        APP_TRACE_DBG(("%c\n\r", *(char *)pdata));
        OSTimeDlyHMSM(0u, 0u, 5u, 0u); /* */
    }
}

// modifiez la fonction task so the waiting time can be given as a parameter

void Task2(void *pdata)
{
    INT8U x;
    INT8U y;
    INT8U err;
    // INT8U waiting_time = (INT8U)pdata;
    for (;;)
    {
        OSSemPend(RandomSem, 0, &err); /* Acquire semaphore to perform random numbers */
        x = rand() % 80;               /* Find X position where task number will appear */
        y = rand() % 16;               /* Find Y position where task number will appear */
        OSSemPost(RandomSem);          /* Release semaphore */

        APP_TRACE_DBG(("%d\n\r", *(INT8U *)pdata - 47));
        OSTimeDlyHMSM(0u, 0u, (*(INT8U *)pdata - 47), 0u); /* */
    }
}

static void TaskStartDispInit(void)
{
    /*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
    /*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */

    printf("                         uC/OS-II, The Real-Time Kernel                         \n");
    printf("                                 Jean J. Labrosse                               \n");
    printf("           TP RTOS_MASTER ESI 2021/2022 - CY Cergy Paris University             \n");
    printf("                                 18 November 2021                               \n");
    printf("                                     EXAMPLE #1                                 \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("#Tasks          :        CPU Usage:     %                                       \n");
    printf("#Task switch/sec:                                                               \n");
    printf("                            <-PRESS 'ESC' TO QUIT->                             \n");
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                           UPDATE THE DISPLAY
*********************************************************************************************************
*/

static void TaskStartDisp(void)
{
    char s[80];
    int a = 0;
    /* Display #tasks running               */
    printf(" %5d\n", OSTaskCtr);
#if OS_TASK_STAT_EN > 0
    printf("%3d\n", OSCPUUsage); /* Display CPU usage in %               */
#endif

    printf("%5d\n", OSCtxSwCtr);                                 /* Display #context switches per second */
    printf("V%1d.%02d\n", OSVersion() / 100, OSVersion() % 100); /* Display uC/OS-II's version number    */
    switch (a)
    { /* Display whether FPU present          */
    case 0:
        // PC_DispStr(71, 22, " NO  FPU ", DISP_FGND_YELLOW + DISP_BGND_BLUE);
        break;

    case 1:
        // PC_DispStr(71, 22, " 8087 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
        break;

    case 2:
        // PC_DispStr(71, 22, "80287 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
        break;

    case 3:
        // PC_DispStr(71, 22, "80387 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
        break;
    }
}
