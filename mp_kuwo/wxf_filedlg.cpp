#include "pch.h"
#include <commdlg.h>
#include <Shlobj.h>
bool DoFileDIRDlg(HWND hwnd,wxf_str &dirname)
{
	BROWSEINFO bi;

	char szFile[MAX_PATH];
	ZeroMemory(&bi,sizeof(bi));
	bi.hwndOwner=hwnd;
	bi.pidlRoot=NULL;  
	bi.pszDisplayName=szFile;     
	bi.lpszTitle=TEXT("打开文件夹");     
	bi.lpfn=NULL;
	bi.ulFlags=/*BIF_BROWSEINCLUDEFILES|*/BIF_EDITBOX |BIF_RETURNONLYFSDIRS ;

	LPITEMIDLIST pItenLDList = SHBrowseForFolder(&bi); 

	if(pItenLDList)
	{
		if (SHGetPathFromIDList(pItenLDList,szFile))
		{
			dirname = szFile;
			return true;
		}
		else
		{
			dirname.empty();
			return false;
		}
	}
	return false;
}
bool DoFileDlg(HWND hwnd,wxf_str &filename)
{
	OPENFILENAME ofn;

	char szFile[MAX_PATH];
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = TEXT('\0');
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = TEXT("ALL\0*.*\0Text\0*.TXT\0");
	ofn.lpstrFilter = TEXT("MP3\0*.mp3\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.hwndOwner = hwnd;
	ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		filename = szFile;
		return true;
	}
	else
	{
		filename.empty();
		return false;
	}
}
wxf_filedlg::wxf_filedlg(HWND hwnd,dlg_type type)
{
	m_wnd = hwnd;
	m_type = type;
}
wxf_filedlg::~wxf_filedlg(void)
{
	m_wnd = NULL;
}
bool wxf_filedlg::doModel(wxf_str &buf)
{
	if (m_type == file_dlg)
	{
		return DoFileDlg(m_wnd,buf);
	}
	else if (m_type == filedir_dlg)
	{
		return DoFileDIRDlg(m_wnd,buf);
	}
	return false;
}
