/* ---------------------------------------------------------------------------------------- *
* File Name  : PacketHandler.cpp
* Author	 : Honggyu Park & Jongeon Lee
* Date		 : 2024-01-26
* Description: This file is related to the PacketHandler.cpp
*			   This file includes the funcitons to make a packet from PacketHandler.cpp
*				>> Implements 
* ----------------------------------------------------------------------------------------- */ 
#include <string>
#include <stdint.h>

#pragma warning(disable: 4996)

using namespace std;

unsigned char calculateChecksum(char* sendData, int dataSize)
{
    unsigned char sum = 0;

    for (int i = 0; i < dataSize; ++i) {
        sum += sendData[i];
    }

    return ~sum & 0xFF;
}


// CRC32 Table initialation
// To use CRC32 step in the main code
// 1. declare     uint32_t crc32; size_t file_size;
// 2. crc32_init();
// 3. crc32 = calculate_crc32(buffer, file_size);
uint32_t crc32_table[256];

// CRC32 function initialation
void crc32_init() {
    uint32_t crc;
    int i, j;

    for (i = 0; i < 256; i++) {
        crc = i;
        for (j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
        crc32_table[i] = crc;
    }
}

// CRC32 function calculation
uint32_t calculate_crc32(const unsigned char* data, size_t len) {
    uint32_t crc = 0xFFFFFFFF;
    size_t i;

    for (i = 0; i < len; i++) {
        crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ data[i]];
    }

    return ~crc;
}