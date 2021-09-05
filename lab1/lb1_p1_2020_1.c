#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
/* Este programa crea un abanico de procsos. */
/* Ejm 2.6 del libro UNIX Programacion Practica - Kay Robbins */
/* Steve Robbins */
#define N 8
int main(void)
{ int i,status;
 pid_t child,pid_padre;
 
 pid_t arrPids[N]; // N porque se considera al padre+los hijos menos el último

 pid_padre=getpid();

 arrPids[0]=pid_padre;
 for(i=0;i<N; ++i){
 	if((child=fork())<=0) {
 		if(i==N-1){
 			system("ps -o pid,ppid,cmd");
 			char comando[30];
 			for(i=0;i<N;i++){
 				sprintf(comando,"kill -9 %d",arrPids[i]);
 				system(comando);
 			}
 			system("ps -o pid,ppid,cmd");
 		}else{
 			sleep(1);
 		}
 		break;
 	}else{
 		arrPids[i+1]=child;
 	}

 } 
 if(pid_padre==getpid()) for(i=0;i<N;++i) wait(&status);
 return 0;
}
