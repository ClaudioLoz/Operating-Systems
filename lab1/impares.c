#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
int main(int argc, char const *argv[])
{
	int n;
	while(1) {
			 read(STDIN_FILENO,&n,sizeof(n));
			 if(n==1){
				 fprintf(stderr,"%d\n",n);
				 return 0;
			 }
			 else {
				 fprintf(stderr,"%d\n",n);
				 n = 3*n +1;
				 write(STDOUT_FILENO,&n,sizeof(n));
			 }
		 }
	
	return 0;
}

