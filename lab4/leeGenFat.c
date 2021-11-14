/**********		leebootFAT32.c		 *********/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "structfat.h"
#include "structfat16.h"
#include "structfat32.h"

int main(int narg, char const *argv[])
{
	
	int fd;
	if(narg!= 2){
		printf("Usage; %s <name image disc>\n",argv[0]);
		exit(1);
	}
	if((fd=open(argv[1],O_RDONLY)) <0 )
		perror("No pudo abrir imagen de disco");

	lseek(fd,17,SEEK_SET); //me adelanto hasta antes de leer root directory entries
	unsigned short cantRootEntries;
	if( read(fd,&cantRootEntries,sizeof(cantRootEntries)) <0 )
		perror("No pudo leer imagen de disco");
	if(cantRootEntries==0){
		printf("Soy FAT 32\n");
		lseek(fd,0,SEEK_SET); //regreso al inicio para leer la estructura que ya conozco de antemano
		Fat32BootSector boot;
		if( read(fd,&boot,sizeof(boot)) <0 )
			perror("No pudo leer imagen de disco");
		printf("Jump code: %x:%x:%x\n",boot.jmp[0],boot.jmp[1],boot.jmp[2]);
		printf("OEM code: %s \n",boot.oem);
		printf("sector_size: %d\n",boot.sector_size);
		printf("sectors_per_cluster:%d\n", boot.sectors_per_cluster);
		printf("reserved_sectors: %d\n",boot.reserved_sectors);
		printf("number_of_fats: %d\n",boot.number_of_fats);
		printf("root_dir_entries:%d\n",boot.root_dir_entries);
		printf("total_sector_short: %d\n",boot.total_sectors_short);//if(boot.total_sectors_short!= 0)
		printf("media_descriptor: %d\n",boot.media_descriptor);
		printf("fat_size_sectors: %d\n",boot.fat_size_sectors);//0
		printf("sectors_per_track: %d\n",boot.sectors_per_track);
		printf("number_of_heads: %d\n",boot.number_of_heads);
		printf("hidden_sectors: %d\n",boot.hidden_sectors);
		printf("total_sectors: %d\n",boot.total_sectors);
		printf("sectors_per_fat: %d\n",boot.sectors_per_fat);
		printf("mirror_flag: %d\n",boot.mirror_flag);
		printf("filesystem_version: %d\n",boot.filesystem_version);
		printf("first_cluster: %d\n",boot.first_cluster);
		printf("filesystem_info: %d\n",boot.filesystem_info);
		printf("backup_boot_sector: %d\n",boot.backup_boot_sector);
		//reserved
		printf("logical_drive_number: %d\n",boot.logical_drive_number);
		printf("reserved_head: %d\n",boot.reserved_head);
		printf("extended_signature: %x\n",boot.extended_signature);
		printf("serial_number_partition: %x\n",boot.serial_number_partition);
		printf("volume_label: %.11s\n",boot.volume_label);
		printf("filesystem_type: %.8s\n",boot.filesystem_type);
		//printf("boot_sector_signature: %x\n",boot.boot_sector_signature);
	}else if (cantRootEntries==224){
		printf("Soy FAT 12\n");
		lseek(fd,0,SEEK_SET); //regreso al inicio para leer la estructura que ya conozco de antemano
		Fat12BootSector boot;
		if( read(fd,&boot,sizeof(boot)) <0 )
			perror("No pudo leer imagen de disco");
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

	} else if(cantRootEntries==512){
		printf("Soy FAT 16\n");
		lseek(fd,0,SEEK_SET); //regreso al inicio para leer la estructura que ya conozco de antemano
		Fat16BootSector boot;
		if( read(fd,&boot,sizeof(boot)) <0 )
			perror("No pudo leer imagen de disco");
		printf("Jump code: %x:%x:%x\n",boot.jmp[0],boot.jmp[1],boot.jmp[2]);
		printf("OEM code: %s \n",boot.oem);
		printf("sector_size: %d\n",boot.sector_size);
		printf("sectors_per_cluster:%d\n", boot.sectors_per_cluster);
		printf("reserved_sectors: %d\n",boot.reserved_sectors);
		printf("number_of_fats: %d\n",boot.number_of_fats);
		printf("root_dir_entries:%d\n",boot.root_dir_entries);
		if(boot.total_sectors_short!= 0)printf("total_sector_short: %d\n",boot.total_sectors_short);
		printf("media_descriptor: %d\n",boot.media_descriptor);
		printf("fat_size_sectors: %d\n",boot.fat_size_sectors);
		printf("sectors_per_track: %d\n",boot.sectors_per_track);
		printf("number_of_heads: %d\n",boot.number_of_heads);
		printf("hidden_sectors: %d\n",boot.hidden_sectors);
		if(boot.total_sectors_short== 0)printf("total_sectors: %d\n",boot.total_sectors);
		printf("logical_drive_number: %d\n",boot.logical_drive_number);
		printf("reserved: %d\n",boot.reserved);
		printf("extended_signature: %x\n",boot.extended_signature);
		printf("serial_number_partition: %x\n",boot.serial_number_partition);
		printf("volume_label: %.11s\n",boot.volume_label);
		printf("filesystem_type: %.8s\n",boot.filesystem_type);
		printf("boot_sector_signature: %x\n",boot.boot_sector_signature);
	}

	close(fd);
	return 0;
}