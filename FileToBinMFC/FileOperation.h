#pragma once
#include "stdio.h"
#include "string.h"

class FileOperation
{
public:
	FileOperation(void);
	~FileOperation(void);
	static FILE * writeBinFileInit(char * fileName);
	static int writeBinFileUpdte(FILE * file, unsigned char * inputData, int inputDataLen);
	static int writeBinFileFinal(FILE * filePointer);
	int readArrayFromTxt(char * inputFileName,char au08ReadOneLineDataCache[], int readCacheLen, unsigned char * au08ReadTextData);
	int hexStringToRealHex(unsigned char * au08ReadTextData, long inputDataLen, unsigned char * outputBuf);
	int charToHex(char oneChar);
	int getFileSize(char * fileDir);
	int writeToBinFile(char * fileName, unsigned char inputData[], int inputDataLen);
	int readTextWriteToBinMain(char * fileName);
	char * getOutputBinName();
	char * getOutputLogName();
	bool removeOutputBinFile();
	bool writeLogFile(char *logName, char * logData);

};

