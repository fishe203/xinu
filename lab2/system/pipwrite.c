/* pipwrite.c -pipwrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  pipwrite
 *------------------------------------------------------------------------
 */
int32	pipwrite(pipid32 pipid, char *buf, uint32 len)
{
	intmask	mask;			/* saved interrupt mask		*/
	int32 count;
	//int32 temp;
	struct pipentry *piptr;
	mask = disable();
	if (isbadpip(pipid)
		|| len < 0){
		restore(mask);
		return SYSERR;
	}
	
	piptr = &piptab[pipid];

	if (piptr->pstate != PIPE_CONNECTED){
		restore(mask);
		return SYSERR;
	}

	count = 0;
	// int32 start;
	// wait(mutex);
//	count = piptab[pipid].buffcount;
	// signal(mutex);
	while (count < len){	
		wait(sem_empty);
//		wait(mutex);
	//	temp = count % PIPE_SIZE;
		writebuff(piptr->pipbuffer,buf[count]);
		count++;
//		signal(mutex);
		signal(sem_full);
	}

	restore(mask);
	return count;
}
