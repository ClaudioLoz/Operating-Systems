/**********		leeDirRaiz.c		 *********/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structfat.h"

#define SIZE_BLOCK 512



int main(int narg, char const *argv[])
{
	FatEntry entry;

	int i,fd;
	if(narg!= 2){
		printf("Usage; %s <name image disc>\n",argv[0]);
		exit(1);
	}
	if((fd=open(argv[1],O_RDONLY)) <0 )
		perror("No pudo abrir imagen de disco");

	//tengo que leer desde el boot sector cuantos sectores tiene cada FAT
	lseek(fd,22,SEEK_SET);
	unsigned short OFFSET;
	if( read(fd,&OFFSET,sizeof(OFFSET)) <0 )
			perror("No pudo leer imagen de disco");
	
	lseek(fd,17,SEEK_SET);//tengo que leer la cantidad de directory entries que tenga el fat

	unsigned short cant;
	if( read(fd,&cant,sizeof(cant)) <0 )
			perror("No pudo leer imagen de disco");

	OFFSET=OFFSET*2+1;//cantidad total de sectores para llegar a las directory entries ya sea de FAT12 O FAT16
	lseek(fd,OFFSET*SIZE_BLOCK,SEEK_SET);//voy a empezar a leer directory entries
	for(i=0;i<cant;i++){ // number de entradas
		if( read(fd,&entry,sizeof(entry)) <0 )// se va leyendo entrada por entrada(esta puede apuntar a un archivo o un subdirectorio)
			perror("No pudo leer imagen de disco");
		if((entry.attributes & 0x8) && (entry.attributes & 0x20)){ // verifica primero el bit 3 de attribute para ver si coincide el volume label y luego el bit 5 para ver si 
			printf("Volume label: [%.8s]\n",entry.filename);
			continue;
		}

	/*No aseguramos que no se una entrada de nombre largo y que sea un archivo*/
		if((entry.attributes!= 0x0F) && (entry.attributes & 0x20))
			printf("File [%.8s.%.3s] First cluster [%d] Size [%d]\n",entry.filename, entry.ext,entry.starting_cluster,entry.file_size);
	}
	close(fd);
	return 0;
}