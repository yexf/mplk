#include "pch.h"
#include "wxf_setting.h"
wxf_setting wxf_setting::g_setting;
bool wxf_setting::LoadSetting()
{
	wxf_str strSettingPath = m_oRootDir + m_oFileName; 
	if (!win_fexist(strSettingPath.c_str())) 
	{
		return false;
	}

	wxf_doc oDoc;
	oDoc.LoadFile(strSettingPath.c_str());

	wxf_node *pRoot = oDoc.FirstChild("setting");
	wxf_node *pFilePath = pRoot->FirstChild("playlist");
	wxf_node *pPlayNo = pRoot->FirstChild("playno");

	wxf_node *pLogName = pRoot->FirstChild("logname");
	wxf_node *pLogLevel = pRoot->FirstChild("loglevel");
	wxf_node *pLogDir = pRoot->FirstChild("logdir");

	m_oPlayListPath = pFilePath->FirstChild()->Value();
	m_iPlayNo = pPlayNo->FirstChild()->Value();

	m_oLogName = pLogName->FirstChild()->Value();
	m_oLogLevel = pLogLevel->FirstChild()->Value();
	m_oLogDir = pLogDir->FirstChild()->Value();

	return true;
}

bool wxf_setting::SaveSetting()
{
	if (!wxf_fexist(m_oRootDir.c_str()))
	{
		CreateSavePath(m_oRootDir.data());
	}

	wxf_doc oDoc;
	wxf_ele oRoot("setting");
	wxf_ele	oTemp("");
	wxf_text *oText = new wxf_text("");

	oTemp.LinkEndChild(oText);

	oTemp.SetValue("playlist");
	oText->SetValue(m_oPlayListPath.c_str());
	oRoot.InsertEndChild(oTemp);

	oText->SetValue(m_iPlayNo.c_str());
	oTemp.SetValue("playno");
	oRoot.InsertEndChild(oTemp);

	oText->SetValue(m_oLogName.c_str());
	oTemp.SetValue("logname");
	oRoot.InsertEndChild(oTemp);

	oText->SetValue(m_oLogLevel.c_str());
	oTemp.SetValue("loglevel");
	oRoot.InsertEndChild(oTemp);

	oText->SetValue(m_oLogDir.c_str());
	oTemp.SetValue("logdir");
	oRoot.InsertEndChild(oTemp);

	oDoc.InsertEndChild(oRoot);

	return oDoc.SaveFile(GetSettingFile().c_str());
	
}

wxf_str & wxf_setting::GetPlayList()
{
	return m_oPlayListPath;
}

void wxf_setting::SetPlayList( wxf_str &strPlayList )
{
	m_oPlayListPath = strPlayList;
}

void wxf_setting::SetRootDir( wxf_str &strRootPath )
{
	m_oRootDir.assign_addend(strRootPath.c_str(),'\\');
	wxf_str FileName = GetFileNameFromFullFileName(m_oPlayListPath.c_str()).c_str();
	
	m_oPlayListPath = m_oRootDir + FileName;

	m_oLogDir = m_oRootDir;
}

wxf_str &wxf_setting::GetRootDir()
{
	return m_oRootDir;
}

wxf_str wxf_setting::GetSettingFile()
{
	return m_oRootDir + m_oFileName; 
}

void wxf_setting::SetSettingFile( wxf_str &strSettingFile )
{
	m_oFileName = strSettingFile;
}
