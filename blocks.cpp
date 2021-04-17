#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iomanip>
#include <stdio.h>    
#include <math.h>
#include <time.h>
#include "vdi.h"
#include "partition.h"
#include "ext2.h"
#include "inode.h"

using namespace std;

off_t partitionSeek(struct partitionFile*, off_t , int );
ssize_t partitionRead(struct partitionFile *, void *, size_t);
ssize_t partitionWrite(struct partitionFile *, void *, size_t);


//Blocks Read function
int32_t fetchBlock(struct Ext2File *ext2_file, uint32_t blockNum, void *buf){
	partitionSeek(&ext2_file->partition_File, (blockNum * ext2_file->superBlocks.s_log_block_size), SEEK_SET);
	int t = partitionRead(&ext2_file->partition_File, buf, ext2_file->superBlocks.s_log_block_size);
	if(t != -1){return 0;}
	else{return -1;}
}


//Blocks Write function
int32_t writeBlock(struct Ext2File *ext2_file, uint32_t blockNum, void *buf){
	partitionSeek(&ext2_file->partition_File, (blockNum * ext2_file->superBlocks.s_log_block_size), SEEK_SET);
	int w = partitionWrite(&ext2_file->partition_File, buf, ext2_file->superBlocks.s_log_block_size);
	if(w != -1){return 0;}
	else{return -1;}
}

/*
//Superblock Read function
int32_t fetchSuperblock(struct Ext2File *ext2_file, uint32_t blockNum, struct Ext2Superblocks &sb){
	int comp = blockNum / ext2_file->superBlocks.s_blocks_per_group;
	
	if(comp == 0){
		partitionSeek(ext2_file->partition_File, ext2_file->superBlocks.s_first_data_block * ext2_file->superBlocks.s_log_block_size, SEEK_SET);
		partitionRead(&ext2_file->partition_File, 400, 1024);
		
		//Error while trying to access the s_log_block_size through SuperBlock sb!!
		(sb->s_log_block_size) = 1024 * pow(2, sb->s_log_block_size);
	}
	else if(comp > 0){
		void *comp = malloc(ext2_file->superBlocks.s_log_block_size); 
		
		int b = comp * ext2_file->superBlocks.s_blocks_per_group; 
		
		fetchBlock(ext2_file, b, comp); 
		memcpy(sb, comp, 1024);
		
		sb->s_log_block_size = pow(2, sb->s_log_block_size) * 1024;
	}
	return 0; 
}

//SuperBlock Write Function
int32_t writeSuperblock(struct Ext2File *ext2_file, uint32_t blockNum, struct Ext2Superblock *sb){
	int comp = blockNum / (ext2_file->superBlocks.s_blocks_per_group);
	
	if(comp == 0){
		partitionSeek(ext2_file->partition_File, ext2_file->superBlocks.s_log_block_size, SEEK_SET);
		partitionWrite(&ext2_file->partition_File, ext2_file, 1024);
		
		sb->s_log_block_size = pow(2, sb->s_log_block_size) * 1024;
	}
	else if(comp > 0){
		void *ref = malloc(ext2_file->superBlocks.s_blocks_per_group);
		writeBlock(ext2_file, comp, ref);
		memcpy(ext2_file, ref, 1024);
	}
	return 0;
}
*/

int32_t fetchBGDT(struct Ext2File *ext2_file, uint32_t blockNum, struct Ext2BlockGroupDescriptor *bgdt){
	int comp = blockNum / (ext2_file->superBlocks.s_blocks_per_group);
	
	bgdt->bg_block_bitmap = ext2_file->blockGroupDescriptorstable[comp].bg_block_bitmap;
	bgdt->bg_inode_bitmap = ext2_file->blockGroupDescriptorstable[comp].bg_inode_bitmap;
	bgdt->bg_inode_table = ext2_file->blockGroupDescriptorstable[comp].bg_inode_table;
	bgdt->bg_free_blocks = ext2_file->blockGroupDescriptorstable[comp].bg_free_blocks;
	bgdt->bg_free_inodes = ext2_file->blockGroupDescriptorstable[comp].bg_free_inodes;
	bgdt->bg_dirs_counts = ext2_file->blockGroupDescriptorstable[comp].bg_dirs_counts;
	bgdt->bg_pad = ext2_file->blockGroupDescriptorstable[comp].bg_pad;
	bgdt->bg_reserved[0] = ext2_file->blockGroupDescriptorstable[comp].bg_pad;
	bgdt->bg_reserved[1] = ext2_file->blockGroupDescriptorstable[comp].bg_pad;
	bgdt->bg_reserved[2] = ext2_file->blockGroupDescriptorstable[comp].bg_pad;

	return 0;
}


