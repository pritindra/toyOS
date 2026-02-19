#ifndef ATA_H
#define ATA_H

#include <stdint.h>

#define ATA_DATA_PORT         0x1F0
#define ATA_FEATURES_PORT     0x1F1
#define ATA_SECTOR_COUNT_PORT 0x1F2
#define ATA_LBA_LO_PORT       0x1F3
#define ATA_LBA_MID_PORT      0x1F4
#define ATA_LBA_HI_PORT       0x1F5
#define ATA_DRIVE_PORT        0x1F6
#define ATA_COMMAND_PORT      0x1F7
#define ATA_STATUS_PORT       0x1F7

// Status flags
#define ATA_SR_BSY  0x80    // Busy
#define ATA_SR_DRQ  0x08    // Data request ready

void ata_read_sector(uint32_t lba, uint8_t* buffer);
void ata_write_sector(uint32_t lba, uint8_t* buffer);

#endif