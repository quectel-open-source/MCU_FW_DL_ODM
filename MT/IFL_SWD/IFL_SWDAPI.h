#ifndef IFL_SWDAPI_H_H_
#define IFL_SWDAPI_H_H_
#include "Macrodef.h"
#include "../WtptpDownLoad/ParaDefine.h"
#include "RndParadef.h"
#include "TimIncludedType.h"
#ifdef IFLSWD_STATIC
#include "BLFProperty.h"
#endif

#ifdef IFLSWD_DYNAMIC
#ifdef IFLSWD_EXPORTS
#define IFLSWD_FUNC extern "C" __declspec(dllexport)
#else
#define IFLSWD_FUNC extern "C" __declspec(dllimport)
#endif
#else
#define IFLSWD_FUNC
#endif


/*! @defgroup groupFunctions function list
* @{
*/
/// =========================================
/// @fn			ReadBLF(PInstanceParams PInstParam,char * pszBLFfileName)
/// @brief		Read and Parse blf file and generate temp folder for download
/// @param[in]	pszBLFfileName 
/// @param[out] PInstParam including download file list and DKB files name and other configurations.                                                                 
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		This function is used in user only provide separated images to download\n
///
/// =========================================
IFLSWD_FUNC bool ReadBLF(PInstanceParams pInstParam,const TCHAR * pszBLFfileName);

/*! @defgroup groupFunctions function list
* @{
*/
/// =========================================
/// @fn			PrepareUpload(PInstanceParams PInstParam,char * pszUploadCfgfileName)
/// @brief		Read and Parse upload configuration file and do preparations
/// @param[in]	pszBLFfileName 
/// @param[out] PInstParam including download file list and DKB files name and other configurations.                                                                 
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		This function is used to generate a Realiable data format file\n
///
/// =========================================
IFLSWD_FUNC bool PrepareUpload(PInstanceParams PInstParam,const TCHAR * pszUploadCfgfileName);
//#if !defined(LINUX)
/// =========================================
/// @fn			RNDBuilder(RNDEntryParams* pRNDEnParas, char *pszRNDfileName)
/// @brief		Generate a Reliable Data file 
/// @param[in]	pszRNDfileName 
/// @param[out] pRNDEnParas.                                                           
/// @details	User need to input files and enry type to pRNDEnParas 
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		please see detail of structure of pRNDEnParas comments of each parameter\n 
///
/// =========================================
IFLSWD_FUNC bool  RNDBuilder(RNDEntryParams* pRNDEnParas, const TCHAR *pszRNDfileName);

//#endif

IFLSWD_FUNC bool RNDParser(const TCHAR *pszRNDfileName, const TCHAR *pszDestDir, list<FLASH_ENTRY_INFO>& flashEntryInfo);

/// =========================================
/// @fn			RRNDParser(const char *pszRNDfileName, FLASH_RD_PARSE* const pFlashRdParse)
/// @brief		Generate some bin files through parsing a Reliable Data file
/// @param[in]	pszRNDfileName 
/// @param[in]  pFlashRdParse                                                           
/// @details	User need to input  Reliable Data file name and structor FLASH_RD_PARSE type 
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		please see detail of structure of FLASH_RD_PARSE comments of each parameter\n 
///
/// =========================================
//bool RNDParser(const char *pszRNDfileName, FLASH_RD_PARSE* const pFlashRdParse);

/// =========================================
/// @fn			SetMode(unsigned int Command)
/// @brief		Set User command
/// @param[in]	Command    
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		This function is for reserved use\n
/// =========================================
IFLSWD_FUNC bool  SetMode(unsigned int Command);


/// =========================================
/// @fn			PrepareDownloadWithFBF(PInstanceParams PInstParam,char * pszBLFfileName ,char* pszFlasherfileName,char *pszFBFfileName)
/// @brief		Parse blf file and generate temp folder for download
/// @param[in]	pszBLFfileName 
/// @param[in]	pszFBFfileName 
/// @param[out] PInstParam including download file list and DKB files name and other configurations.     
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		This function is used in user only provide a prepared FBF file in production,\n
///				This API is better in production line,but user need to prepare FBF file first\n
///				How to Generate FBF file , there are two steps: \n
///				1, run NtimBuild.exe to generate a tim header file;\n
///				2, run FBF_Make.exe to generate a FBF file.\n
///
/// =========================================
IFLSWD_FUNC  bool PrepareDownloadWithFBF(PInstanceParams pInstParam,const TCHAR * pszBLFfileName ,const TCHAR* pszFlasherfileName,const TCHAR *pszFBFfileName);

