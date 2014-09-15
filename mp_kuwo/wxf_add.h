#pragma once

class CFileDlg
{
public:
	CFileDlg(HWND hwnd);
	~CFileDlg(void);
	wxf_str m_filename;
};


class wxf_add
{
public:
	wxf_add(void);
	wxf_add(CKuWo *kuwo);
	wxf_add(CKuWo *kuwo,const char *file);
	~wxf_add(void);

private:
	CKuWo *m_pkuwo;
};

