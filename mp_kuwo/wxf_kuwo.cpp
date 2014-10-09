#include "pch.h"

wxf_kuwo::wxf_kuwo(void)
{
	m_use_timer = true;
}
wxf_kuwo::~wxf_kuwo(void)
{
	DeInit();
}
void wxf_kuwo::DeInit()
{
	wxf_setting &oSetting = wxf_setting::get_instance();


	int iPlayNo = m_playlist->get_play();
	oSetting.m_iPlayNo.format("%d",iPlayNo);
	m_log->Add("保持设置");

	oSetting.SetLoopMode(m_playlist->get_order());
	oSetting.SaveSetting();
	m_log->Add("保持设置成功");
	m_use_timer = false;
	m_pm.KillTimer(m_pRoot);
	delete m_playctl;
	delete m_playlist;
	m_log->Add("关闭日志文件");
	delete m_log;	
}
void wxf_kuwo::Prepare()
{
	m_pRoot = static_cast<CVerticalLayoutUI *>(m_pm.FindControl(_T("root")));

	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("close")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("max")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restore")));
	m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("min")));

	m_pDefaultList = static_cast<CListUI*>(m_pm.FindControl(_T("DefaultList")));
	m_pAdd = static_cast<CButtonUI*>(m_pm.FindControl(_T("add")));
	m_pDel = static_cast<CButtonUI*>(m_pm.FindControl(_T("del")));
	m_pFind = static_cast<CButtonUI*>(m_pm.FindControl(_T("find")));
	m_pOrder = static_cast<CButtonUI*>(m_pm.FindControl(_T("order")));

	m_pPlayerProgress = static_cast<CSliderUI*>(m_pm.FindControl(_T("PlayerProgress")));
	m_pPlayerVolumn = static_cast<CSliderUI*>(m_pm.FindControl(_T("PlayerVolumn")));
	m_pPlayControlPanel = static_cast<CHorizontalLayoutUI *>(m_pm.FindControl(_T("PlayControlPanel")));
	m_pMessageLable = static_cast<CLabelUI *>(m_pm.FindControl(_T("MessageLable")));

	m_pPre = static_cast<CButtonUI*>(m_pm.FindControl(_T("Pre")));
	m_pNext = static_cast<CButtonUI*>(m_pm.FindControl(_T("Next")));	
	
	m_pPlay_Pause = static_cast<CTabLayoutUI *>(m_pm.FindControl(_T("Play_Pause")));
	m_pPlay = static_cast<CButtonUI*>(m_pm.FindControl(_T("Play")));
	m_pPause = static_cast<CButtonUI*>(m_pm.FindControl(_T("Pause")));

	m_pMUTE_MUTED = static_cast<CTabLayoutUI *>(m_pm.FindControl(_T("MUTE_MUTED")));
	m_pMuted = static_cast<CButtonUI*>(m_pm.FindControl(_T("Muted")));
	m_pMute = static_cast<CButtonUI*>(m_pm.FindControl(_T("Mute")));

	m_pPL_LBL_CurTime = static_cast<CLabelUI*>(m_pm.FindControl(_T("PL_LBL_CurTime")));
	m_pPL_LBL_TotalTime = static_cast<CLabelUI*>(m_pm.FindControl(_T("PL_LBL_TotalTime")));

	m_pLyricDrawPanel = static_cast<CTabLayoutUI *>(m_pm.FindControl(_T("LyricDrawPanel")));
	m_nFFTPanel = static_cast<CControlUI *>(m_pm.FindControl(_T("FFTPanel")));
	m_pLyric = static_cast<COptionUI*>(m_pm.FindControl(_T("lyric")));
	m_pMV = static_cast<COptionUI*>(m_pm.FindControl(_T("mv")));

	m_pDefaultListBtn = static_cast<COptionUI*>(m_pm.FindControl(_T("DefaultListBtn")));
	m_pClassListBtn = static_cast<COptionUI*>(m_pm.FindControl(_T("ClassListBtn")));

	m_pm.SetTimer(m_pRoot,0,40);
}
void wxf_kuwo::Init()
{
	Prepare();

	m_tRunTime = 0;
	m_iDelayPlay = -1;

	
	CStdString t = (CStdString)m_pm.GetResourcePath();
	m_respath = t.GetData();
	t = (CStdString)m_pm.GetInstancePath();
	m_inspath = t.GetData();

	wxf_setting &oSetting = wxf_setting::get_instance();

	oSetting.SetRootDir(m_inspath+"run");
	oSetting.LoadSetting();
	m_listpath = oSetting.m_oPlayListPath;

	wxf_str temp = m_respath;
	temp = temp + "icon.ico";
	if (wxf_pathfile_exist(temp.c_str()) == 0)
	{
		SetIcon(temp.c_str());
	}

	m_log = new wxf_log;
	

	m_log->SetLogLevel(atoi(oSetting.m_oLogLevel.c_str()));
	m_log->SetLogPath(oSetting.m_oLogDir.c_str());
	m_log->Open(oSetting.m_oLogName.c_str());
	m_log->Add("初始化日志文件");

	m_playctl = new wxf_playctl(m_pPlayerProgress,m_log);
	m_playlist = new wxf_playlist(m_pDefaultList);

	if (m_playlist != NULL)
	{
		m_playlist->set_order(oSetting.GetLoopMode(),m_pOrder);
	}

	bool btemp = m_playlist->load_list(m_listpath.c_str());
	if (btemp)
	{
		m_playlist->set_play(atoi(wxf_setting::get_instance().m_iPlayNo.c_str()));
		//m_playctl->play(m_playlist);
		m_iDelayPlay = 25;
	}

	m_pPlayerProgress->SetMaxValue(1024);
	m_pPlayerVolumn->SetMaxValue(100);
	m_pPlayerProgress->SetValue(0);
	m_pPlayerVolumn->SetValue(80);
	m_playctl->set_vol(80, 100);
}

