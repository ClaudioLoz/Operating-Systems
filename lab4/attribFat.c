#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SIZE_BLOCK 512

//CLaudio Lozano 20180472
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

int main(int narg, char const *argv[])
{
	FatEntry entry;
	int i,fd;
	//if(narg!= 2){
	//	printf("Usage; %s <name image disc>\n",argv[0]);
	//	exit(1);
	//}
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
	lseek(fd,OFFSET*SIZE_BLOCK,SEEK_SET);
	char nameCEntry[12];
	for(i=0;i<cant;i++){
			if( read(fd,&entry,sizeof(entry)) <0 )
					perror("No pudo leer imagen de disco");
			
			if(narg==2){
				if((entry.attributes & 0x8) && (entry.attributes & 0x20))
					continue;
				

			/*No aseguramos que no se una entrada de nombre largo y que sea un archivo*/
				if((entry.filename[0]!=0xe5)&&(entry.filename[0]!=0)&&(entry.attributes!= 0x0F) && (entry.attributes & 0x20)){
						if((entry.attributes & 0x20))
							printf("A ");
						else printf("  ");
						if((entry.attributes & 0x10))
							printf("D ");
						else printf("  ");
						if((entry.attributes & 0x08))
							printf("V ");
						else printf("  ");
						if((entry.attributes & 0x04))
							printf("S ");
						else printf("  ");
						if((entry.attributes & 0x02))
							printf("H ");
						else printf("  ");
						if((entry.attributes & 0x01))
							printf("R ");
						else printf("  ");

						printf("\t%.8s.%.3s \n",entry.filename, entry.ext);
				}

			}else if(narg==4){
		
				if(strcmp(entry.filename,argv[3])==0){ //no encuentra porque no coindice lo que ingreso con el entry.filename
					
					if(argv[2][0]=='-'){
						if(argv[2][1]=='r')
							entry.attributes&= 0xFE;	
						else if(argv[2][1]=='h')
							entry.attributes&= 0xFD;
						else if(argv[2][1]=='s')
							entry.attributes&= 0xFB;

						else if(argv[2][1]=='v')
							entry.attributes&= 0xF7;
						else if(argv[2][1]=='d')
							entry.attributes&= 0xEF;
						else if(argv[2][1]=='a')
							entry.attributes&= 0xDF;
					} else if (argv[2][0]=='+'){
						if(argv[2][1]=='r'){
							entry.attributes= entry.attributes|0x01;	
							//printf("encontrado\n");
						}
						else if(argv[2][1]=='h')
							entry.attributes|= 0x02;
						else if(argv[2][1]=='s')
							entry.attributes|= 0x04;
						else if(argv[2][1]=='v')
							entry.attributes|= 0x08;
						else if(argv[2][1]=='d')
							entry.attributes|= 0x10;
						else if(argv[2][1]=='a')
							entry.attributes|= 0x20;

					}
				}
			}
		}


	close(fd);
	return 0;
}