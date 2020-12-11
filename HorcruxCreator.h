#pragma once

#include <stdlib.h>
#include <string>

void horcruxCreator(FILE *iInputFile, int iHorcruxAmount, int iFileSize, const std::string& iHorcruxName, const std::string& iPath)
{
	size_t buffSize = iFileSize / (iHorcruxAmount);
	unsigned char *readBuf = new unsigned char[buffSize];
	int aHorcruxCount = 0;
	std::FILE *newHorcruxFile;
	std::string aHorcruxFileName;
	while (true)
	{
		aHorcruxCount++;

		aHorcruxFileName = iPath + iHorcruxName +  "Horcrux" + std::to_string(aHorcruxCount) + ".txt";
		if(aHorcruxCount == iHorcruxAmount)
		{
			buffSize += 1;
		}
		size_t charactersRead = fread(readBuf, sizeof(unsigned char), buffSize, iInputFile);
		if (charactersRead == 0) 
		{
            break;
        }
		else 
		{
			newHorcruxFile = std::fopen(aHorcruxFileName.c_str(), "wb");
        	fwrite(readBuf, sizeof(unsigned char), charactersRead, newHorcruxFile);
			fclose(newHorcruxFile);
		}
	}
}