void wxf_kuwo::Close()
{
	m_playlist->save_list(m_listpath.c_str());
	SendMessage(WM_CLOSE, 0, 0);
}

void wxf_kuwo::Timer()
{
	int ret = wxf_succ;
	m_tRunTime = clock();
	ret = m_playctl->over(m_playlist);
	if (ret == wxf_succ)
	{
		wxf_logout(1,"Timer","播放下一曲");
		/*wxf_listitem *pitem;
		pitem = m_playlist->get_pcur();
		m_playlist->select_item(pitem);*/
	}
	if (m_playctl->get_state())
	{
		int temp = m_playctl->get_pos(1024);
		if (temp != wxf_err)
		{
			m_pPlayerProgress->SetValue(temp);
		}
		m_pMessageLable->SetText(m_playlist->get_pcur()->get_file());
		m_playctl->len_time(m_pPL_LBL_TotalTime);
		m_playctl->cur_time(m_pPL_LBL_CurTime);
		m_playctl->draw_fft(m_hWnd,m_nFFTPanel);
	}
	else
	{
		if (m_iDelayPlay != -1)
		{
			if (m_iDelayPlay == 0)
			{
				//m_playctl->play(m_playlist);
				PlayCtrl(m_pPlay);
				m_iDelayPlay = -1;
			}
			m_iDelayPlay--;
		}		
	}
}


void wxf_kuwo::AddFile()
{
	vector<wxf_str>	temp;
	//wxf_filedlg dlg(m_hWnd,wxf_filedlg::filedir_dlg);
	wxf_filedlg dlg(m_hWnd);
	if (dlg.doModel(temp))
	{
		if (temp.size() == 1)
		{
			m_playlist->add_item(temp[0].c_str());
			m_playctl->play(m_playlist);
		}
		else
		{
			for (size_t i = 1; i < temp.size(); i++)
			{
				m_playlist->add_item(temp[i].c_str());
				m_playctl->play(m_playlist);
			}
		}
				
	}
	
	return ;
}

