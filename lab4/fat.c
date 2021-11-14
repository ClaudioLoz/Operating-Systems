
/*********************************** FAT32.c ******************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "structfat.h" /* struct of root entry */
#include "structfat32.h" /* struct of FAT32 */

int main( int narg, char*argv[]){

	FatEntry entry;
	Fat32BootSector boot;
	int fd, offset, i;
	int OFFSET;
	int cluster;

	/* validations */

	if(narg != 2){
		printf("Usage: %s <name image disc>\n",argv[0]);
		exit(1);
	}

	if((fd = open(argv[1],O_RDONLY)) < 0)
		perror("No pudo abrir imagen de disco");

	/* first, read the boot sector of FAT32 */

	if(read(fd,&boot,sizeof(boot)) < 0)
		perror("No pudo leer imagen de disco");

	/* lets move first to first cluster */

	cluster = boot.first_cluster;

	OFFSET = boot.reserved_sectors + boot.number_of_fats*boot.sectors_per_fat + (boot.first_cluster-2)*boot.sectors_per_cluster;
	// 32 (including Boot) + 2 (FATs) * 8005 + (2 - 2) * 8 
	// al in terms of number of sectors because space is divided into sectors

	OFFSET = lseek(fd,OFFSET*boot.sector_size,SEEK_SET);

	int number_of_root_dir_entries_per_cluster = boot.sectors_per_cluster*boot.sector_size/sizeof(entry);

	//printf("OFFSET: %d\n",OFFSET);

	for(i=0; i < number_of_root_dir_entries_per_cluster; i++) {

		lseek(fd,OFFSET,SEEK_SET);

		//printf("OFFSET: %d\n",OFFSET);

		if(read(fd,&entry,sizeof(entry))<0)
			perror("No pudo leer imagen de disco");

		OFFSET += sizeof(entry);

		if((entry.attributes & 0x8) && (entry.attributes & 0x20)){
			printf("Volume label: [%.8s]\n", entry.filename );
			continue;
		}

		if(entry.filename[0]!=0 ){
			printf("File [%.8s.%.3s] First cluster [%d] Size [%d]\n",
				entry.filename,entry.ext,entry.starting_cluster,entry.file_size);


			cluster = entry.starting_cluster;

			printf("Chain of clusters: ");

			while (1) {
				offset = boot.reserved_sectors*boot.sector_size + cluster*4; // each cluster have size of 4 bytes
				printf("C%d ",cluster);
				lseek(fd,offset,SEEK_SET);
				read(fd,&cluster,sizeof(cluster));
				if (cluster == 0x0FFFFFF8 || cluster == 0x0FFFFFFF) break; // EOF
			}
			printf("\n");
		}

	}
		
	close(fd);
	return 0;
}