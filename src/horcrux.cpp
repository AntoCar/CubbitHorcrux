#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "Base64Transformer.h"
#include "HorcruxCreator.h"
#include "Cipher.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

unsigned char * instanceVectorCreator(unsigned char * iAesKey)
{
	unsigned char * iv = new unsigned char [16];
	for (int i = 0; i < 16; i++)
	{
		iv[i] = iAesKey[i];
	}

	return iv;
}

long myFileSize (FILE *fOUT)
{
	fseek (fOUT, 0, SEEK_END);
    long size=ftell (fOUT);
	fseek ( fOUT , 0 , SEEK_SET );
	fclose(fOUT);

	return size;
}

int main(int argc, char* argv[])
{
	const int defaultNumberOfArguments = 6;

	if (argc < defaultNumberOfArguments)
	{
		std::cout << "You are missing " << (defaultNumberOfArguments - argc) << " argument";
		(defaultNumberOfArguments - argc) ? std::cout << "s." : std::cout << ".";
		std::cout << std::endl;
		std::cout << "A correct form is either:\nhorcrux create -n <horcrux count> <input path> <output path>" << std::endl;
		std::cout << "Or\nhorcrux load -k <decryption key> <input files> <output_file>" << std::endl;
		return 0;
	}

	if (argv[1] == std::string("create") && argv[2] == std::string("-n"))
	{
		std::vector<std::string> aVectorOfFoldersAndOutputFile;
		boost::split(aVectorOfFoldersAndOutputFile, argv[4], boost::is_any_of("/"));

		std::FILE *fIN, *fOUT;
		fIN = fopen(argv[4], "rb");
		fOUT = fopen("output.txt", "wb");
	

		unsigned char * aesKey = new unsigned char [32];
    	RAND_bytes(aesKey, 32);
		unsigned char * iv = instanceVectorCreator(aesKey);
		conditionalEncryptionOrDecryption(TRUE, fIN, fOUT, aesKey, iv);
		char * aCharStarKey = reinterpret_cast<char*>(aesKey);
		std::string aNewCharStarKey = base64EncoderDecoder(true,aCharStarKey);
		unsigned char * aBase64aesKey = reinterpret_cast<unsigned char*>(const_cast<char*>(aNewCharStarKey.c_str()));
		std::cout << aBase64aesKey << std::endl;

    	long fileSize = myFileSize(fOUT);

		std::vector<std::string> aVectorOfFileNameAndExtension;
		boost::split(aVectorOfFileNameAndExtension, aVectorOfFoldersAndOutputFile[aVectorOfFoldersAndOutputFile.size() - 1], boost::is_any_of("."));

		fOUT = fopen("output.txt", "rb");
		HorcruxCreator::horcruxCreator(fOUT, std::stoi(argv[3]), fileSize, aVectorOfFileNameAndExtension[0], argv[5]);

		fclose(fIN);
    	fclose(fOUT);
		remove("output.txt");
		return 0;
	}

	if (argv[1] == std::string("load") && argv[2] == std::string("-k"))
	{
		std::vector<std::string> aVectorOfHorcruxFiles;
		for (int i=3; i<argc - 1; i++)
		{
			aVectorOfHorcruxFiles.push_back(argv[i]);
		}

		std::ifstream fileToAppend;
		std::string stringToAppend;

		for (int i = 0; i < aVectorOfHorcruxFiles.size(); i++)
		{
			fileToAppend.open(aVectorOfHorcruxFiles[i], std::ifstream::binary);
			
			std::string aTempString;
			aTempString.assign((std::istreambuf_iterator<char>(fileToAppend)),std::istreambuf_iterator<char>());
			stringToAppend = stringToAppend + aTempString;

			fileToAppend.close();
		}

		std::ofstream outputFile( "temporaryOutput.txt", std::ifstream::out | std::ifstream::binary ) ;
		outputFile << stringToAppend;
		outputFile.close();
		
		std::FILE *fIN;
		std::FILE *fOUT;
		fIN = fopen("temporaryOutput.txt", "rb");
		fOUT = fopen(argv[argc-1], "wb");

		std::string aAesKey = argv[3];
		std::string aNewAesKey = base64EncoderDecoder(false,aAesKey);
		unsigned char * aUnsignedCharAesKey = reinterpret_cast<unsigned char*>(const_cast<char*>(aNewAesKey.c_str()));

		unsigned char * iv = instanceVectorCreator(aUnsignedCharAesKey);

		conditionalEncryptionOrDecryption(FALSE, fIN, fOUT, aUnsignedCharAesKey, iv);

    	fclose(fIN);
    	fclose(fOUT);
		remove("temporaryOutput.txt");

		return 0;
	}

    return 0;
}