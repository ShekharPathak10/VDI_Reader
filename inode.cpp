#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <iomanip>
#include "vdi.h"
#include "partition.h"
#include "ext2.h"
#include "inode.h"
#include <ctime>

using namespace std; 

off_t vdiSeek (VDIFile *f,off_t offset, int anchor);
ssize_t vdiRead (struct VDIFile *f,void *buf,size_t count);
ssize_t  vdiWrite (struct VDIFile *f,void *buf,size_t count);

void printInode(struct VDIFile *f, struct Ext2BlockGroupDescriptor BGD[], int blockSize, struct partitionEntry partition_entry[], struct Ext2SuperBlocks super, int inodeNum, struct inode ino){
	int offsetToBG = (inodeNum - 1) % super.s_inodes_per_group;
	
	int bgNum = (inodeNum - 1) / super.s_inodes_per_group;
	
	int offSetToIn = partition_entry[0].firstLBASector * 512 + (BGD[bgNum].bg_inode_table) * blockSize + offsetToBG * super.s_inode_size;
	
	vdiSeek(f, offSetToIn, SEEK_SET);
	vdiRead(f, &ino, sizeof(ino));
	cout << endl << "INODE CONTENTS: " << endl;
	cout << "Mode: " << oct <<  ino.i_mode << endl;
	cout << "Size: " << dec  <<  ino.i_size << endl;
	cout << "Blocks: " <<  ino.i_blocks << endl;
	cout << "UID / GID:  " << dec << ino.i_uid << "/" <<  ino.i_gid << endl;
	cout << "Links: " <<  ino.i_links_count << endl;	
	time_t t = ino.i_ctime;
	cout << "Created: " << asctime(localtime(&t));
	time_t t1 = ino.i_atime;
	cout << "Last access: " << asctime(localtime(&t1));
	time_t t2 = ino.i_mtime;
	cout << "Last modification: " << asctime(localtime(&t2));
	time_t t3 = ino.i_dtime;
	cout << "Deleted: " << asctime(localtime(&t3));
	cout << "Flags: " <<  ino.i_flags << endl;
	cout << "File Version: " <<  ino.i_generation << endl;
	cout << "Direct blocks: "<< endl;
	cout << "          0-3: ";
	for(int x = 0; x < 4; x++){
		cout << ino.i_block[x] << "\t";
	}
	cout << endl << "          4-7: ";
	for(int x = 3; x < 7; x++){
		cout << ino.i_block[x] << "\t";
	}
	cout << endl << "         8-11: ";
	for(int x = 7; x < 11; x++){
		cout << ino.i_block[x] << "\t";
		}	
	
	cout << endl << "Single indirect block: " <<  ino.i_file_acl << endl;
	cout << "Double indirect block: " <<  ino.i_dir_acl << endl;
	cout << "Triple indirect block: " <<  ino.i_faddr << endl << endl;
}
