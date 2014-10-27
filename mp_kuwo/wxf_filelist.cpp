#include "wxf_app.h"
#include "wxf_filelist.h"
//////////////////////////////////////////////////////////////////////////
//
//


bool wxf_filelist::LoadList( TiXmlElement *pFileList )
{
	if (pFileList->Attribute("isfav"))
	{
		std::string strIsLike = pFileList->Attribute("isfav");
		if (strIsLike == "true")
		{
			m_bFav = true;
		}
	}
	else
	{
		m_bFav = false;
	}

	TiXmlElement *pItem = pFileList->FirstChildElement("item");
	while(pItem != NULL)
	{
		wxf_fileitem oFileItem;

		oFileItem.m_strFilePath = pItem->FirstChild()->Value();
		if (pItem->Attribute("islike"))
		{
			std::string strIsLike = pItem->Attribute("islike");
			if (strIsLike == "true")
			{
				oFileItem.m_bIsLikeMusic = true;
			}
		}
		pItem = pItem->NextSiblingElement("item");

		m_vecFileItem.push_back(oFileItem);
	}
	return true;
}

bool wxf_filelist::SaveList( TiXmlElement *pFileList )
{
	for (int i = 0;i < m_vecFileItem.size();i++)
	{
		TiXmlElement *pItem = new TiXmlElement("item");

		if (m_vecFileItem[i].m_bIsLikeMusic)
		{
			pItem->SetAttribute("islike","true");
		}

		pItem->InsertEndChild(TiXmlText(m_vecFileItem[i].m_strFilePath.c_str()));

		pFileList->LinkEndChild(pItem);
	}

	if (m_bFav)
	{
		pFileList->SetAttribute("isfav","true");
	}
	return true;
}


bool wxf_filemap::LoadFile( const char *pstrFilePath )
{
	TiXmlDocument oXml;

	if (!oXml.LoadFile(pstrFilePath)) return false;

	TiXmlElement *pRoot = oXml.RootElement();

	if (pRoot == NULL)
	{
		return false;
	}

	if (pRoot->Value() != std::string("playlist"))
	{
		return false;
	}

	TiXmlElement *pFileList = pRoot->FirstChildElement();
	while (pFileList)
	{
		std::string strListName = pFileList->Value();

		wxf_filelist &filelist = m_mapFileMap[strListName];
		filelist.LoadList(pFileList);

		pFileList = pFileList->NextSiblingElement();
	}

	return true;
}

bool wxf_filemap::SaveFile( const char *pstrFilePath )
{
	TiXmlDocument oXml;

	TiXmlElement *pRoot = new TiXmlElement("playlist");

	std::map<std::string,wxf_filelist>::iterator iteFileMap = m_mapFileMap.begin();

	while(iteFileMap != m_mapFileMap.end())
	{
		TiXmlElement *pItem = new TiXmlElement(iteFileMap->first.c_str());
		iteFileMap->second.SaveList(pItem);

		pRoot->LinkEndChild(pItem);

		iteFileMap++;
	}

	oXml.LinkEndChild(pRoot);
	oXml.SaveFile(pstrFilePath);
	return true;
}	

std::vector<wxf_fileitem> & wxf_filemap::GetFileList( const char *pstrTitle /*= "default"*/ )
{
	return m_mapFileMap[std::string(pstrTitle)].GetList();
}

void wxf_filemap::SetFileList( std::vector<wxf_fileitem> &vecFileLit,const char *pstrTitle /*= "default"*/ )
{
	m_mapFileMap[std::string(pstrTitle)].SetList(vecFileLit);
}

bool wxf_filemap::GetFav( const char *pstrTitle /*= "default"*/ )
{
	return m_mapFileMap[std::string(pstrTitle)].GetFav();
}

void wxf_filemap::SetFav( bool bFav,const char *pstrTitle /*= "default"*/ )
{
	return m_mapFileMap[std::string(pstrTitle)].SetFav(bFav);
}
