#pragma once

//#define WM_PLAYCTL						WM_USER + 0x0001
//#define WM_ADDFILE						WM_USER + 0x0002
//#define WM_DELITEM						WM_USER + 0x0003
//#define WM_TIMER25HZ					WM_USER + 0x0004

class wxf_player;
class wxf_playlist;
class wxf_playctl;
class wxf_kuwo: public CWindowWnd,public INotifyUI
{
public:
	wxf_kuwo(void);
	~wxf_kuwo(void);
public:
	LPCTSTR GetWindowClassName() const { return _T("WXFPlay"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };

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

protected:
	CPaintManagerUI m_pm;
	CVerticalLayoutUI *m_pRoot;
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;

	CListUI *m_pDefaultList;
	CButtonUI* m_pAdd;
	CButtonUI* m_pDel;
	CButtonUI* m_pFind;
	CButtonUI* m_pOrder;

	CHorizontalLayoutUI *m_pPlayControlPanel;
	CSliderUI *m_pPlayerProgress;
	CSliderUI *m_pPlayerVolumn;
	CLabelUI *m_pMessageLable;

	CButtonUI* m_pPre;
	CButtonUI* m_pNext;
	
	CTabLayoutUI *m_pPlay_Pause;
	CButtonUI* m_pPlay;
	CButtonUI* m_pPause;

	CTabLayoutUI *m_pMUTE_MUTED;
	CButtonUI* m_pMuted;
	CButtonUI* m_pMute;

	CLabelUI *m_pPL_LBL_CurTime;
	CLabelUI *m_pPL_LBL_TotalTime;

	CTabLayoutUI *m_pLyricDrawPanel;
	CControlUI *m_nFFTPanel;
	COptionUI *m_pLyric;
	COptionUI *m_pMV;

private:	
	wxf_log	*m_log;
	wxf_playlist *m_playlist;
	wxf_playctl *m_playctl;

	bool m_use_timer;
	clock_t m_tRunTime;
	int m_iDelayPlay;
	
	wxf_str m_respath;
	wxf_str m_inspath;
	wxf_str m_listpath;

private:
	void DeInit();
	void Init();
	void Prepare();
	void Close();
	void Timer();
	
	void AddFile();
	
	void DelItem();
	void ActivateItem(int no);
	void ClickItem(int no);
	void PlayCtrl(CControlUI *psender);
	void ValueChange(CControlUI *psender);
	void LyricChange(CControlUI *psender);
private:
	inline void wxf_logout(int ret,const char *inf,char *msg)
	{
		wxf_log	*log;

		log = m_log;

		if (ret == 0)
		{
			log->FormatAdd(
				wxf_log::error,
				"wxf_kuwo :[%s]=>>%s",
				inf,msg);
		}
		else
		{
			log->FormatAdd(
				wxf_log::trace,
				"wxf_kuwo :[%s]=>>%s",
				inf,msg);
		}
	}

};

