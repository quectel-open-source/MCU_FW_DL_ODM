#pragma once

class CIniFile
{
public:
	CIniFile(void);
	~CIniFile(void);
	void StoreStr(const wchar_t * SectionName, const wchar_t * KeyName, const wchar_t * csV);
	CString m_csIniFileName;
	void StoreInt(const wchar_t * SectionName, const wchar_t * keyName, int iV);
	void StoreD(const wchar_t * SectionName, const wchar_t * KeyName, double dV);
	void StoreB(const wchar_t * SectionName, const wchar_t * KeyName, bool bV);
	CString ReadStr(const wchar_t * SectionName, const wchar_t * KeyName, const wchar_t * csDV);
	int ReadInt(const wchar_t * SectionName, const wchar_t * KeyName, int iDV);
	double ReadD(const wchar_t * SectionName, const wchar_t * KeyName, double dDV);
	bool ReadB(const wchar_t * SectionName, const wchar_t * KeyName, bool bDV);
};
