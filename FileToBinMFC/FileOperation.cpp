#include "StdAfx.h"
#include "FileOperation.h"
#include "stdio.h"
#include "string.h"


FileOperation::FileOperation(void)
{
}


FileOperation::~FileOperation(void)
{
}



FILE *  FileOperation::writeBinFileInit(char * fileName) 
{
    FILE *file = fopen(fileName, "wb");
    if (file == NULL)
        return NULL;
    else 
		return file; 
}

int FileOperation::writeBinFileUpdte(FILE * file, unsigned char * inputData, int inputDataLen)
{
	int i=0;
	for(i=0; i<inputDataLen; i++) 
	{	     
	    fwrite(inputData+i, sizeof(unsigned char), 1, file);
	 
    }
	return 0;
}
 
 
int FileOperation::writeBinFileFinal(FILE * filePointer)
{
 	 fclose(filePointer);
	 return 0;
}



int FileOperation::readArrayFromTxt(char * inputFileName,char au08ReadOneLineDataCache[], int readCacheLen, unsigned char * au08ReadTextData)
{	
	FILE *fp;
   // char *au08ReadOneLineDataCache=au08ReadOneLineDataCache_input;
	long u64FinalHexDataOfs=0;   
	unsigned char tempOneChar;

	long u64OneLineCharactersLen=0;
	long realDataStartOfs=0;
	int u32FetchedHexNum;
	int i=0;
	short isDataValid=0;
	//to find '//'
	short lineCommentIsComing=0;
	//to find ' /************/ '
	short blockCommentIsComing=0;
	//if block comment is coming, we should find the end '*/
	short countBlockComment=0;
	
	//for left '{'
	char * strLeftBraces;
	long leftBracesOffset=0;
	//for right '}'
	char * strRightBraces;
	long rightBracesOffset=0;
	int  arraySizeStrLen=0;
	char strArraySize[100];
	int  intArraySize=0;
    // char * strArraySize02;
    int isArraySizeSet=0;
    
	//for left '['
	char * strLeftBrackets;
	long leftBracketsOffset=0;
	int isLeftBracketsFound=0;
	//for right ']'
	char * strRightBrackets; 
	long rightBracketsOffset=0;
	
	unsigned char u8HalfByteIsCome=0;
	unsigned char u8HalfByteCounter=0;
	
	if((fp=fopen(inputFileName,"r"))==NULL)
	{
		return 0;
	}
 
	//init
	isArraySizeSet=0;
	countBlockComment=0;
	isDataValid=1;
	memset(strArraySize, '\0', sizeof(strArraySize));

 	while(fgets(au08ReadOneLineDataCache, readCacheLen, fp))
 	{ 		
		isDataValid=1;
 		//get one line data length
 		u64OneLineCharactersLen=strlen(au08ReadOneLineDataCache);
 		realDataStartOfs=0; 		
 		lineCommentIsComing=0;
   		
 		//1. search'[' and ']' , this include the length
 	 	strLeftBrackets=strstr(au08ReadOneLineDataCache, "[");
 	 	if(strLeftBrackets!=NULL)
 	 	{
 	 		leftBracesOffset=strLeftBrackets-au08ReadOneLineDataCache;
 	 	}
  	 	strRightBraces=strstr(au08ReadOneLineDataCache, "]");
 	 	if(strRightBraces!=NULL)
 	 	{
 	 		rightBracesOffset=strRightBraces-au08ReadOneLineDataCache;
 	 	}
 		
 	 	arraySizeStrLen=rightBracesOffset-leftBracesOffset-1;
 	 	if(isArraySizeSet==0)
 	 	{
	 		if(arraySizeStrLen>0)
	 		{
	 			for(i=0; i<arraySizeStrLen; i++)
	 			{
					strArraySize[i]=au08ReadOneLineDataCache[leftBracesOffset+i+1];
	 			}			   			   
			   intArraySize=atoi(strArraySize);
			}
			isArraySizeSet=1;
 	 	} 	
 		  	
 		//2. search '{' and '}' , after { is the array data start 		
 		strLeftBrackets=strstr(au08ReadOneLineDataCache, "{");
		if(strLeftBrackets!=NULL)
 	 	{
 	 		//if '{' found 2 times, error is coming
 	 		if(isLeftBracketsFound)
 	 			break ;
 	 			
			isLeftBracketsFound=1;
			leftBracketsOffset=strLeftBrackets-au08ReadOneLineDataCache;
			realDataStartOfs=leftBracketsOffset+1;
 	 	}

 	 	strRightBrackets=strstr(au08ReadOneLineDataCache, "}");
		if(strRightBrackets!=NULL)
 	 	{
 	 		rightBracketsOffset=strRightBrackets-au08ReadOneLineDataCache; 	 		
 	 	}
 		 		
 		//3 get data, remove "/****/" or "//"
 		if(isLeftBracketsFound)
 		{		 	
		 	for(i=realDataStartOfs; i<u64OneLineCharactersLen; i++)
		 	{		 		 
				//if found '/*' , do not add the data, until next '*/' is coming					
				//if found '//', do not add the data, until this line is finish					
				//if found '\n\r' do not add as data					
				tempOneChar=au08ReadOneLineDataCache[i];
				
				if(tempOneChar==0x2F)
				{
					if(au08ReadOneLineDataCache[i+1]==0x2F)  //0x2F is '/'
							lineCommentIsComing=1;
					if(au08ReadOneLineDataCache[i+1]==0x2A)  //0x2A is '*'
					{
						blockCommentIsComing=1;
						countBlockComment=0;
					}
					
					isDataValid=0;
				}
				
				//if line comment is coming, give up this line
				if(lineCommentIsComing)
					break;
										
				if(blockCommentIsComing)
				{
					//for this counter '1' means find '/', so after '/' at least have two '*' characters.
					//so this counter at least should >=3, then we can search the end '/'.
					countBlockComment++;
						
					if(countBlockComment>3)
					{
						//block comment end found
						if(tempOneChar==0x2F)  //0x2F is '/'
						{
							isDataValid=1;
							blockCommentIsComing=0;		
							countBlockComment=0;
						}							
					}
				}// end if(blockCommentIsComing)
					
				//data is valid, we add all the data to final output buffer
				if(isDataValid)
				{						
					//0x0A is the ASCII code of '\n',  0x2F is '/'  0x20 is space
					//if(tempOneChar>0x2F)
					if(((tempOneChar>=0x30)&&(tempOneChar<=0x39))||((tempOneChar>=0x41)&&(tempOneChar<=0x46))||((tempOneChar>=0x61)&&(tempOneChar<=66))||(tempOneChar==0x58)||(tempOneChar==0x78))
					{
						//check if the data is 8 bits, eg. 0xAA, if found 0xA, chang to 0x0A							
						if(au08ReadOneLineDataCache[i]=='0')
						{
							//check if next character is 'X'
							if((au08ReadOneLineDataCache[i+1]=='x')||(au08ReadOneLineDataCache[i+1]=='X'))
							{
								if((au08ReadOneLineDataCache[i+3]==0x20)||(au08ReadOneLineDataCache[i+3]==',')||(au08ReadOneLineDataCache[i+3]==0x0A))
								{	
									u8HalfByteIsCome=1;		
									u8HalfByteCounter=0;				
								}										
							}								
						}
						
						u8HalfByteCounter++;
						if(u8HalfByteIsCome)
						{
							au08ReadTextData[u64FinalHexDataOfs++]=tempOneChar;
							i++;
							au08ReadTextData[u64FinalHexDataOfs++]=au08ReadOneLineDataCache[i]; //this is 'x'
							au08ReadTextData[u64FinalHexDataOfs++]='0'; //this is 'x'
							
							u8HalfByteIsCome=0;	
						}
						else{
							au08ReadTextData[u64FinalHexDataOfs++]=tempOneChar;		
						}
					}
				}
	 			
	 		}//end for			 	
	 	}//end if	 	
	}//end while
	
	return u64FinalHexDataOfs;	
}


