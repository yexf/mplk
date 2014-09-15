#include "ybyt.h"

CFileDlg::CFileDlg(HWND hwnd)
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
		m_filename = szFile;
	}
}

CFileDlg::~CFileDlg(void)
{
}


wxf_add::wxf_add(void)
{
}

wxf_add::wxf_add(CKuWo *kuwo):m_pkuwo(kuwo)
{
	CFileDlg temp = m_pkuwo->GetHWND();
	if (!temp.m_filename.empty())
	{
		wxf_playlist *ppl;
		ppl = m_pkuwo->m_playlist;

		wxf_plitem *pitem;
		pitem = ppl->add(temp.m_filename);
		
	}	
}

wxf_add::wxf_add(CKuWo *kuwo,const char *filename):m_pkuwo(kuwo)
{
	char buf[256];
	wxf_file_t fp;
	fp = wxf_fopen(filename,WXF_OM_READ);
	while(wxf_fgets(buf,256,fp))
	{
		if (wxf_fexist(buf))
		{
			wxf_playlist *ppl;
			ppl = m_pkuwo->m_playlist;

			wxf_str	temp(buf);
			ppl->add(temp);
		}
	}



}

wxf_add::~wxf_add(void)
{
}