void wxf_kuwo::DelItem()
{
	m_playlist->del_select();
	return ;
}

void wxf_kuwo::ActivateItem(int no)
{
	m_playlist->set_play(no);
	m_playctl->stop(m_playlist);
	m_playctl->play(m_playlist);

	m_playctl->len_time(m_pPL_LBL_TotalTime);
}

void wxf_kuwo::ClickItem(int no,TNotifyUI *pmsg)
{
	m_playlist->set_select(no);
	m_playlist->click_item(no,pmsg);
}

void wxf_kuwo::PlayCtrl(CControlUI *psender)
{
	if( psender == m_pPlay ) 
	{
		m_pPlay_Pause->SelectItem(1);
		m_playctl->play(m_playlist);
	}
	else if( psender == m_pPause ) 
	{
		m_pPlay_Pause->SelectItem(0);
		m_playctl->pause(m_playlist);
	}
	else if( psender == m_pMuted ) 
	{
		m_pMUTE_MUTED->SelectItem(1);
		m_playctl->muted(m_pPlayerVolumn->GetValue(),100);
	}
	else if( psender == m_pMute ) 
	{
		m_pMUTE_MUTED->SelectItem(0);
		m_playctl->mute(m_pPlayerVolumn->GetValue(),100);
	}
	else if( psender == m_pPre ) 
	{
		m_playctl->pre(m_playlist);
	}
	else if( psender == m_pNext ) 
	{
		int ret = m_playctl->next(m_playlist);

		if (ret == wxf_err)
		{
			//wxf_msg_box("下一曲错误");
			m_playlist->set_play(0);
			m_playctl->next(m_playlist);
		}
	}
	
}

void wxf_kuwo::ValueChange(CControlUI *psender)
{
	if (psender == m_pPlayerProgress)
	{
		m_use_timer = false;
		int val = m_pPlayerProgress->GetValue();
		m_playctl->set_play(val,1024);
		m_use_timer = true;
	}
	else if (psender == m_pPlayerVolumn)
	{
		int val = m_pPlayerVolumn->GetValue();
		m_playctl->set_vol(val,100);
	}
}

void wxf_kuwo::LyricChange(CControlUI *psender)
{
	if (psender == m_pLyric)
	{
		/*const RECT &rect = m_pLyricDrawPanel->GetPos();


		CRect Rect = rect;*/
		m_playctl->set_draw(false);
		m_pLyricDrawPanel->SelectItem(0);
	}
	else if (psender == m_pMV)
	{
		/*const RECT &rect = m_pLyricDrawPanel->GetPos();


		CRect Rect = rect;*/
		m_playctl->set_draw(true);
		m_pLyricDrawPanel->SelectItem(1);
	}
}
#ifndef _WXF_KUWO_NOP_
#define _WXF_KUWO_NOP_

