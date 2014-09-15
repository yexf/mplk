#pragma once
class CKuWo:public CWindowWnd,public INotifyUI
{
public:
	CKuWo(void);
	~CKuWo(void);
public:
	LPCTSTR GetWindowClassName() const { return _T("WXFPlay"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	
	void Init();
	void SavePlaylist();
	void OnPrepare();

	void Notify(TNotifyUI& msg);
	LRESULT OnKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CPaintManagerUI m_pm;

	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	//...
	CEditUI *m_pSearchedit;
	CTabLayoutUI* m_playlist_tab;
	CListUI *m_Playlist;

	wxf_playlist *m_playlist;

	CButtonUI* m_pAdd;
	CButtonUI* m_pDel;
	CButtonUI* m_pFind;
	CButtonUI* m_pOrder;

	CSliderUI *m_playps;
	CSliderUI *m_playvol;

	bool m_use_timer;

	wxf_str m_respath;
	wxf_str m_inspath;
};

