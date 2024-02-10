/* ------------------------------------------------------------------------------------ *
* File Name  : FileHandler.cpp
* Author	 : Honggyu Park & Jongeon Lee
* Date		 : 2024-01-26
* Description: This file will be handle the grabing a requsted file from the client
*				>> Implements the ability to read and write data from a file
				>> Write functions to load and save files from disk
* ------------------------------------------------------------------------------------ */

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>

using namespace std;

#define HEADER_LEN    16
#define CHECK_SUM_LEN 2
#define MAXIMUM_FILE_NAME_LEN (256 - HEADER_LEN - CHECK_SUM_LEN)


bool IsFileValid(const unsigned char fileName[], int size)
{
    // Check whether the data is empty or not
    // Check that it is not larger than the maximum size that can be specified in the file name
    if (size <= 0 || size > MAXIMUM_FILE_NAME_LEN) return false;

    // Convert fileName to std::string for extension checking
    string fileFullNameStr(reinterpret_cast<const char*>(fileName), size);

    // Find the position of the last dot ('.')
    size_t lastDotPos = fileFullNameStr.find_last_of('.');
    size_t firstNull = fileFullNameStr.find_first_of('\0');

    // If there is no dot or the last character of the file name is a dot (no extension)
    if (lastDotPos == string::npos || lastDotPos == fileFullNameStr.length() - 1) return false;

    // Extract the file extenstion only from the full file name 
    // and Check if the extension is compatible
    string fileExtension = fileFullNameStr.substr(lastDotPos + 1, firstNull - lastDotPos - 1); // Extract only extenstion without '/0'
    transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);   // Transform to the lower case letter

    // Compatible extension list
    vector<std::string> compatibleExtensions = { "txt", "pdf", "docx", "png", "jpg" };

    // Checks if a given extension is in the list of compatible extensions
    if (find(compatibleExtensions.begin(), compatibleExtensions.end(), fileExtension) == compatibleExtensions.end()) 
    {
        // In case of incompatible extension
        return false;
    }

    // Returns true if a valid extension exists
    return true;
}

