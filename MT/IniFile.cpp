#include "StdAfx.h"
#include "IniFile.h"

CIniFile::CIniFile(void)
: m_csIniFileName(_T(""))
{
}

CIniFile::~CIniFile(void)
{
}

void CIniFile::StoreStr(const wchar_t * SectionName, const wchar_t * KeyName, const wchar_t * csV)
{
	WritePrivateProfileString(SectionName, KeyName, csV, this->m_csIniFileName); 
}

void CIniFile::StoreInt(const wchar_t * SectionName, const wchar_t * keyName, int iV)
{
	CString csTemp;
	csTemp.Format(_T("%d"), iV);

	WritePrivateProfileString(SectionName, keyName, csTemp, this->m_csIniFileName);
}

void CIniFile::StoreD(const wchar_t * SectionName, const wchar_t * KeyName, double dV)
{
	CString csTemp;
	csTemp.Format(_T("%.5f"), dV);
	
	WritePrivateProfileString(SectionName, KeyName, csTemp, this->m_csIniFileName);
}

void CIniFile::StoreB(const wchar_t * SectionName, const wchar_t * KeyName, bool bV)
{
	if(bV)
	{
		WritePrivateProfileString(SectionName, KeyName, _T("1"), this->m_csIniFileName);
	}
	else
	{
		WritePrivateProfileString(SectionName, KeyName, _T("0"), this->m_csIniFileName);
	}
}

CString CIniFile::ReadStr(const wchar_t * SectionName, const wchar_t * KeyName, const wchar_t * csDV)
{
	CString csTemp;
	wchar_t * wcpBuff = NULL;

	//分配内存
	wcpBuff = (wchar_t *)calloc(4100, sizeof(wchar_t));//分配后sizeof(wcpBuff) = 4的大小本身没有变化
	
	GetPrivateProfileString(SectionName, KeyName, csDV, wcpBuff, 4096, this->m_csIniFileName);//sizeof(wcpBuff) = 4

	csTemp.Format(_T("%s"), wcpBuff);

	//释放内存
	if(wcpBuff != NULL)
	{
		free(wcpBuff);
	}

	return csTemp;
}

int CIniFile::ReadInt(const wchar_t * SectionName, const wchar_t * KeyName, int iDV)
{
	int i;
	i = GetPrivateProfileInt(SectionName, KeyName, iDV, this->m_csIniFileName);
	return i;
}

double CIniFile::ReadD(const wchar_t * SectionName, const wchar_t * KeyName, double dDV)
{
	wchar_t wcrData[64];
	CString csTemp;

	memset(wcrData, 0, sizeof(wcrData));

	csTemp.Format(_T("%.5f"), dDV);

	GetPrivateProfileString(SectionName, KeyName, csTemp, wcrData, sizeof(wcrData), this->m_csIniFileName);

	double dt = _wtof(wcrData);
	return dt;
}

bool CIniFile::ReadB(const wchar_t * SectionName, const wchar_t * KeyName, bool bDV)
{
	int a, i;

	a = (BOOL)bDV;

	i = GetPrivateProfileInt(SectionName, KeyName, a, this->m_csIniFileName);
	if(i > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
