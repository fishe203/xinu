/* xsh_gen.c - xsh_gen */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wordlist.h>

/*------------------------------------------------------------------------
 * xsh_gen - generate words
 *------------------------------------------------------------------------
 */
shellcmd xsh_gen(int32 pip) {

	int32 i;
	int32 len;
	int32 mylen;
	int32 nwords;
	int32 init;
	char wordlist[5];
	int32 value;
	
	if (pipcount < 1){
		fprintf(stderr, "No pipeline.\n");
		return SYSERR;
	}
	
	
	nwords = 0;
	init = clktime;

	fprintf(stdout,"Start generating\n");

	while(TRUE){
		if (clktime - init > 1){
			kprintf("Number of generated words: %d\n",nwords);
			nwords = 0;
			init = clktime;
		}

		// while (len < 5){		
		len = 0;
		value = rand() % 2048;
		for (i = 0; i < 5; i++){
			wordlist[len] = words[value][i];
			len++;
		}
		nwords+=1;
		// }
	//	fprintf(stderr, "%s\n", wordlist);
		// len = 0;
		mylen = pipwrite(pip, wordlist, 5);
		
		if (SYSERR == mylen){
			fprintf(stderr, "Unable to write into the pipeline.\n");
	 		return SYSERR;
		}
	}
	return 0;
}


