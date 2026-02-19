#include "ata.h"
#include "../include/io.h"

// Wait until the drive is no longer busy
static void ata_wait_bsy() {
    while (inb(0x1F7) & ATA_SR_BSY);
}

// Wait until the drive is ready to transfer data
static void ata_wait_drq() {
    while (!(inb(0x1F7) & ATA_SR_DRQ));
}

void ata_read_sector(uint32_t lba, uint8_t* buffer) {
    ata_wait_bsy();
    
    // Select master drive (0xE0) and send the highest 4 bits of the LBA
    outb(ATA_DRIVE_PORT, 0xE0 | ((lba >> 24) & 0x0F));
    
    // Send the rest of the LBA
    outb(ATA_SECTOR_COUNT_PORT, 1);             // Read exactly 1 sector
    outb(ATA_LBA_LO_PORT, (uint8_t)lba);        // Low 8 bits
    outb(ATA_LBA_MID_PORT, (uint8_t)(lba >> 8));  // Mid 8 bits
    outb(ATA_LBA_HI_PORT, (uint8_t)(lba >> 16));  // High 8 bits
    
    // Send the READ command (0x20)
    outb(ATA_COMMAND_PORT, 0x20);
    
    ata_wait_bsy();
    ata_wait_drq();
    
    // Read 256 words (512 bytes) from the data port
    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(ATA_DATA_PORT);
        buffer[i * 2] = (uint8_t)(data & 0xFF);
        buffer[i * 2 + 1] = (uint8_t)(data >> 8);
    }
}

void ata_write_sector(uint32_t lba, uint8_t* buffer) {
    ata_wait_bsy();
    
    outb(ATA_DRIVE_PORT, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_SECTOR_COUNT_PORT, 1);
    outb(ATA_LBA_LO_PORT, (uint8_t)lba);
    outb(ATA_LBA_MID_PORT, (uint8_t)(lba >> 8));
    outb(ATA_LBA_HI_PORT, (uint8_t)(lba >> 16));
    
    // Send the WRITE command (0x30)
    outb(ATA_COMMAND_PORT, 0x30);
    
    ata_wait_bsy();
    ata_wait_drq();
    
    // Write 256 words (512 bytes) to the data port
    for (int i = 0; i < 256; i++) {
        uint16_t data = buffer[i * 2] | (buffer[i * 2 + 1] << 8);
        outw(ATA_DATA_PORT, data);
    }
    
    // Flush cache to ensure it's written to physical disk
    outb(ATA_COMMAND_PORT, 0xE7);
    ata_wait_bsy();
}