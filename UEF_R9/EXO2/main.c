/*
*********************************************************************************************************
*                                            EXAMPLE CODE  EXO2
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
*                                             EXAMPLE CODE  EXO2
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

*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define TASK_STK_SIZE 512u /* Size of each task's stacks (# of WORDs)       */

#define TASK_START_ID 0 /* Application tasks IDs                         */
#define TASK_CLK_ID 1
#define TASK_1_ID 2
#define TASK_2_ID 3
#define TASK_3_ID 4
#define TASK_4_ID 5
#define TASK_5_ID 6

#define TASK_START_PRIO 20u /* Application tasks priorities                  */
#define TASK_CLK_PRIO 21u
#define TASK_1_PRIO 22u
#define TASK_2_PRIO 23u
#define TASK_3_PRIO 24u
#define TASK_4_PRIO 25u
#define TASK_5_PRIO 26u

/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

OS_STK TaskStartStk[TASK_STK_SIZE]; /* Startup    task stack                         */
OS_STK TaskClkStk[TASK_STK_SIZE];   /* Clock      task stack                         */
OS_STK Task1Stk[TASK_STK_SIZE];     /* Task #1    task stack                         */
OS_STK Task2Stk[TASK_STK_SIZE];     /* Task #2    task stack                         */
OS_STK Task3Stk[TASK_STK_SIZE];     /* Task #3    task stack                         */
OS_STK Task4Stk[TASK_STK_SIZE];     /* Task #4    task stack                         */
OS_STK Task5Stk[TASK_STK_SIZE];     /* Task #5    task stack                         */

/* Message mailboxes for Tasks #4 and #5         */

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void TaskStart(void *data); /* Function prototypes of tasks                  */
static void TaskStartCreateTasks(void);
static void TaskStartDispInit(void);
static void TaskStartDisp(void);
void TaskClk(void *data);
void Task1(void *data);
void Task2(void *data);
void Task3(void *data);
void Task4(void *data);
void Task5(void *data);
/*
******************************************************************************************
* VARIABLES
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
int main(void)
{
    OS_STK *ptos;
    OS_STK *pbos;
    INT32U size;

    APP_TRACE_DBG(("                Master 2 ESI 2020/2021 RTOS uCOS II           \n\n\n\r"));
#if OS_TASK_NAME_EN > 0u
    CPU_INT08U os_err;
#endif
    CPU_IntInit();
    Mem_Init();   /* Initialize Memory Managment Module                   */
    CPU_IntDis(); /* Disable all Interrupts                               */
    CPU_Init();   /* Initialize the uC/CPU services                       */
    OSInit();     /* Initialize uC/OS-II                                  */

    ptos = &TaskStartStk[TASK_STK_SIZE - 1u]; /* TaskStart() will use Floating-Point      */
    pbos = &TaskStartStk[0u];
    size = TASK_STK_SIZE;
    // OSTaskStkInit_FPE_x86(&ptos, &pbos, &size);
    OSTaskCreateExt(TaskStart,
                    (void *)0,
                    ptos,
                    TASK_START_PRIO,
                    TASK_START_ID,
                    pbos,
                    size,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    /*
        OSTaskCreateExt( StartupTask,                                                   0,
                        &StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE - 1u],
                         APP_CFG_STARTUP_TASK_PRIO,
                         APP_CFG_STARTUP_TASK_PRIO,
                        &StartupTaskStk[0u],
                         APP_CFG_STARTUP_TASK_STK_SIZE,
                         0u,
                        (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

    */

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
        OSTimeDlyHMSM(0u, 0u, 10u, 0u);
        APP_TRACE_DBG(("Time: %d\n\r", OSTimeGet()));
    }
}

/*
*********************************************************************************************************
*                                               STARTUP TASK
*********************************************************************************************************
*/

