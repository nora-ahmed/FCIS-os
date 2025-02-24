// User-level Semaphore

#include "inc/lib.h"

struct semaphore create_semaphore(char *semaphoreName, uint32 value) {
//TODO: [PROJECT'24.MS3 - #02] [2] USER-LEVEL SEMAPHORE - create_semaphore
//COMMENT THE FOLLOWING LINE BEFORE START CODING
//panic("create_semaphore is not implemented yet");
//Your Code is Here...
//	    sem.semdata = (struct __semdata*)sys_createSharedObject(semaphoreName,sizeof(struct __semdata),1,(void*)0);
//	    sem.semdata->count = value;
//	    sem.semdata->lock = 0;
//	    LIST_INIT(&sem.semdata->queue);
//	    return sem;
	    struct semaphore semasemo;
	    semasemo.semdata = (struct __semdata*)smalloc(semaphoreName,sizeof(struct __semdata),1);
	    if (semasemo.semdata == NULL)
	    {
	        panic("NO MORE FORTNITE");
	    }
	    strcpy(semasemo.semdata->name, semaphoreName);
	    semasemo.semdata->count = value;
	    semasemo.semdata->lock = 0;
	    LIST_INIT(&semasemo.semdata->queue);
	    return semasemo;
}

struct semaphore get_semaphore(int32 ownerEnvID, char* semaphoreName)
{
	//TODO: [PROJECT'24.MS3 - #03] [2] USER-LEVEL SEMAPHORE - get_semaphore
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("get_semaphore is not implemented yet");
	//Your Code is Here...
	struct semaphore semrus;
	semrus.semdata = (struct __semdata*)sget(ownerEnvID, semaphoreName);
	    if (semrus.semdata == NULL)
	    {
	        panic("NO MORE CARDS");
	    }
	    return semrus;
}

void wait_semaphore(struct semaphore sem)
{
	//TODO: [PROJECT'24.MS3 - #04] [2] USER-LEVEL SEMAPHORE - wait_semaphore
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("wait_semaphore is not implemented yet");
	//Your Code is Here...
	//cprintf("hi\n");
	// sys_pushcli();
   // struct Env* current_env = sys_get_current_env();
	while (xchg(&(sem.semdata->lock), 1) != 0);
    //cprintf("im locked\n");
    sem.semdata->count--;
    //cprintf(" counte --\n");
    if (sem.semdata->count < 0) {
    	//cprintf("im less than 0\n");
            sys_enqueue(&(sem.semdata->queue),&sem);
            //cprintf("enqueued\n");
             //sys_sched();
    }
    else {
            sem.semdata->lock = 0;
        }
    //cprintf(" done\n");
   // sys_popcli();
}
void signal_semaphore(struct semaphore sem) {
	//[PROJECT'24.MS3]
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("signal_semaphore is not implemented yet");
	//Your Code is Here...
    while (xchg(&(sem.semdata->lock), 1) != 0);
    sem.semdata->count++;
    if (sem.semdata->count <= 0) {
        sys_dequeue(&(sem.semdata->queue));
    }
    sem.semdata->lock = 0;
}
int semaphore_count(struct semaphore sem)
{
	return sem.semdata->count;
}
