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