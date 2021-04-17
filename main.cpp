#include <iostream>
#include <fcntl.h>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <iomanip>
#include "vdi.h"
#include "partition.h"
#include "ext2.h"
#include "inode.h"


using namespace std; 



//Declaration
void displayBuffer (uint8_t *,int);
void displayBufferPage (uint8_t *, uint32_t, uint32_t, uint64_t);
struct VDIFile *vdiOpen (char *fn);
void vdiClose (struct VDIFile *f);
ssize_t vdiRead (struct VDIFile *f,void *buf,size_t count);
ssize_t  vdiWrite (struct VDIFile *f,void *buf,size_t count);
off_t vdiSeek (VDIFile *f,off_t offset, int anchor);
void dumpVDIHeader (struct VDIFile *);
void partitionDump(struct partitionEntry *);
void partitionAccess(struct partitionEntry *);
void displaySuperBlock(struct Ext2SuperBlocks);
void displayBGD(struct Ext2BlockGroupDescriptor BGD[]);

void blockGroupRead(struct blockGroupDescriptor *);
void printInode(struct VDIFile *, struct Ext2BlockGroupDescriptor [], int, struct partitionEntry [], struct Ext2SuperBlocks , int, struct inode);


int main(int argc, char *argv[]){
	struct VDIFile *f = vdiOpen(argv[1]);
	
	cout << endl << endl; 
	uint8_t buffer[400];
	lseek(f->fd, 0, SEEK_SET);
	vdiRead(f, buffer, sizeof(buffer));
	displayBuffer(buffer, sizeof(buffer));
	dumpVDIHeader(f);


	//Accessing Partition
	struct partitionEntry partitionEntries[4];
	vdiSeek(f, 446, SEEK_SET);
	vdiRead(f, &partitionEntries, sizeof(partitionEntries));
	partitionDump(partitionEntries);
	
	
	//Accessing SuperBlock
	struct Ext2SuperBlocks s;
	int offset = partitionEntries[0].firstLBASector * 512+1024;
	vdiSeek(f, offset, SEEK_SET);
	vdiRead(f, &s, sizeof(s));
	displaySuperBlock(s);
	
	//Block Group Count
	int blockGroupCount = (s.s_blocks_count) / (s.s_blocks_per_group);
	stringstream stream;
	stream << blockGroupCount; 
	stream >> hex >> blockGroupCount;
	if(blockGroupCount != 0){
		blockGroupCount++;
	}
	
	struct Ext2BlockGroupDescriptor BGD[blockGroupCount];
	int blockSize = 1024 << s.s_log_block_size;
	int offsetToBGD;
	if(blockSize == 1024) {
		offsetToBGD = partitionEntries[0].firstLBASector * 512 + 2*blockSize;
	}
	else{
		offsetToBGD = partitionEntries[0].firstLBASector * 512 + 1*blockSize;
	}

	vdiSeek(f, offsetToBGD, SEEK_SET);
	vdiRead(f, BGD, sizeof(BGD));
	displayBGD(BGD);
	
	
	struct inode ino;
	printInode(f, BGD, blockSize, partitionEntries, s, 2, ino);
	free(f);
	
}





