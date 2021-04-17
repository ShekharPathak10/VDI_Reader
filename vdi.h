#ifndef VDI_H
#define VDI_H

struct UUID {
    uint32_t
        timeLow;
    uint16_t
        timeMid,
        timeHigh,
        clock;
    uint8_t
        node[6];
};

struct __attribute__((packed))HeaderStructure{
	uint8_t image_info[64];
	uint32_t signature; 
	uint32_t version;
	uint32_t header_size;
	uint32_t image_type;
	uint32_t flags; 
	uint8_t imgComment[256];
	uint32_t mapOffset;
	uint32_t frameOffset;
	uint32_t logicalChs;
	uint32_t sector_size; 
	uint32_t virtualCHS;
	uint32_t sectorSize;
	uint32_t frames_allocated;
	uint32_t disk_size; 
	uint32_t test;
	uint32_t frame_size; 
	uint32_t extra_frame_size; 
	uint32_t total_frame; 
	uint32_t allocatedFrames; 
	uint32_t uuid_create[4];
	uint32_t uuid_snap[4];
	uint32_t uuid_link[4];
	uint32_t uuid_parent[4]; 
};

struct __attribute__((packed))VDIFile {
    int fd;
    HeaderStructure header;
    ssize_t cursor;
};

#endif /* VDI_H */