OS_EVENT *sendMbox;
OS_EVENT *ackMbox;
void TaskStart(void *pdata)
{
#if OS_CRITICAL_METHOD == 3 /* Allocate storage for CPU status register */
    OS_CPU_SR cpu_sr;
#endif
    INT16S key;

    pdata = pdata; /* Prevent compiler warning                 */

    TaskStartDispInit(); /* Setup the display                        */

    OSStatInit(); /* Initialize uC/OS-II's statistics         */

    /* Create 2 message mailboxes               */
    sendMbox = OSMboxCreate((void *)0);
    ackMbox = OSMboxCreate((void *)0);

    TaskStartCreateTasks(); /* Create all other tasks                   */

    for (;;)
    {
        TaskStartDisp(); /* Update the display                       */

        if (PC_GetKey(&key))
        { /* See if key has been pressed              */
            if (key == 0x1B)
            {                   /* Yes, see if it's the ESCAPE key          */
                PC_DOSReturn(); /* Yes, return to DOS                       */
            }
        }

        OSCtxSwCtr = 0; /* Clear context switch counter             */
                        // OSTimeDly(OS_TICKS_PER_SEC);                       /* Wait one second                          */
        OSTimeDlyHMSM(0u, 0u, 1u, 0u);
    }
}
/*
*********************************************************************************************************
*                                             CREATE TASKS
*********************************************************************************************************
*/

