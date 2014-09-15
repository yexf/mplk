#include "ybyt.h"

CKuWo::CKuWo(void)
{
}
CKuWo::~CKuWo(void)
{
	
}


void CKuWo::Init() {
	m_use_timer = true;

	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("close")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("max")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restore")));
	m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("min")));

	m_pSearchedit = static_cast<CEditUI*>(m_pm.FindControl(_T("searchedit")));

	m_playlist_tab = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("DefaultListRecoveryTab")));

	m_Playlist = static_cast<CListUI*>(m_pm.FindControl(_T("DefaultList")));
	
	m_playlist = new wxf_playlist(m_Playlist);

	m_pAdd = static_cast<CButtonUI*>(m_pm.FindControl(_T("add")));
	m_pDel = static_cast<CButtonUI*>(m_pm.FindControl(_T("del")));
	m_pFind = static_cast<CButtonUI*>(m_pm.FindControl(_T("find")));
	m_pOrder = static_cast<CButtonUI*>(m_pm.FindControl(_T("order")));

	m_playps = static_cast<CSliderUI*>(m_pm.FindControl(_T("PlayerProgress")));
	m_playvol = static_cast<CSliderUI*>(m_pm.FindControl(_T("PlayerVolumn")));
	m_playps->SetMaxValue(1024);
	m_playvol->SetValue(80);
	m_playlist->get_player()->set_vol(0.8);
	m_pm.SetTimer(m_playps,0,10);
}
void CKuWo::SavePlaylist()
{
	wxf_file_t fp;
	wxf_str temp;
	temp = m_inspath + "playlist.txt";
	fp = wxf_fopen(temp.c_str(),WXF_OM_WRITE);
	m_playlist->save(fp);
	m_playlist->close();
	//delete m_playlist;
}
void CKuWo::OnPrepare() {
}

void CKuWo::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) OnPrepare();
	else if( msg.sType == _T("click") ) {
		if( msg.pSender == m_pCloseBtn ) {
			SavePlaylist();
			PostQuitMessage(0);
			return; 
		}
		else if( msg.pSender == m_pMinBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
		else if( msg.pSender == m_pMaxBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
		else if( msg.pSender == m_pRestoreBtn ) { 
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }

		else if( msg.pSender == m_pAdd ) {
			wxf_add temp(this);
		}
	}
	else if(msg.sType==_T("click"))
	{
		CStdString name = msg.pSender->GetName();

		if (name == _T("PlaylistBtn"))
		{
			COptionUI *temp = static_cast<COptionUI*>(msg.pSender);
			temp->SetForeImage(temp->GetBkImage());
			temp->SetVisible(false);
		}

		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
		if(name==_T("examine"))
			pControl->SelectItem(0);
		else if(name==_T("trojan"))
			pControl->SelectItem(1);
		else if(name==_T("plugins"))
			pControl->SelectItem(2);
		else if(name==_T("vulnerability"))
			pControl->SelectItem(3);
		else if(name==_T("rubbish"))
			pControl->SelectItem(4);
		else if(name==_T("cleanup"))
			pControl->SelectItem(5);
		else if(name==_T("fix"))
			pControl->SelectItem(6);
		else if(name==_T("tool"))
			pControl->SelectItem(7);
	}
	else if( msg.sType == _T("itemactivate") ) 
	{
		int iIndex = msg.pSender->GetTag();
		m_playlist->select_hdl(iIndex);
	}
	else if( msg.sType == _T("itemclick") ) 
	{
		int iIndex = msg.pSender->GetTag();
		m_Playlist->SetTag(iIndex);
	}
	else if( msg.sType == _T("valuechanged") )
	{
		if (msg.pSender == m_playps)
		{
			int val = m_playps->GetValue();
			double temp = val;
			m_playlist->get_player()->seek(temp/1024);
		}
		else if (msg.pSender == m_playvol)
		{
			double vol;
			int temp = m_playvol->GetValue();
			vol = temp;
			vol = vol /100;
			m_playlist->get_player()->set_vol(vol);
		}
	}
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
	else if (msg.sType == _T("popthis") )
	{
		if (msg.pSender == m_playps)
		{
			m_use_timer = true;
		}
	}
	else if (msg.sType == _T("pushthis") )
	{
		if (msg.pSender == m_playps)
		{
			m_use_timer = false;
		}
	}
}
LRESULT CKuWo::OnKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
LRESULT CKuWo::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0,  NULL, &m_pm);
	WXF_ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	CStdString t = (CStdString)m_pm.GetResourcePath();
	m_respath = t.GetData();
	t = (CStdString)m_pm.GetInstancePath();
	m_inspath = t.GetData();
	//this->SetIcon(m_respath+"icon.ico");
	Init();
	wxf_str temp;
	temp = m_inspath + "playlist.txt";
	if (wxf_fexist(temp.c_str()))
	{
		new wxf_add(this,temp.c_str());
	}
	return 0;
}

LRESULT CKuWo::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	
	return 0;
}

LRESULT CKuWo::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	::PostQuitMessage(0L);
	bHandled = FALSE;
	return 0;
}

LRESULT CKuWo::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CKuWo::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CKuWo::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CKuWo::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	// 		if( !::IsZoomed(*this) ) {
	// 			RECT rcSizeBox = m_pm.GetSizeBox();
	// 			if( pt.y < rcClient.top + rcSizeBox.top ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
	// 				return HTTOP;
	// 			}
	// 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
	// 				return HTBOTTOM;
	// 			}
	// 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
	// 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	// 		}

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

LRESULT CKuWo::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CKuWo::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CKuWo::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
			/*CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("max")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("returnold")));
			if( pControl ) pControl->SetVisible(true);*/
			m_pMaxBtn->SetVisible(false);
			m_pRestoreBtn->SetVisible(true);
		}
		else {
			/*CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("max")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("returnold")));
			if( pControl ) pControl->SetVisible(false);*/
			m_pMaxBtn->SetVisible(true);
			m_pRestoreBtn->SetVisible(false);
		}
	}
	return lRes;
}

LRESULT CKuWo::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}
