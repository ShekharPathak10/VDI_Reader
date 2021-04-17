#ifndef PARTITION_H
#define PARTITION_H

struct __attribute__((packed))partitionEntry {
  uint8_t status;
  uint8_t firstSectorCHS[3];
  uint8_t partitionType;
  uint8_t lastSectorCHS[3];
  uint32_t firstLBASector;
  uint32_t sectorCount;
};

struct partitionFile{
    VDIFile * vdiFile;
    partitionEntry partition_Entry;
};

struct partitionFileTables{

    VDIFile * vdiFile;
    partitionEntry table[4];
};

#endif /* PARTITION_H */
