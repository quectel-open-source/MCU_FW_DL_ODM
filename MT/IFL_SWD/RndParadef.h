#pragma once
#include <string>
#include <list>
using namespace std;
#include "Macrodef.h"
#define  MAXTYPENUMBER          100

typedef  list<tstring>::iterator FileIter;
typedef unsigned int        	UINT32,   *PUINT32;
typedef struct _EntryType
{
	UINT32           nEntryFileNumber; //Count of same entry type bin
	UINT32          EntryTypeID;      //TypeID (HexFormat)
	list<tstring>    FileList;         //Filename list in reliable data 

	_EntryType() :nEntryFileNumber(0),EntryTypeID(0){}

}EntryType;



typedef struct _RNDEntryParams
{
	UINT32     nEntryTypeNumber;	//Count of Entry Type Numbers in ReliableData.bin
	EntryType       TypeDetails[MAXTYPENUMBER]; 

	_RNDEntryParams(): nEntryTypeNumber(0){}


}RNDEntryParams,*PRNDEntryParams;





typedef struct tag_flash_entry_info
{	
	enum{
		FILE_NAME_LEN = 116,
	};
	unsigned long		entryType; 
	unsigned char		fileName[FILE_NAME_LEN];
	unsigned long		entryVersion;
	unsigned long		entryDate;	
	unsigned long		entryPayload;	
} FLASH_ENTRY_INFO;