void displaySuperBlock(Ext2SuperBlocks s){
	cout << endl << "SUPERBLOCKS CONTENTS: " << endl;
	cout << "Number of inodes: " << s.s_inodes_count << endl;
	cout << "Number of blocks: " << s.s_blocks_count<<endl;
	cout << "Number of reserved blocks: " << s.s_r_blocks_count<<endl;
	cout << "Number of free blocks: " << s.s_free_blocks_count<<endl;
	cout << "Number of free inodes: " << s.s_free_inodes_count<<endl;
	cout << "First data block: " << s.s_first_data_block<<endl;
	cout << "Log block size: " << s.s_log_block_size<<endl;
	cout << "Log fragment size: " << s.s_log_frag_size<<endl;
	cout << "Blocks per group: " << s.s_blocks_per_group<<endl;
	cout << "Fragments per group: " << s.s_frags_per_group<<endl;
	cout << "Inodes per group: " << s.s_inodes_per_group<<endl;
	time_t t = s.s_mtime;
	cout << "Last mount time: " << asctime(localtime(&t));
	time_t t1 = s.s_wtime;
	cout << "Last write time: " << asctime(localtime(&t1));
	cout << "Mount count: " << s.s_mnt_count << endl;
	cout << "Max mount count: " << s.s_max_mnt_count<<endl;
	cout << "Magic number: " << s.s_magic<<endl;
	cout << "State: " << s.s_state<<endl;
	cout << "Error processing: " << s.s_errors<<endl;
	cout << "Revision level: " << s.s_minor_rev_level<<endl;
	time_t t2 = s.s_lastcheck;
	cout << "Last system check: " << asctime(localtime(&t2));
	cout << "Check interval: " << s.s_checkinterval<<endl;
	cout << "OS creator: " << s.s_creator_os<<endl;
	cout << "Default reserve UID: " << s.s_def_resuid<<endl; 
	cout << "Default reserve GID: " << s.s_def_resgid<<endl;
	cout << "First inode number: " << s.s_first_ino<<endl;
	cout << "inode size: " << s.s_inode_size<<endl;
	cout << "Block group number: " << s.s_block_group_nr<<endl;
	cout << "Feature Compatibility bits: 0x" << setfill('0') << setw(8) << s.s_feature_compat<<endl;
	cout << "Feature incompatibility bits: 0x" << setfill('0') << setw(8) << s.s_feature_incompat<<endl;
	cout << "feature read/only compatibility bits: 0x" << setfill('0') << setw(8) << s.s_feature_ro_compat<<endl;
	cout << "UUID: ";
	for(int i = 0; i < 4; i++){
		cout << hex << (int)s.s_uuid[i];
		if(i < 3){cout << "-";}
	}
	
	cout << endl << "Volume name: " << "[";
	for(int i = 0; i < 5; i++){
		char print = s.s_volume_name[i];
		cout << print;
	}
	cout << "]" << endl;
	
	cout << "Last mount point: "; 
	for(int i = 0; i < 17; i++){
		cout << hex << s.s_last_mounted[i];
		//if()
	}
	
	cout << endl << "Algorithm bitmap: " << s.s_algorithm_bitmap << endl;
	cout << "Number of blocks to preallocate: " << (int)s.s_prealloc_blocks<<endl;
	cout << "Number of blocks to preallocate for directories: " << (int)s.s_prealloc_dir_blocks<<endl;
	cout << "Journal UUID: " << s.s_journal_uuid[4] << endl;
	cout << "Journal inode number: " << s.s_journal_inum << endl;
	cout << "Journal device number: " << s.s_journal_dev << endl;
	cout << "Journal last orphan inode number: " << s.s_last_orphan << endl;
	cout << "Default hash version: " <<(int)s.s_def_hash_version << endl;
	cout << "Default mount option bit map: "<< "0x" << setfill('0') << setw(8) << s.s_default_mount_op<<endl;
	cout << "First meta block group: " << s.s_first_meta_bg << endl;
}

void displayBGD(Ext2BlockGroupDescriptor BGD[]){
	int count = 16;
	cout << endl << "BLOCK GROUP DESCRIPTOR TABLE: " << endl; 
	
	//Table Header
	cout << setw(15) << setfill(' ') <<"Block Number" << setw(15) << setfill(' ') << "Block Bitmap" << setw(15) << setfill(' ') << "Inode Bitmap" << setw(15) << setfill(' ') << "Inode Table"; 
	cout << setw(15) << setfill(' ') <<"Free Blocks" << setw(15) << setfill(' ') << "Free Inodes" << setw(15) << setfill(' ') <<"Used Dirs" << endl;
	
	//Formatting
	cout << setw(15) << setfill(' ') <<"------------" << setw(15) << setfill(' ') << "------------" << setw(15) << setfill(' ') << "------------" << setw(15) << setfill(' ') << "-----------"; 
	cout << setw(15) << setfill(' ') <<"------------" << setw(15) << setfill(' ') << "----------" << setw(15) << setfill(' ') <<"---------" << endl;
	
	// Table Data
	for(int i = 0; i < count; i++){
		cout << dec << setw(15) << setfill(' ') << i;
 	
		cout << dec << setw(15) << setfill(' ') << BGD[i].bg_block_bitmap;
 	
		cout << dec << setw(15) << setfill(' ') << BGD[i].bg_inode_bitmap;

		cout << dec << setw(15) << setfill(' ') << BGD[i].bg_inode_table;

		cout << dec << setw(15) << setfill(' ') << BGD[i].bg_free_blocks;

		cout << dec << setw(15) << setfill(' ') << BGD[i].bg_free_inodes;

		cout << dec << setw(15) << setfill(' ') << BGD[i].bg_dirs_counts << endl;
	}
	cout << endl;

}




