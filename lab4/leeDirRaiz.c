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
#define OFFSET 19


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

	lseek(fd,OFFSET*SIZE_BLOCK,SEEK_SET);
	
	for(i=0;i<224;i++){
		if( read(fd,&entry,sizeof(entry)) <0 )
			perror("No pudo leer imagen de disco");

		if((entry.attributes & 0x8) && (entry.attributes & 0x20)){
			printf("Volume label: [%.8s]\n",entry.filename);
			continue;
		}
	

	/*No aseguramos que no se una entrada de nombre largo y que sea un archivo*/
		if((entry.attributes!= 0x0F) && (entry.attributes & 0x20))
			printf("File [%.8s.%.3s] First cluster [%d] Size [%d]\n",entry.filename, entry.ext,entry.starting_cluster,entry.file_size);

	}
	/*
	printf("Jump code: %x:%x:%x\n",boot.jmp[0],boot.jmp[1],boot.jmp[2]);
	printf("OEM code: %s \n",boot.oem);
	printf("sector_size: %d\n",boot.sector_size);
	printf("sectors_per_cluster:%d\n", boot.sectors_per_cluster);
	printf("reserved_sectors: %d\n",boot.reserved_sectors);
	printf("number_of_fats: %d\n",boot.number_of_fats);
	printf("root_dir_entries:%d\n",boot.root_dir_entries);
	printf("total_sector_short: %d\n",boot.total_sectors_short);
	printf("media_descriptor: %d\n",boot.media_descriptor);
	printf("fat_size_sectors: %d\n",boot.fat_size_sectors);
	printf("sectors_per_track: %d\n",boot.sectors_per_track);
	printf("number_of_heads: %d\n",boot.number_of_heads);
	printf("hidden_sectors: %d\n",boot.hidden_sectors);
	printf("boot_sector_signature: %x\n",boot.boot_sector_signature);
	*/
	close(fd);
	return 0;
}