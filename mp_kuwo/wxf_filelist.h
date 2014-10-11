#pragma once

#include <vector>
#include <string>
#include <map>

#include "./tinyxml/tinyxml.h"


struct wxf_fileitem
{
	std::string m_strFilePath;
	bool m_bIsLikeMusic;
	wxf_fileitem():m_strFilePath(""),m_bIsLikeMusic(false)
	{
	}
	wxf_fileitem(std::string strFilePath,bool bIsLikeMusic = false)
		:m_strFilePath(strFilePath),m_bIsLikeMusic(bIsLikeMusic)
	{
	}
};
typedef std::vector<wxf_fileitem> wxf_fileitemvec;


class wxf_filelist
{
public:
	wxf_filelist() : m_bFav(false)
	{

	}
public:
	bool LoadList(TiXmlElement *pFileList);
	bool SaveList(TiXmlElement *pFileList);

	std::vector<wxf_fileitem> &GetList(){return m_vecFileItem;}
	void SetList(std::vector<wxf_fileitem> &vecFileList){m_vecFileItem = vecFileList;}

	bool GetFav() {return m_bFav;}
	void SetFav(bool bFav) {m_bFav = bFav;}
private:
	std::vector<wxf_fileitem> m_vecFileItem;
	bool m_bFav;
};

class wxf_filemap
{
public:
	bool LoadFile(const char *pstrFilePath);
	bool SaveFile(const char *pstrFilePath);
	
	bool GetFav(const char *pstrTitle = "default");
	void SetFav(bool bFav,const char *pstrTitle = "default");

	wxf_fileitemvec &GetFileList(const char *pstrTitle = "default");
	void SetFileList(wxf_fileitemvec &vecFileLit,const char *pstrTitle = "default");
private:
	std::map<std::string,wxf_filelist> m_mapFileMap;
};