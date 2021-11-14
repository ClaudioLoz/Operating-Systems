
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
	int OFFSET, number_of_clusters = 0;
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

	/* we know that FAT has one entry per cluster, so lets calculate the number of clusters (equals to number of entries) */

	while (1) {

		/* calculate offset to each cluster (entry) */

		offset = boot.reserved_sectors*boot.sector_size + cluster*4; // each cluster have size of 4 bytes
		// 32 * 512 + 2*4

		lseek(fd,offset,SEEK_SET);
		read(fd,&cluster,sizeof(cluster));
		number_of_clusters++;
		if (cluster == 0x0FFFFFF8 || cluster == 0x0FFFFFFF) break; // EOF
	}

	printf("Number of clusters: %d\n",number_of_clusters);

	/* calculate the offset to root directory */

	OFFSET = boot.reserved_sectors + boot.number_of_fats*boot.sectors_per_FAT + (boot.first_cluster_root_directory-2)*boot.sectors_per_cluster;
	// 32 (including Boot) + 2 (FATs) * 8005 + (2 - 2) * 8 
	// al in terms of number of sectors because space is divided into sectors

	lseek(fd,OFFSET*boot.sector_size,SEEK_SET);

	int number_of_root_dir_entries_per_cluster = boot.sectors_per_cluster*boot.sector_size/sizeof(entry);

	for(i=0; i < number_of_clusters*number_of_root_dir_entries_per_cluster; i++) {


		if(read(fd,&entry,sizeof(entry))<0)
			perror("No pudo leer imagen de disco");

		if((entry.attributes & 0x8) && (entry.attributes & 0x20)){
			printf("Volume label: [%.8s]\n", entry.filename );
			continue;
		}

		if(entry.attributes & 0x20){
			printf("File [%.8s.%.3s] First cluster [%d] Size [%d]\n",
				entry.filename,entry.ext,entry.starting_cluster,entry.file_size);
		
			
		}
	}
	close(fd);
	return 0;
}