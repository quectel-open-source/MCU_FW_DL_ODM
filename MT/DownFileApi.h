#ifndef DOWNFILE_H
#define DOWNFILE_H
using namespace std;
#ifdef DOWNFILE_EXPORTS
#define DOWNFILE_API extern "C" __declspec(dllexport)
#else
#define DOWNFILE_API extern "C" __declspec(dllimport)
#endif

typedef enum
{
        DOWN_OK,//下载成功
        DOWN_FILE_ERR,//下载文件，打开失败
        DOWN_PORT_ERR,//串口打开失败
        DOWN_HANDLE_ERR,//文件句柄错误
        DOWN_START,//正在开始下载
        DOWN_STOP,//正在停止下载
        DOWN_ERR,//下载错误
}ERR_CODE;

typedef enum
{
	POS_SIZE_ALL,//set all file number
	POS_SIZE_ONE,//set one file size
	POS_ONE,//set one file pos
	POS_ALL,//set all file pos
	POS_FINISH,
}POS_EM;
//down file pos information, first param : total position, second param :one file position,third param :parent id.
typedef int(*GetDownPos_)(int, int, int);

//get down file log, first param; log, second param parent id.
typedef int(*GetLog_)(char*, int);

//get down status, return ERR_CODE
typedef int(*GetErr_)(int, int);

//return downfile id, -1 error
DOWNFILE_API int InitDown();
DOWNFILE_API int ReleaseDown(int nHandleIdx);
DOWNFILE_API int StartDown(char * pDownFile, int nPort, int nBaudrate, bool bCyc, int nHandleIdx);
DOWNFILE_API int StopDown(int nHandleIdx);
DOWNFILE_API int SetCallBack(GetDownPos_ getDownPos, GetLog_ getLog, GetErr_ getErr, int nHandleIdx);

#endif
