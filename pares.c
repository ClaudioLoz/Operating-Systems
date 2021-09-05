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
	 	while(!(n & 1)) {
	 		fprintf(stderr,"%d\n",n);
	 		n = n / 2;
		 }
		 write(STDOUT_FILENO,&n,sizeof(n));
		 if(n==1) exit(0);
	}
	return 0;
}