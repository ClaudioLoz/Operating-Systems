#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
	int fdPadre[2],fdHijo[2];
	pipe(fdPadre);
	pipe(fdHijo);
	 if(!fork()) {
	 	 close(fdPadre[1]);
		 close(fdHijo[0]);
		 char cadena3[]="UNIVERSIDAD";
		 char cadena4[]="CATOLICA";
		 char cadena5[]="PERU";
		
		 write(fdHijo[1],cadena3,sizeof(cadena3));
		 read(fdPadre[0],cadena3,sizeof(cadena3));
		 
		 write(fdHijo[1],cadena4,sizeof(cadena4));

		 
		 printf("%s ",cadena3);
		 printf("%s ",cadena5);
		//*
		 close(fdHijo[1]);
		 close(fdPadre[0]);//*/
		 exit(0);
	 } else {
	 	 close(fdHijo[1]);
		 close(fdPadre[0]);
		 char cadena1[]="PONTIFICIA";
		 char cadena2[]="DEL";
		 printf("%s ",cadena1);

		 read(fdHijo[0],cadena1,sizeof(cadena1));
		 printf("%s ",cadena1);

		 write(fdPadre[1],cadena2,sizeof(cadena2));

		 read(fdHijo[0],cadena1,sizeof(cadena1));
		 printf("%s ",cadena1);
		
		 //wait(NULL);
		 close(fdHijo[0]);
		 close(fdPadre[1]);
		 

	 }
	 exit(0);
}