/// =========================================
/// @fn			GetBlfInfo(char * pszBLFfileName,BlfInfoProperty* pBLFInfo)
/// @brief		Parse blf file and generate temp folder for download
/// @param[in]	pszBLFfileName 
/// @param[in]	pszFlasherfileName 
/// @param[in]	pszFBFfileName 
/// @param[out] pBLFInfo including all information of blf file.                                                                 
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		This function is used in user need to do modifications of configurations in blf file in their UI\n
///				This API is NOT recommend in production line tool, it is better use in development phase's tool\n
///				because it is too flexible \n 
///
/// =========================================

#ifdef IFLSWD_STATIC
bool  GetBlfInfo(const TCHAR * pszBLFfileName,BlfInfoProperty* pBLFInfo);
#endif

/// =========================================
/// @fn			SetBlfInfo(char * pszBLFfileName,BlfInfoProperty* pBLFInfo)
/// @brief		Parse blf file and generate temp folder for download
/// @param[in]	pszBLFfileName
/// @param[out] pBLFInfo including all information of blf file.                                                             
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		This function is used in user have Modified configurations of blf file in their UI \n
///				and need to save those changes into blf file for future usage \n
///				This API is NOT recommend in production line tool, it is better use in development phase's tool\n
///				because it is too flexible \n
///
/// =========================================
#ifdef IFLSWD_STATIC
bool SetBlfInfo(const TCHAR * pszBLFfileName,BlfInfoProperty* pBLFInfo);
#endif
/// =========================================
/// @fn			DoFBFMakeforSplitImage(const char * pszBLFfileName,const char * pszSplitSize)
/// @brief		Parse blf file and make a special FBF file as user request that split an image to 
///             several images 
/// @param[in]	pszBLFfileName 
/// @param[in]	pszSplitSize 
/// @param[out] fbf.bin file.                                                             
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		This function is used for speical case to split an large image to serveral images by splitsize user set and 
///				wrap all images into a fbf file \n
///
/// =========================================
IFLSWD_FUNC bool DoFBFMakeforSplitImage(const TCHAR * pszBLFfileName,const TCHAR * pszSplitSize);

/// =========================================
/// @fn			NTimBuilder(const char * pszBLFfileName)
/// @brief		Parse blf file and generate a tim file as the name described in blf file 
///             it will be have the same path as blf file
/// @param[in]	pszBLFfileName 
/// @param[out] pszTimFolder.                                                             
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		this function is used to generate a tim/ntim file  \n
///
/// =========================================
IFLSWD_FUNC bool NTimBuilder(const TCHAR * pszBLFfileName, const TCHAR* pszTimFolder, bool bMergeOBM2Tim, bool bAllEnable=false);

/// =========================================
/// @fn			FBFMake(const char * pszBLFfileName,const char * pszFBFFolder)
/// @brief		Parse blf file and generate fbf files in pszFBFFolder
/// @param[in]	pszBLFfileName 
/// @param[out] pszFBFFolder.                                                             
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		this function is used to generate a FBF format file for preparedownloadwithFBF \n
///
/// =========================================
IFLSWD_FUNC bool  FBFMake(const TCHAR * pszBLFfileName,const TCHAR * pszFBFFolder);

/// =========================================
/// @fn			PInstanceParams IFLSWD_FUNC GetInstanceParams()
/// @brief		Malloc memory for  PInstanceParams to let user use
///             it will return a pointer
/// @param[in]	NULL 
/// @param[out] PInstanceParams.                                                             
/// @return		bool.
///				@retval non NULL : Successfully .  
///				@retval NULL : Failed .
/// @note		this function is used to new a PInstanceParams to save parameters for use \n
///
/// =========================================
IFLSWD_FUNC  PInstanceParams MallocInstanceParams();



