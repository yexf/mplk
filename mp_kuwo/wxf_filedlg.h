#pragma once
#include <vector>
bool DoFileDIRDlg(HWND hwnd,wxf_str &dirname);
class wxf_filedlg
{
public:
	enum dlg_type
	{
		file_dlg,
		filedir_dlg
	};
public:
	wxf_filedlg(HWND hwnd = NULL,dlg_type type = file_dlg);
	bool doModel(wxf_str &buf);
	bool doModel(vector<wxf_str> &buf);
	~wxf_filedlg(void);

private:
	HWND m_wnd;
	dlg_type m_type;
};