int FileOperation::charToHex(char oneChar)
{
	int value=0;
	if ((oneChar >= '0') && (oneChar <= '9'))  
		value= oneChar - '0'; 
	
	if ((oneChar >= 'A') && (oneChar <= 'F')) 
		value = oneChar - 'A'+10; 
	
	if ((oneChar >= 'a') && (oneChar <= 'f')) 
		value = oneChar - 'a'+10; 

	return value;
}

/**
* change 0xAA to 
* @return output buffer data length
*/
int FileOperation::hexStringToRealHex(unsigned char * au08ReadTextData, long inputDataLen, unsigned char * outputBuf)
{
	unsigned char ucMsb4Bit=0;
	unsigned char ucLsb4Bit=0;
	int u32MsbValue=0;
	int u32LsbValue=0;
	int u32OneValue=0;	
	int i=0;
	int outputBufCounter=0;
	for(i=0;i<inputDataLen;i+=4)
	{		
		ucMsb4Bit=au08ReadTextData[i+2];
	    ucLsb4Bit=au08ReadTextData[i+3];		 				
		u32MsbValue=charToHex(ucMsb4Bit);
		u32LsbValue=charToHex(ucLsb4Bit);	 	
		u32OneValue=(u32MsbValue<<4);
		u32OneValue|=u32LsbValue;
		outputBuf[outputBufCounter++]=u32OneValue;
	}
	return outputBufCounter;
}

