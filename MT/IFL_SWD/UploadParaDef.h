#pragma once
#include <list>
#include "Macrodef.h"
typedef unsigned char BYTE;
using namespace std;


#pragma pack(push,1)
typedef struct _UPLOAD_DATA_PARAMS
{
	BYTE Type;
	BYTE SubType;
	BYTE Partition;
	BYTE Times;// Total Number of upload Areas
	unsigned int Offset;
	unsigned int DataSize;
	BYTE IncludeSpareAreas; // indicate this upload include spare areas or not
	BYTE ResetUE;
	BYTE ResetTimeDelay;
	BYTE Reserved2; // 用来存放DataSize的高32位，支持一次性upload大于4GB的内容
	void operator = (struct _UPLOAD_DATA_PARAMS& UpLoadDataPara)
	{
		Type = UpLoadDataPara.Type;
		SubType = UpLoadDataPara.SubType;
		Partition = UpLoadDataPara.Partition;
		Times = UpLoadDataPara.Times;
		Offset = UpLoadDataPara.Offset;
		DataSize = UpLoadDataPara.DataSize;
		IncludeSpareAreas = UpLoadDataPara.IncludeSpareAreas; // indicate this upload include spare areas or not
		ResetUE = UpLoadDataPara.ResetUE;
		ResetTimeDelay = UpLoadDataPara.ResetTimeDelay;
		Reserved2 = UpLoadDataPara.Reserved2;
	}
	_UPLOAD_DATA_PARAMS(){Reserved2 = 0;}
} UPLOAD_DATA_PARAMS; 
#pragma pack(pop)


#pragma pack(push,1)
typedef struct _UPLOAD_DATA_SPEC
{
	UPLOAD_DATA_PARAMS m_Params;
	tstring    strOutputFileName;
	tstring    strComparisonFileName;
	bool	   bCreateNewFile;
	void operator = (struct _UPLOAD_DATA_SPEC& UpLoadDataSpec)
	{  
		m_Params = UpLoadDataSpec.m_Params;
		strOutputFileName = UpLoadDataSpec.strOutputFileName;
		strComparisonFileName = UpLoadDataSpec.strComparisonFileName;
		bCreateNewFile = UpLoadDataSpec.bCreateNewFile;
	}
} UPLOAD_DATA_SPEC,*pUPLOAD_DATA_SPEC; 
#pragma pack(pop)









typedef list<UPLOAD_DATA_SPEC*> t_UpLoadDataSpecList;
typedef list<UPLOAD_DATA_SPEC*>::iterator t_UpLoadDataSpecIter;