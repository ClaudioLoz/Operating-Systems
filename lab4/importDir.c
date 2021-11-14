#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE_BLOCK 512
//#define OFFSET 19

/**********		structfat.h		 *********/
typedef struct{
	unsigned char jmp[3];
	char oem[8];
	unsigned short sector_size;
	unsigned char sectors_per_cluster;
	unsigned short reserved_sectors;// 1 para FAT 12 y FAT 16 , 32 para FAT 32
	unsigned char number_of_fats;
	unsigned short root_dir_entries;//0 para FAT32
	unsigned short total_sectors_short; //if zero, later field is used
	unsigned char media_descriptor;
	unsigned short fat_size_sectors;// number of sectors per FAT : 0 para FAT32 
	unsigned short sectors_per_track;
	unsigned short number_of_heads;
	unsigned short hidden_sectors;
	char boot_code[480];
	unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;


typedef struct{
	unsigned char filename[8];
	unsigned char ext[3];
	unsigned char attributes;
	unsigned char reserved[10];
	unsigned short modify_time;
	unsigned short modify_date;
	unsigned short starting_cluster;
	unsigned int file_size;
}__attribute((packed)) FatEntry;


int main(int argc, char const *argv[])
{
	FatEntry entry;

	int i, fd, offset;
	short cluster;
    
	if(argc != 2){
		printf("Usage: %s <name image disc>\n", argv[0]);
		exit(1);
	}

	if((fd = open(argv[1], O_RDONLY)) < 0)
		perror("No pudo abrir la imagen de disco");
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

	lseek(fd, OFFSET*SIZE_BLOCK, SEEK_SET);
	for(i=0; i < cant; i++){
		if(read(fd, &entry, sizeof(entry)) < 0)
			perror("No pudo leer imagen del disco");

		if((entry.attributes & 0x8) && (entry.attributes & 0x20)) 
			continue;

		
		if((entry.attributes & 0x20)){//se usa el nombre largo y verificamos que sea un archivo
				
				printf("File [%.8s.%.3s] First cluster [%d] Size [%d]\n", //viendo que archivos se tienen para copiar
				entry.filename, entry.ext, entry.starting_cluster, entry.file_size);
				cluster = entry.starting_cluster;//nos ponemos en el primer cluster
				char nombreArch[50];
				sprintf(nombreArch, "%.8s.%.3s", entry.filename, entry.ext);
				int archDirectorio;
			    if ((archDirectorio = open(nombreArch, O_CREAT))) {//verificamos que se abra bien
					perror("No se puede abrir el directorio");
					exit(1);
			    }
				while(1){
					offset = 512 + cluster*2;//vamos por los clusters
					lseek(fd, offset, SEEK_SET);
					read(fd, &cluster, sizeof(cluster));
					if(cluster == -1)break;
					write(archDirectorio, &cluster, sizeof(cluster));
				}
				close(archDirectorio);
				printf("\n");
				break;
			}
			
	} 
	close(fd);
	return 0;
}