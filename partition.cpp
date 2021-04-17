#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iomanip>
#include "vdi.h"
#include "partition.h"
#include "ext2.h"

using namespace std;

ssize_t vdiRead(struct VDIFile *f, void *buf, size_t c);
off_t vdiSeek(VDIFile *, off_t, int);
ssize_t vdiWrite(struct VDIFile *, void *, ssize_t);

//Printing the partition
void partitionDump(struct partitionEntry partitionEntries[]){
	cout << endl << endl << "PARTITION CONTENTS: " << endl;  
	for(int i = 0; i < 4; i++){
		cout << "Partition: " << i + 1 << endl;
		cout << "Status: " << (int) partitionEntries[i].status << endl;
		cout << "First Sector CHS: ";
		for(int x = 0; x < 3; x++){
			cout << (int) partitionEntries[i].firstSectorCHS[x]; 
			if(x < 2){cout << "-";}
		};
		cout << endl << "Partition Type: " << hex << (int) partitionEntries[i].partitionType << endl; 
		cout << "Last Sector CHS: "; 
		for(int j = 0; j < 3; j++){
			cout << (int) partitionEntries[i].lastSectorCHS[j];
			if(j < 2){cout << "-";} 
		};
		cout << endl << "First LBA Sector: " << dec << partitionEntries[i].firstLBASector << endl; 
		cout << "Sector Count: " << dec << partitionEntries[i].sectorCount << endl << endl; 
	}
	cout << endl;
}

struct partitionFile *partitionOpen(struct VDIFile *vdi, struct partitionEntry entry){
	partitionFile *p_file = new partitionFile; 
	p_file->vdiFile = vdi;
	p_file->partition_Entry = entry; 
	return p_file;
}

void partitionClose(struct partitionFile *p_file){
	close(p_file->vdiFile->fd);
}

ssize_t partitionRead(struct partitionFile *f, void *buf, size_t count ){
	int rNum = 0;
	if( ((f->vdiFile->cursor) + count) <= 512 * (f->partition_Entry.firstLBASector + f->partition_Entry.sectorCount)){
		if(f->partition_Entry.firstLBASector * 512 <= f->vdiFile->cursor){
			lseek(f->vdiFile->fd, f->vdiFile->cursor, SEEK_SET);
			int rNum = vdiRead(f->vdiFile, buf, count);	
		}
	}
	else{
		rNum = -1;
		}
	return rNum;
}


off_t partitionSeek(struct partitionFile *file, off_t offset, int anchor){
	off_t ofs = vdiSeek(file->vdiFile, offset + (file->partition_Entry.firstLBASector * 512), SEEK_SET);
	
	if(ofs < (file->partition_Entry.firstLBASector * 512)){
		if(ofs >= (file->partition_Entry.firstLBASector + file->partition_Entry.sectorCount) * 512){
			ofs = file->vdiFile->cursor;
		}
		else{
			file->vdiFile->cursor = ofs; 
		}
	} 
	return file->vdiFile->cursor;
}

ssize_t partitionWrite(struct partitionFile *file, void *buf, size_t count){
	int wNum = 0;
	if((count + file->vdiFile->cursor) <= 512 * (file->partition_Entry.firstLBASector + file->partition_Entry.sectorCount)){
		if(file->partition_Entry.firstLBASector * 512 <= file->vdiFile->cursor){
			int wNum = vdiWrite(file->vdiFile, buf, count);
		}
	}
	else{wNum = -1;}
	return wNum;
}