static void TaskStartCreateTasks(void)
{
    OSTaskCreateExt(TaskClk,
                    (void *)0,
                    &TaskClkStk[TASK_STK_SIZE - 1u],
                    TASK_CLK_PRIO,
                    TASK_CLK_ID,
                    &TaskClkStk[0u],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskCreateExt(Task1,
                    (void *)0,
                    &Task1Stk[TASK_STK_SIZE - 1u],
                    TASK_1_PRIO,
                    TASK_1_ID,
                    &Task1Stk[0u],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskCreateExt(Task2,
                    (void *)0,
                    &Task2Stk[TASK_STK_SIZE - 1u],
                    TASK_2_PRIO,
                    TASK_2_ID,
                    &Task2Stk[0u],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskCreateExt(Task3,
                    (void *)0,
                    &Task3Stk[TASK_STK_SIZE - 1u],
                    TASK_3_PRIO,
                    TASK_3_ID,
                    &Task3Stk[0u],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskCreateExt(Task4,
                    (void *)0,
                    &Task4Stk[TASK_STK_SIZE - 1u],
                    TASK_4_PRIO,
                    TASK_4_ID,
                    &Task4Stk[0u],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskCreateExt(Task5,
                    (void *)0,
                    &Task5Stk[TASK_STK_SIZE - 1u],
                    TASK_5_PRIO,
                    TASK_5_ID,
                    &Task5Stk[0u],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                               TASK #1
*
* Description: This task executes every 100 mS and measures the time it task to perform stack checking
*              for each of the 5 application tasks.  Also, this task displays the statistics related to
*              each task's stack usage.
*********************************************************************************************************
*/

void Task1(void *pdata)
{
    INT8U err;
    OS_STK_DATA data; /* Storage for task stack data                             */
    INT16U time = 0;  /* Execution time (in uS)                                  */
    INT8U i;
    char s[80];

    pdata = pdata;

    for (;;)
    {
        for (i = 0; i < 7; i++)
        {

            // Add here the code to measure the time to check the stack size

            err = OSTaskStkChk(TASK_START_PRIO + i, &data);

            if (err == OS_ERR_NONE)
            {
                printf("%4ld        %4ld        %4ld        %6d\n",
                       data.OSFree + data.OSUsed,
                       data.OSFree,
                       data.OSUsed,
                       time);
                // PC_DispStr(19, 12 + i, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
            }
        }

        OSTimeDlyHMSM(0u, 0u, 2u, 0u); // Delay for 2 S
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               TASK #2
*
* Description: This task displays a clockwise rotating wheel on the screen.
*********************************************************************************************************
*/

void Task2(void *data)
{
    data = data;
    for (;;)
    {
        // PC_DispChar(70, 15, '|',  DISP_FGND_YELLOW + DISP_BGND_BLUE);
        printf("task2: |\n");
        // PC_DispChar(70, 15, '\\', DISP_FGND_YELLOW + DISP_BGND_BLUE);
        OSTimeDlyHMSM(0u, 0u, 4u, 0u);
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               TASK #3
*
* Description: This task displays a counter-clockwise rotating wheel on the screen.
*
* Note(s)    : I allocated 500 bytes of storage on the stack to artificially 'eat' up stack space.
*********************************************************************************************************
*/

void Task3(void *data)
{
    char dummy[500];
    INT16U i;

    data = data;
    for (i = 0; i < 500; i++)
    { /* Use up the stack with 'junk'                                 */
        dummy[i] = '?';
    }
    for (;;)
    {
        // PC_DispChar(70, 15, '|',  DISP_FGND_YELLOW + DISP_BGND_BLUE);
        printf("task3: |\n");
        for (i = 0; i < 499; i++)
        { /* Use up the stack with 'junk'                                 */
            dummy[i] = '?';
        }
        // PC_DispChar(70, 15, '\\', DISP_FGND_YELLOW + DISP_BGND_BLUE);
        OSTimeDlyHMSM(0u, 0u, 5u, 0u);
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               TASK #4
*
* Description: This task sends a message to Task #5.  The message consist of a character that needs to
*              be displayed by Task #5.  This task then waits for an acknowledgement from Task #5
*              indicating that the message has been displayed.
*********************************************************************************************************
*/
/*
为此，我们需要创建2个邮箱，允许在数据上包含一个（且只有一个/指针。此数据是特定于应用程序的，必须由发送方和接收方共同定义。

*/

void Task4(void *data)
{

    INT8U err;

    // data = data;
    INT8U i = 0;
    while (1)
    {

        /* Send message to Task #5                            */
        char *msg = "A" + i++;
        OSMboxPost(sendMbox, (void *)msg);
        printf("Task4: %s\n", msg);
        /* Wait for acknowledgement from Task #5              */
        char *ack = (char *)OSMboxPend(ackMbox, 0, &err);
        printf("Task4: ack\n");
        /* Next message to send                               */
        if (i == 26)
            i = 0;
        OSTimeDlyHMSM(0u, 0u, 4u, 0u);
    }
    /* Start new series of messages                       */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               TASK #5
*
* Description: This task displays messages sent by Task #4.  When the message is displayed, Task #5
*              acknowledges Task #4.
*********************************************************************************************************
*/

void Task5(void *data)
{

    INT8U err;

    // data = data;
    while (1)
    {

        /* Wait for message from Task #4 */
        char *msg = (char *)OSMboxPend(sendMbox, 0, &err);
        printf("Task5: %s\n", msg);
        /* Display message               */
        /* Acknowledge reception to Task #4 */
        OSMboxPost(ackMbox, (void *)1);
        printf("Task5: ack\n");
        OSTimeDlyHMSM(0u, 0u, 4u, 0u);
    }
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               CLOCK TASK
*********************************************************************************************************
*/

void TaskClk(void *data)
{
    char s[40];

    data = data;
    for (;;)
    {
        PC_GetDateTime(s);
        // PC_DispStr(60, 23, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
        printf("Taskclk__time: %s\n", s);
        // OSTimeDly(OS_TICKS_PER_SEC);
        OSTimeDlyHMSM(0u, 0u, 5u, 0u);
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
    printf("                                     EXAMPLE #2                                 \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("TasksInrunning:             CPU Usage:             TaskchangePerSecond:         \n");
    printf("<-PRESS 'ESC' TO QUIT->                                                         \n");
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
    //     printf(" %5d\n", OSTaskCtr);
    // #if OS_TASK_STAT_EN > 0
    //     printf("%3d\n", OSCPUUsage); /* Display CPU usage in %               */
    // #endif

    //     printf("%5d\n", OSCtxSwCtr);                                 /* Display #context switches per second */
    //     printf("V%1d.%02d\n", OSVersion() / 100, OSVersion() % 100); /* Display uC/OS-II's version number    */
    printf("Tasks running: %5d, CPU Usage: %3d, Context switches per second: %5d, uC/OS-II's version number: V%1d.%02d\n", OSTaskCtr, OSCPUUsage, OSCtxSwCtr, OSVersion() / 100, OSVersion() % 100);

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