/// =========================================
/// @fn			bool MakeDownloadPackage(const TCHAR* pszBlfFileName,const TCHAR* pszDownloadPackageName)
/// @brief		Parse blf file, process images and parse blf file and Prepare download zip package
///             
/// @param[in]	pszBlfFileName 
/// @param[in]	pszDownloadPackageName 
/// @param[out] null.                                                             
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		
///
/// =========================================
IFLSWD_FUNC bool MakeDownloadPackage(const TCHAR* pszBlfFileName,const TCHAR* pszDownloadPackageName);

/// =========================================
/// @fn			 bool DownloadWithPackage(const TCHAR* pszDownloadPackagePath,PInstanceParams pInstParam)
/// @brief		 initial PInstanceParams from Download Zip Package
///
/// @param[in]	pszDownloadPackagePath 
/// @param[out] pInstParam.                                                             
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		
///
/// =========================================

IFLSWD_FUNC bool DownloadWithPackage(const TCHAR* pszDownloadPackagePath,PInstanceParams pInstParam);

/// =========================================
/// @fn			bool MakeDownloadPackageWithAllExistImages(const TCHAR* pszBlfFileName)
/// @brief		Parse blf file, process images and parse blf file and Prepare download zip package
///  
/// @param[in]	pszBlfFileName 
/// @param[out] null.                                                             
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		
///
/// =========================================
IFLSWD_FUNC bool MakeDownloadPackageWithAllExistImages(const TCHAR* pszBlfFileName,
												  const TCHAR* pszDKBTim,
												  const TCHAR* pszFBFTim,
												  const TCHAR* pszFBFfolder,
												  const TCHAR* pszPackageFile);

/// =========================================
/// @fn			 bool GenerateTimNoDSandTimHashFiles(const TCHAR* pszBlfFile,const TCHAR* pszpublickKeyBlfFile,const TCHAR* pszTimNoDSFile,const TCHAR* pszTimHashFile,TIMINCLUDEDTYPE_T  eTimType,const TCHAR* pszDtimPublicKeyRsaSysModulusDSFile,const TCHAR* pszDtimPublicKeyRsaExponentDSFile)
/// @brief		 Generate TimNoDS.bin and TimHash.bin
///
/// @param[in]	pszBlfFile 
/// @param[in]	pszpublickKeyBlfFile 
/// @param[in]	eTimType 
/// @param[in]	pszDtimPublicKeyRsaSysModulusDSFile 
/// @param[in]	pszDtimPublicKeyRsaExponentDSFile 
/// @param[out] pszTimNoDSFile.  
/// @param[out] pszTimHashFile.  
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		
///
/// =========================================


IFLSWD_FUNC bool GenerateTimNoDSandTimHashFiles(const TCHAR* pszBlfFile,
												const TCHAR* pszpublickKeyFile,
												const TCHAR* pszDtimPublicKeyRsaSysModulusDSFile,
												const TCHAR* pszDtimPublicKeyRsaExponentDSFile,
												TIMINCLUDEDTYPE_T  eTimType,
												const TCHAR* pszTimNoDSFile,
												bool bIsHSMSignature);

/// =========================================
/// @fn			 bool BuildTimWithTimNoDSandSignedFile(const TCHAR* pszBlfFile,const TCHAR* pszTimNoDSFile,const TCHAR* pszSignedFile,TIMINCLUDEDTYPE_T  eTimType,bool bIsHSMSignature,const TCHAR* pszTIMHeaderFile)
/// @brief		 Generate a completed Tim/DTim with signed file and TimNoDS File
///
/// @param[in]	pszBlfFile 
/// @param[in]	pszTimNoDSFile 
/// @param[in]	pszSignedFile 
/// @param[in]	eTimType 
/// @param[in]	bIsHSMSignature 
/// @param[in]	pszTIMHeaderFile ,need input pszTIMHeaderFile if generate DKBtim nad FbfTim
/// @param[out] Tim/Dtim file name in SWDownloader display.   
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		
///
/// =========================================

IFLSWD_FUNC bool BuildTimWithTimNoDSandSignedFile(const TCHAR* pszBlfFile,
													const TCHAR* pszTimNoDSFile,
													const TCHAR* pszSignedFile,
													TIMINCLUDEDTYPE_T eTimType,
													bool bIsHSMSignature,
													const TCHAR* pszTIMHeaderFile);
