/*
 *
 * FBL ENGINE
 *
 * tp.c
 *
 * Contains functions for the threadpool.
 *
 * Hans Strömquist 2017 - 2022
 *
 */

#include "engine.h"

#define THREAD_NUM 4
#define QUEUE_SIZE 256

SDL_Thread* fbl_thread[THREAD_NUM];

FBL_TP_TASK fbl_task_queue[QUEUE_SIZE];

int task_count = 0;

SDL_mutex *mutex_queue; // mutex for the queue
SDL_cond *cond_queue;

bool fbl_multithreaded = false;


void execute_task(FBL_TP_TASK* task) {

    //task->task_func(task->arg1, task->arg2);
    task->task_func();

}

void submit_task(FBL_TP_TASK task) {

    if (task_count < QUEUE_SIZE) {

        SDL_LockMutex(mutex_queue);
    
        fbl_task_queue[task_count] = task;
        task_count++;
    
        SDL_UnlockMutex(mutex_queue);
        SDL_CondSignal(cond_queue);

    }

}

static int start_thread(void* args) {

    int i;

    while (1) {
    
        FBL_TP_TASK task;

        SDL_LockMutex(mutex_queue);
        
        while (task_count == 0) {
            SDL_CondWait(cond_queue, mutex_queue);
        }

        task = fbl_task_queue[0];

        for (i = 0; i < task_count - 1; i++) {
            fbl_task_queue[i] = fbl_task_queue[i + 1];
        }

        task_count--;

        SDL_UnlockMutex(mutex_queue);
        execute_task(&task);

        //if(rand() % 100 == 1) printf("task_count: %d\n", task_count);

    }

    return 0;

}

int fbl_create_threadpool() {

    int i;

    if (!fbl_multithreaded)
    {

        mutex_queue = SDL_CreateMutex();
        cond_queue = SDL_CreateCond();

        for (i = 0; i < THREAD_NUM; i++) {

            fbl_thread[i] = SDL_CreateThread(start_thread, "start_thread", (void*)NULL);

            if (fbl_thread[i] == NULL) {
                fprintf(FBL_ERROR_OUT, "Failed to create the thread.\n");
                return -1;
            }

        }

        /*
        for (i = 0; i < 200; i++) {

            TP_TASK t = {
                .taskFunction = i % 2 == 0 ? &sum : &product,
                .arg1 = rand() % 100,
                .arg2 = rand() % 100
            };

            submitTask(t);

        }
        */

        fbl_multithreaded = true;

    }

    return 0;
}

void fbl_submit_task_tp(void (*func)())
{

    if (fbl_multithreaded) {

        FBL_TP_TASK t;

        t.task_func = func;  // assign function

        submit_task(t);  // submit to the queue

    }
    else func();    // just call the function normally if not multithreaded

}

void fbl_destroy_threadpool()
{

    int i;

    if (fbl_multithreaded)
    {

        for (i = 0; i < THREAD_NUM; i++) {
            //SDL_WaitThread(th[i], NULL);
            SDL_DetachThread(fbl_thread[i]);
        }

        SDL_DestroyCond(cond_queue);
        SDL_DestroyMutex(mutex_queue);

        fbl_multithreaded = false;

    }

}

/* end of tp.c */
