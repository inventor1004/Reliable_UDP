#include "pch.h"
#include "../Reliable_UPD/FileHandler.cpp"


#define HEADER_LEN    16
#define CHECK_SUM_LEN 2
#define MAXIMUM_FILE_NAME_LEN (256 - HEADER_LEN - CHECK_SUM_LEN)

TEST(FileHandlerTest, ValidateFileNames)
{
	unsigned char fileName[MAXIMUM_FILE_NAME_LEN];
	memset(fileName, 0, sizeof(fileName));

	const char* source = "test.txt";
	std::copy(source, source + strlen(source) + 1, fileName);
	EXPECT_TRUE(IsFileValid(fileName, sizeof(fileName)));

	source = "test.TXT";
	std::copy(source, source + strlen(source) + 1, fileName);
	EXPECT_TRUE(IsFileValid(fileName, sizeof(fileName)));

	source = "test.pdf";
	std::copy(source, source + strlen(source) + 1, fileName);
	EXPECT_TRUE(IsFileValid(fileName, sizeof(fileName)));

	source = "test.docx";
	std::copy(source, source + strlen(source) + 1, fileName);
	EXPECT_TRUE(IsFileValid(fileName, sizeof(fileName)));

	source = "test.jpg";
	std::copy(source, source + strlen(source) + 1, fileName);
	EXPECT_TRUE(IsFileValid(fileName, sizeof(fileName)));

	source = "test.png";
	std::copy(source, source + strlen(source) + 1, fileName);
	EXPECT_TRUE(IsFileValid(fileName, sizeof(fileName)));
}


TEST(FileHandlerTest, InvalidateFileNames)
{
	unsigned char fileName[MAXIMUM_FILE_NAME_LEN];
	memset(fileName, 0, sizeof(fileName));

	// Test the file name without extension
	const char* source = "test";
	std::copy(source, source + strlen(source) + 1, fileName);
	EXPECT_FALSE(IsFileValid(fileName, sizeof(fileName)));

	// Test the file name has '.' only
	source = "test.";
	std::copy(source, source + strlen(source) + 1, fileName);
	EXPECT_FALSE(IsFileValid(fileName, sizeof(fileName)));

	// File name with incompatible extension name
	source = "test.test";
	std::copy(source, source + strlen(source) + 1, fileName);
	EXPECT_FALSE(IsFileValid(fileName, sizeof(fileName)));
	
}