#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <iomanip>
#include "vdi.h"
#include "partition.h"
//#include "ext2file.h"

using namespace std; 

//void displayBuffer (uint8_t *,int);
//void displayBufferPage (uint8_t *, uint32_t, uint32_t, uint64_t);
//void setup(struct VDIFile *);




/*void setup(struct VDIFile *f){
	char buf[39];
	uint8_t buffer[400];
	
	read(f->fd, buf, sizeof(buf));
	lseek(f->fd,0,SEEK_SET);
	int count=0;

	int totalNumberOfBytesRead = read(f->fd, buffer, sizeof(buffer));
	for (uint8_t x: buffer){
	cout<< std::hex<<(int)x<<"\t";
	count++;
	if (count==16){
		cout<<endl;
		count=0;
		}
	}	
	cout<<"Total bytes read:: "<< dec << totalNumberOfBytesRead << endl;
}*/

static char
    uuidStr[40];

static char *uuid2ascii(struct UUID *uuid) {
    sprintf(uuidStr,"%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x",
            uuid->timeLow,uuid->timeMid,uuid->timeHigh,uuid->clock,
            uuid->node[0],uuid->node[1],uuid->node[2],uuid->node[3],
            uuid->node[5],uuid->node[5]);
    return uuidStr;
} 

	
struct VDIFile *vdiOpen(char *fn){
	int fd = open(fn, O_RDWR);
	
	if(fd != 1){
		struct VDIFile * filestruct = (struct VDIFile *) malloc(sizeof(struct VDIFile));
		read(fd, &filestruct->header, sizeof(filestruct->header));
		filestruct->cursor = filestruct->header.frameOffset; 
		filestruct->fd = fd;
		return filestruct; 
	}
	
	else{
		return nullptr;
	}
}

void vdiClose(struct VDIFile *f){
	close ((*f).fd);
	delete f; 
}

ssize_t vdiRead(struct VDIFile *f, void *buf, size_t c){
	int bytesRead = read(f->fd, buf, c); 
	if(bytesRead == -1){
		cout << "Error while reading the file" << endl; 
	}
	
	return bytesRead; 
}

ssize_t vdiWrite(struct VDIFile *f, void *buf, ssize_t c){
	lseek(f->fd, f->cursor, SEEK_SET); 
	int bytes_read = write(f->fd, buf, c);
	return 0;
}

off_t vdiSeek(VDIFile *f, off_t offset, int anchor){
	off_t location; 
	if(anchor == SEEK_SET && (f->cursor + offset) < (f->header.disk_size)){ 
		location = lseek(f -> fd, f->cursor + offset, SEEK_SET); 
	}
	
	else if(anchor == SEEK_CUR && (f -> cursor + offset) < (f -> header.disk_size)){
		location = lseek(f->fd, f->cursor + offset, SEEK_SET);
	}
	
	else if(anchor == SEEK_END && offset <= 0){
		location = lseek(f->fd, f->cursor + offset, SEEK_SET);
	}
	
	return location;
}

void dumpVDIHeader(struct VDIFile *f){
	cout << endl <<"     Image Name: "; 
	for(int i = 0; i < 65; i++){
		char print = f->header.image_info[i]; 
		cout << print;
	}

	cout <<"       Signature: " << "0x" << setfill('0') << setw(8)  << (int)(f->header.signature) << endl;
	cout <<"         Version: " << setprecision(2) << hex  << (int)(f->header.version) <<endl;
	cout <<"     Header Size: " << "0x" << setfill('0') << setw(8)  << (int)(f->header.header_size) << dec << "  " << (int)(f->header.header_size) << endl;
	cout <<"      Image Type: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.image_type) << dec << "  " << (int)(f->header.image_type) << endl;
	cout <<"           Flags: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.flags) << dec << "  " << (int)(f->header.flags) << endl;
	cout <<"     Virtual CHS: " << (int)(f->header.virtualCHS) << endl;
	cout <<"     Sector size: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.sectorSize) << dec << "  " << (f->header.sectorSize) << endl;
	cout <<"     Logical CHS: " << (int)(f->header.logicalChs) <<endl;
	cout <<"     Sector_size: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.sectorSize) << dec << "  " << (int)(f->header.sectorSize) << endl;
	cout <<"      Map offset: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.mapOffset) << dec << "  " << (int)(f->header.mapOffset)  << endl;
	cout <<"    Frame offset: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.frameOffset) << dec << "  " << (int)(f->header.frameOffset) << endl;  
	cout <<"      Frame Size: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.frame_size) << dec << "  " << (int)(f->header.frame_size) << endl;
	cout <<"Extra Frame Size: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.extra_frame_size) << dec << "  " << (int)(f->header.extra_frame_size) << endl;
	cout <<"     Total Frame: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.total_frame) << dec << "  " << (int)(f->header.total_frame) << endl;
	cout <<"Frames Allocated: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.allocatedFrames) << dec << "  " << (int)(f->header.allocatedFrames) << endl;
	cout <<"       Disk Size: " << "0x" << setfill('0') << setw(8) << hex << (int)(f->header.disk_size) << dec << "  " << (int)(f->header.disk_size) << endl;
	cout <<"            UUID: " ;
	for(int i = 0; i < 4; i++){
		cout << hex << (int)f->header.uuid_create[i];
		if(i < 3){cout << "-";}
	}
	cout << endl <<"  Last snap UUID: " ;
	for(int i = 0; i < 4; i++){
		cout << hex << (int)f->header.uuid_snap[i];
		if(i < 3){cout << "-";}
	}
	cout << endl <<"       Link UUID: " ;
	for(int i = 0; i < 4; i++){
		cout << hex << (int)f->header.uuid_link[i];
		if(i < 3){cout << "-";}
	}
	cout << endl <<"     Parent UUID: " ;
	for(int i = 0; i < 4; i++){
		cout << hex << (int)f->header.uuid_parent[i];
		if(i < 3){cout << "-";}
	}
	cout << endl << "  Image comment:";
	for(int i = 0; i < 257; i++){
		char print = f->header.imgComment[i]; 
		cout << print;
	}
}

void displayBufferPage(int8_t *buf, uint32_t count, uint32_t start, uint64_t offset){
	cout << endl << "Printing buffer in hexadecimal" << endl << endl;
	int counter = 0;
	for(int i = start; i <= count; i++){
		if(start <= i && i < (start + count)){
			uint8_t byte = buf[i];
			cout << setfill('0') << setw(2) << hex << (int)(byte) << " ";
		} else {
			cout << endl;
			break;
		}	
		counter++;
		if(counter % 16 == 0){
			cout << endl;
		}
	}
	
	cout << endl;
	cout << "Printing buffer in text" << endl << endl; 
	for(int i = start; i <= count; i++){
		if(start <= i && i , (start + count)){
			uint8_t byte = buf[i];
			if(isprint(int(byte))){
				cout << byte << " ";
			} else {
				cout << " ";
			}
		}
		else{
		cout << endl;
		break;
		}
	}		
}

void displayBuffer(uint8_t *buf, int size){
	int count = 0 ; 
	cout << endl;
	cout << "DISPLAYING BUFFER: " << endl;
	for(int i = 0; i < size; i++){
		cout << setfill('0') << setw(2) << hex << (int)*(buf + i) << "   ";
		//cout << hex << (int)*(buf + i);
		count++;
		if(count == 16){
			cout << endl;
			count = 0;
		} 
	} 
	cout << endl;
}