void wxf_kuwo::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) 
	{
		Init();
	}
	else if (msg.sType == _T("timer") )	{
		if( msg.pSender == m_pRoot ) {
			if (m_use_timer) Timer();}
	}
	else if( msg.sType == _T("selectchanged") ) 
	{
		if( msg.pSender == m_pLyric ) 
			LyricChange(msg.pSender);
		else if( msg.pSender == m_pMV ) 
			LyricChange(msg.pSender);
	}
	else if( msg.sType == _T("click") ) {
		if( msg.pSender == m_pCloseBtn ) 
			Close();
		else if( msg.pSender == m_pAdd ) 
			AddFile();
		else if( msg.pSender == m_pDel ) 
			DelItem();
		else if( msg.pSender == m_pPlay ) 
			PlayCtrl(msg.pSender);
		else if( msg.pSender == m_pPause ) 
			PlayCtrl(msg.pSender);
		else if( msg.pSender == m_pMuted ) 
			PlayCtrl(msg.pSender);
		else if( msg.pSender == m_pMute ) 
			PlayCtrl(msg.pSender);
		else if( msg.pSender == m_pPre ) 
			PlayCtrl(msg.pSender);
		else if( msg.pSender == m_pNext ) 
			PlayCtrl(msg.pSender);
		else if( msg.pSender == m_pOrder )
			OrderChange(msg.pSender);
		else if( msg.pSender == m_pDefaultListBtn ) 
			ListChange(msg.pSender);
		else if( msg.pSender == m_pClassListBtn )
			ListChange(msg.pSender);

		else if( msg.pSender == m_pMinBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
		else if( msg.pSender == m_pMaxBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
		else if( msg.pSender == m_pRestoreBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
	}
	else if( msg.sType == _T("itemactivate") ) 
	{
		int no = msg.pSender->GetTag();
		ActivateItem(no);
	}
	else if( msg.sType == _T("itemclick") ) 
	{
		int no = msg.pSender->GetTag();
		ClickItem(no,&msg);
	}
	else if( msg.sType == _T("valuechanged") )
	{
		ValueChange(msg.pSender);
	}
	else if (msg.sType == _T("popthis") )
	{
		if (msg.pSender == m_pPlayerProgress)
		{
			m_use_timer = true;
		}
	}
	else if (msg.sType == _T("pushthis") )
	{
		if (msg.pSender == m_pPlayerProgress)
		{
			m_use_timer = false;
		}
	}
	/*
	else if (msg.sType == _T("timer") )
	{
	if (m_playlist->get_player()->get_state() == EM_STOP)
	{
	m_playlist->play_next();			
	}

	if (m_use_timer)
	{
	double temp;
	temp = m_playlist->get_pos();
	if (temp >= 0)
	{
	int val;
	temp = temp * 1024;
	val = (int)temp;
	m_playps->SetValue(val);
	}
	}
	}
	*/
}

LRESULT wxf_kuwo::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0,  NULL, &m_pm);
	wxf_assert(pRoot);
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	return 0;
}

LRESULT wxf_kuwo::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT wxf_kuwo::OnKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//char key = wParam;
	//if (key == VK_DELETE)
	//{
	//	int index = m_Playlist->GetTag();
	//	m_playlist->del_item(index);
	//	if (index)
	//	{
	//		m_Playlist->SetTag(index-1);
	//	}
	//	else m_Playlist->SetTag(0);
	//}
	return 0;
}

LRESULT wxf_kuwo::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}

LRESULT wxf_kuwo::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT wxf_kuwo::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT wxf_kuwo::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT wxf_kuwo::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT wxf_kuwo::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT wxf_kuwo::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT wxf_kuwo::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			m_pMaxBtn->SetVisible(false);
			m_pRestoreBtn->SetVisible(true);
		}
		else {
			m_pMaxBtn->SetVisible(true);
			m_pRestoreBtn->SetVisible(false);
		}
	}
	return lRes;
}

LRESULT wxf_kuwo::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_KEYDOWN:	   lRes = OnKey(uMsg, wParam, lParam, bHandled); break;
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	//case WM_ADDFILE:	   lRes = OnAddFile(uMsg, wParam, lParam, bHandled); break;
	//case WM_PLAYCTL:	   lRes = OnPlayCtrl(uMsg, wParam, lParam, bHandled); break;
	//case WM_DELITEM:	   lRes = OnDelItem(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void wxf_kuwo::OrderChange( CControlUI *psender )
{
	int mode = m_playlist->get_order();
	m_playlist->set_order(mode+1,m_pOrder);
}

void wxf_kuwo::ListChange( CControlUI *psender )
{
	bool bRst = false;
	if (psender == m_pDefaultListBtn)
	{
		bRst = m_playlist->change_list(false);
	}
	else
	{
		bRst = m_playlist->change_list(true);
	}
	if (bRst)
	{
		PlayCtrl(0);	
	}
}

#endif

