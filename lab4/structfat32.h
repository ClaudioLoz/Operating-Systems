/**********		structfat32.h		 *********/
typedef struct{
	unsigned char jmp[3];
	char oem[8];
	unsigned short sector_size;
	unsigned char sectors_per_cluster;
	unsigned short reserved_sectors;
	unsigned char number_of_fats;
	unsigned short root_dir_entries;//en este caso 0
	unsigned short total_sectors_short; //en este caso 0
	unsigned char media_descriptor;
	unsigned short fat_size_sectors; //en este caso 0
	unsigned short sectors_per_track;
	unsigned short number_of_heads;
	unsigned int hidden_sectors; //desde aqui difiere con el de 12
	unsigned int total_sectors;
	unsigned int sectors_per_fat;
	unsigned short mirror_flag;
	unsigned short filesystem_version;
	unsigned int  first_cluster;
	unsigned short filesystem_info;
	unsigned short backup_boot_sector;
	char reserved[12];
	unsigned char logical_drive_number;
	unsigned char reserved_head;
	unsigned char extended_signature;
	unsigned int serial_number_partition;
	char volume_label[11];
	char filesystem_type[8];
	//char boot_code[448];
	//unsigned short boot_sector_signature;
} __attribute((packed)) Fat32BootSector;