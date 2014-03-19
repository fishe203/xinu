/* send.c - send */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	sendb(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* contents of message		*/
	)
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	procent *prptr;		/* ptr to process' table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];

	if ((prptr->prstate == PR_FREE)
	//|| prptr->prhasmsg 
	){
		restore(mask);
		return SYSERR;
	}
//	struct sentry *semptr;
//	semptr = &semtab[prptr->prbuffsem];
	wait(prptr->prbuffsem);
	writebuff(&prptr->prmsg, msg);

	prptr->prhasmsg = TRUE;		/* indicate message is waiting	*/
	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECVB) {
		ready(pid, RESCHED_YES);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid, RESCHED_YES);
	}
	restore(mask);		/* restore interrupts */
	return OK;
}
