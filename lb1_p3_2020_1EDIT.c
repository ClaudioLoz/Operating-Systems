#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
int main(void) {
	 int n;
	 int fdpares[2], fdimpares[2];
	 srand(time(NULL));
	 n = (rand() % 30)+1;
	 pipe(fdpares);
	 pipe(fdimpares);
	 if(n & 1) write(fdimpares[1],&n,sizeof(n));
	 else write(fdpares[1],&n,sizeof(n));
	 if(!fork()) { /****** Inicio de hijo 1 ****/
	 	close(fdpares[1]);
	 	close(fdimpares[0]);
	 	dup2(fdpares[0],STDIN_FILENO);
	 	close(fdpares[0]);
	 	dup2(fdimpares[1],STDOUT_FILENO);
	 	close(fdimpares[1]);
		execl("/home/linuxmint/Documents/20180472/pares","/home/linuxmint/Documents/20180472/pares",NULL);
	} /******* Fin de hijo 1 *****/
	 if(!fork()) { /****** Inicio de hijo 2 ****/
	 	close(fdpares[0]);
	 	close(fdimpares[1]);
	 	dup2(fdimpares[0],STDIN_FILENO);
	 	close(fdimpares[0]);
	 	dup2(fdpares[1],STDOUT_FILENO);
	 	close(fdpares[1]);
		execl("/home/linuxmint/Documents/20180472/impares","/home/linuxmint/Documents/20180472/impares",NULL);
		 
	 } /******* Fin de hijo 2 *****/
	 wait(NULL);
	 wait(NULL);
 	return 0;
} 