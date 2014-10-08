#pragma once
#include <vector>
#include <map>

#include ".\tinyxml\tinyxml.h"

typedef TiXmlDocument		wxf_doc;		//�ĵ�
typedef TiXmlElement		wxf_ele;		//Ԫ��
typedef TiXmlComment		wxf_comment;	//ע��
typedef TiXmlUnknown		wxf_unknown;	//δ֪
typedef TiXmlAttribute		wxf_attr;		//����
typedef TiXmlText			wxf_text;		//�ı�
typedef TiXmlDeclaration	wxf_decl;		//�ĵ�����
typedef TiXmlNode			wxf_node;		//�ڵ�
typedef TiXmlNode::NodeType wxf_node_type;	//�ڵ�����

class wxf_setting
{
private:
	wxf_setting()
		:m_strVersion("0.01"),
		m_oFileName("mp_kuwo_setting.xml"),m_oRootDir(".\\"),
		m_oPlayListPath("playlist.txt"),m_iPlayNo(0),
		m_oLogDir(".\\"),m_oLogName("kuwo.log"),m_oLogLevel("3"),
		m_oLoopMode("0")
	{
		
	}
	~wxf_setting() 
	{

	}
public:
	void SetRootDir(wxf_str &strRootPath);
	wxf_str &GetRootDir();
	
	wxf_str GetSettingFile();
	void SetSettingFile(wxf_str &strSettingFile);

	bool LoadSetting();
	bool SaveSetting();

	wxf_str &GetPlayList();
	void SetPlayList(wxf_str &strPlayList);

	int GetLoopMode();
	void SetLoopMode(int mode);
public:
	static wxf_setting &get_instance() {return g_setting;}
	static const wxf_setting &get_const_instance() {return g_setting;}
public:
	static wxf_setting g_setting;

	wxf_str m_oRootDir;

	wxf_str m_oFileName;	
	wxf_str m_strVersion;

	wxf_str m_oPlayListPath;
	wxf_str m_iPlayNo;

	wxf_str m_oLogName;
	wxf_str m_oLogLevel;
	wxf_str m_oLogDir;

	wxf_str m_oLoopMode;
};