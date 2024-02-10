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


/*
 * Function     : calculateChecksum
 * Description  : Calculates the checksum of the input data.
 * Parameters   : char* sendData - Pointer to the input data array.
 *                int dataSize    - Size of the input data array.
 * Return       : unsigned char - The calculated checksum.
 */
unsigned char calculateChecksum(char* sendData, int dataSize)
{
    unsigned char sum = 0;

    for (int i = 0; i < dataSize; ++i) {
        sum += sendData[i];
    }

    return ~sum & 0xFF;
}


 /*
  * Function     : crc32_init
  * Description  : Initializes the CRC32 table.
  */
uint32_t crc32_table[256];
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


/*
 * Function     : calculate_crc32
 * Description  : Calculates the CRC32 value for the given data.
 * Parameters   : const unsigned char* data - Pointer to the input data array.
 *                size_t len                 - Size of the input data array.
 * Return       : uint32_t - The calculated CRC32 value.
 *
 * To use CRC32 step in the main code
 * 1. declare     uint32_t crc32; size_t file_size;
 * 2. crc32_init();
 * 3. crc32 = calculate_crc32(buffer, file_size);
 */
uint32_t calculate_crc32(const unsigned char* data, size_t len) {
    uint32_t crc = 0xFFFFFFFF;
    size_t i;

    for (i = 0; i < len; i++) {
        crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ data[i]];
    }

    return ~crc;
}


/*
 * Function     : breakingFile
 * Description  : Reads a binary file, calculates its CRC32 value, appends the checksum,
 *                and then prepares the data for transmission.
 * Parameters   : const char* fileName - Name of the input file.
 */
void breakingFile(const char* fileName) {
    size_t file_size;
    uint32_t crc32;
    unsigned char* buffer;

    FILE* pf_input;
    pf_input = fopen(fileName, "rb");
    if (pf_input == NULL) {
        printf("Error - opening input file.\n");
        return;
    }

    fseek(pf_input, 0, SEEK_END);
    file_size = ftell(pf_input);
    fseek(pf_input, 0, SEEK_SET);

    buffer = (unsigned char*)malloc(file_size);
    if (buffer == NULL) {
        perror("Error - Failed to allocate memory");
        fclose(pf_input);
        return;
    }

    if (fread(buffer, 1, file_size, pf_input) != file_size) {
        perror("Error - Failed to read file");
        free(buffer);
        fclose(pf_input);
        return;
    }

    crc32_init();
    crc32 = calculate_crc32(buffer, file_size);

    char* sendbuf = (char*)malloc(file_size);
    if (sendbuf == NULL) {
        printf("Error - Memory allocation failed.\n");
    }
    memset(sendbuf, 0, file_size);
    size_t bytesRead = fread(sendbuf, file_size, 1, pf_input);
    if (bytesRead < sizeof(buffer)) {
        if (ferror(pf_input)) {
            printf("Error reading file.\n");
            free(sendbuf);
            fclose(pf_input);
            return;
        }
    }
    fclose(pf_input);

    unsigned char checkSum = calculateChecksum(sendbuf, sizeof(sendbuf));
    sendbuf[sizeof(sendbuf)] = checkSum;

    // call send function

    free(sendbuf);
}


/*
 * Function     : fileWriteToDisk
 * Description  : Writes data to a file on the disk.
 * Parameters   : const char* fileName  - Name of the output file.
 *                size_t fileSize       - Size of the data to be written.
 *                const char* dataPiece - Pointer to the data to be written.
 */
void fileWriteToDisk(const char* fileName, size_t fileSize, const char* dataPiece) {
    char outputFileName[] = "";
    strcpy(outputFileName, fileName);

    FILE* pf_output;
    pf_output = fopen(outputFileName, "wb");
    if (pf_output == NULL) {
        printf("Error - opening output file.\n");
        fclose(pf_output);
        return;
    }
    while (1) {
        size_t bytesRead = fileSize;
        if (bytesRead <= 0) {
            if (feof(pf_output) != 0) {
                printf("Complete to transfer file.\n");
                break;
            }
            else {
                printf("Data transfer failed.\n");
                break;
            }
        }
        fwrite(dataPiece, 1, bytesRead, pf_output);
    }
    fclose(pf_output);
}