int FileOperation::getFileSize(char * fileDir)
{
	int fileLen=0;
	FILE *fp;
	if ((fp=fopen(fileDir,"rb"))!=NULL)
	{
		fseek(fp,0L,SEEK_END);   
		fileLen=ftell(fp);  
	}
	return fileLen;
}


/**
* this will append binary data to file, if file is exist, append to the end
*
*/
int FileOperation::writeToBinFile(char * fileName, unsigned char inputData[], int inputDataLen)
{
	FILE *file = fopen(fileName, "ab+");
	int i=0;
	if(file!=NULL)
	{
		for(i=0; i<inputDataLen; i++) 
		{	     
		    fwrite(inputData+i, sizeof(unsigned char), 1, file);
		 
	    }
	}
	fclose(file);	
	return 0;
}

char * FileOperation::getOutputBinName()
{
	return "output.bin";
}

char * FileOperation::getOutputLogName()
{
	return "log.txt";
}

bool FileOperation::removeOutputBinFile()
{
	remove(getOutputBinName());
	remove(getOutputLogName());
	return true;
}

int FileOperation::readTextWriteToBinMain(char * readFileName)
{
	try
	{  
		unsigned char * au08ReadTextData=new  unsigned char[115200*100]();  
		unsigned char * au08IntergerData=new  unsigned char[115200*100]();
		int readCacheLen=100000;  
	    char * au08ReadOneLineDataCache=new char[readCacheLen]();
		  
  		FileOperation arrayToBinObj;
  
		char * outBinFile=getOutputBinName();
		int hexDataLen=0;
		int u64FinalHexDataLen=0;
		 
		//1. read pure hex string 
		u64FinalHexDataLen= arrayToBinObj.readArrayFromTxt( readFileName, au08ReadOneLineDataCache ,readCacheLen, au08ReadTextData);
		//2. change the hex string to real hex
		hexDataLen=arrayToBinObj.hexStringToRealHex(au08ReadTextData, u64FinalHexDataLen, au08IntergerData);
		//3. write pure hex to a bin file
		arrayToBinObj.writeToBinFile(outBinFile, au08IntergerData, hexDataLen);

		delete [] au08ReadTextData;
		delete [] au08IntergerData;
		delete [] au08ReadOneLineDataCache;
	}
	catch(...)
	{
		 
	}
	
	return 0;
}


bool FileOperation::writeLogFile(char *logName, char * logData)
{
	FILE *file = fopen(logName, "a+");
    int inputDataLen=strlen(logData);
    int i=0;
 
	if(file!=NULL)
	{
		for(i=0; i<inputDataLen; i++) 
		{	     
		    fwrite(logData+i, sizeof(unsigned char), 1, file);		 
	    }
	}
	fclose(file);	
	return true;
}
 