/// =========================================
/// @fn			 bool GenerateENCKHashNoDS(const TCHAR* pszBlfFile,const TCHAR* pszDTimPublicKeyFile,const TCHAR* pszRsaSysModulusFile,const TCHAR* pszRsaSysExponentFile)
/// @brief		 Generate a completed Tim/DTim with signed file and TimNoDS File
///
/// @param[in]	pszBlfFile 
/// @param[in]	pszDTimPublicKeyFile 
/// @param[in]	pszRsaSysModulusFile 
/// @param[out]	pszRsaSysExponentFile 
/// @param[out]	bIsHSMSignature    
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		
///
/// =========================================


IFLSWD_FUNC bool GenerateDtimPublicKeyCompFile(const TCHAR* pszBlfFile,
									  const TCHAR* pszDTimPublicKeyFile,
									  const TCHAR* pszRsaSysModulusFile,
									  const TCHAR* pszRsaSysExponentFile);

/// =========================================
/// @fn			 GenerateDKBTimNoDSandTimHashFiles(const TCHAR* pszBlfFile,const TCHAR* pszPublickKeyFile,const TCHAR* pszTimNoDSFile,const TCHAR* pszTimHashFile)
/// @brief		 Generate DKB TimNoDS.bin and TimHash.bin and DKB.bin
///
/// @param[in]	pszBlfFile 
/// @param[in]	pszDTimPublicKeyFile 
/// @param[out]	pszTimNoDSFile 
/// @param[out]	pszTimHashFile

/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		
///
/// =========================================
										
IFLSWD_FUNC bool GenerateDKBTimNoDSandTimHashFiles(const TCHAR* pszBlfFile,
												   const TCHAR* pszPublickKeyFile,
												   const TCHAR* pszTimNoDSFile);


/// =========================================
/// @fn			 GenerateFBFTimNoDSandTimHashFiles(const TCHAR* pszBlfFile,const TCHAR* pszPublickKeyFile,const TCHAR* pszTimNoDSFile,const TCHAR* pszTimHashFile,const TCHAR* pszFBFFilesFolder)
/// @brief		 Generate FBF TimNoDS.bin and TimHash.bin and create all FBF_h.bin files in the specified folder pszFBFFilesFolder
///
/// @param[in]	pszBlfFile 
/// @param[in]	pszDTimPublicKeyFile 
/// @param[out]	pszTimNoDSFile 
/// @param[out]	pszTimHashFile 
/// @param[in]	pszFBFFilesFolder    
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		
///
/// =========================================
IFLSWD_FUNC bool GenerateFBFTimNoDSandTimHashFiles(const TCHAR* pszBlfFile,
												   const TCHAR* pszPublickKeyFile,
												   const TCHAR* pszTimNoDSFile,
												   const TCHAR* pszFBFFilesFolder);

/// =========================================
/// @fn			bool IFLSWD_FUNC FreeInstanceParams(PInstanceParams pInstParam)
/// @brief		Free memory for  PInstanceParams to let user use
/// @param[in]	NULL 
/// @param[out] PInstanceParams.                                                             
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// @note		this function is used to free a PInstanceParams to save parameters for use \n
///
/// =========================================
IFLSWD_FUNC bool FreeInstanceParams(PInstanceParams pInstParam);


IFLSWD_FUNC bool PrepareJtagImages(const tstring strBLFfileName,tstring& strTimHeader,tstring& strDKBFile,tstring& strTZIIFile);


/// =========================================
/// @fn			GetEraseFlashStatus()
/// @brief		Get EraseAllFlash Status 
/// @param[in]	PInstParam including download file list and DKB files name and other configurations.
/// @return		bool.
///				@retval true : erase all flash .  
///				@retval false: don't erase all flash .
/// =========================================
IFLSWD_FUNC bool GetEraseFlashStatus(PInstanceParams pInstParam);


/// =========================================
/// @fn			SetEraseFlashStatus()
/// @brief		erase all flash if bEraseAllFlash is true, or don't erase flash  
/// @param[in]	PInstParam including download file list and DKB files name and other configurations.
/// @param[in]	bEraseAllFlash to specify erase all flash status.
/// @return		bool.
///				@retval true : Successfully .  
///				@retval false : Failed .
/// =========================================
IFLSWD_FUNC bool SetEraseFlashStatus(PInstanceParams pInstParam, bool bEraseAllFlash);

///@}		//@defgroup groupFunctions function list


//
//#ifdef  __cplusplus
//}
//#endif

#endif

