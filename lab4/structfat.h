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