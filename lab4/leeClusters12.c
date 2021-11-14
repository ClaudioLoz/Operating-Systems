/**********		leeClusters.c		 *********/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structfat.h"

#define SIZE_BLOCK 512
//#define OFFSET 513//cantidad de sectores


int main(int narg, char const *argv[])
{
	FatEntry entry;
	int i,fd,offset;
	short cluster,aux;
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

	for(i=0;i<cant;i++){
		lseek(fd,OFFSET*SIZE_BLOCK+sizeof(entry)*i,SEEK_SET);
		if( read(fd,&entry,sizeof(entry)) <0 )
			perror("No pudo leer imagen de disco");

		if((entry.attributes & 0x8) && (entry.attributes & 0x20)){
			printf("Volume label: [%.8s]\n",entry.filename);
			continue;
		}
	/*No aseguramos que no se una entrada de  nombre largo y que sea un archivo*/
		if((entry.attributes!= 0x0F) && (entry.attributes & 0x20)){
			printf("File [%.8s.%.3s] First cluster [%d] Size [%d]\n",entry.filename, entry.ext,entry.starting_cluster,entry.file_size);
			cluster=entry.starting_cluster;
			printf("Cadena de clusters: %d ",cluster);
			while(1){
				aux=cluster;
				cluster*=3;
				cluster/=2;
				offset=512+cluster; //512 para irme a la tabla FAT +cluster que estoy buscando
				lseek(fd,offset,SEEK_SET);
				read(fd,&cluster,sizeof(cluster));//en cada entrada se guarda el number del siguiente cluster
				if(aux%2==0){
					cluster=cluster & 0x0FFF;
				}else if(aux%2!=0) cluster>>=4;
				if(cluster== 0x0FFF || cluster ==-1)break;
				printf("%d ", cluster);// el encontrado
			}
			printf("\n");
			// para todos los archivos contenidos break;

		}

	}
	close(fd);
	return 0;
}