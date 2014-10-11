#include "pch.h"
#include "wxf_mp3st.h"

wxf_listitem::wxf_listitem(void)
{
	init();
}
wxf_listitem::wxf_listitem(int no,const char *file_name)
{
	init();
	set_file(file_name);
	set_no(no);
}

wxf_listitem::~wxf_listitem(void)
{
	deinit();
	m_listno = NULL;
	m_songname = NULL;
	m_singername = NULL;
	m_mvflag = NULL;

	//m_nop = NULL;
	m_like = NULL;
	m_phl = NULL;
	m_item = NULL;
}

void wxf_listitem::deinit(void)
{
	delete m_item;
}
void wxf_listitem::init(void)
{
	m_listno = NULL;
	m_songname = NULL;
	m_singername = NULL;
	m_mvflag = NULL;

	m_nop = NULL;
	m_phl = NULL;
	m_item = NULL;
	m_next = NULL;

	m_id3.Title = NULL;
	m_id3.Artist = NULL;
	m_id3.Album = NULL;
	m_id3.Year = NULL;
	m_id3.Comment = NULL;
	m_id3.TrackNum = NULL; 
	m_id3.Genre = NULL;

	m_item = new CListContainerElementUI;
	m_phl = new CHorizontalLayoutUI;

	m_listno = new CLabelUI;
	m_like = new CLabelUI;
	m_songname = new CLabelUI;
	m_singername = new CLabelUI;
	m_mvflag = new CLabelUI;
	//m_nop = new CControlUI;

	m_item->Add(m_phl);
	m_item->SetAttribute("height","23");

	m_listno->SetAttribute("width","30");
	m_like->SetAttribute("width","24");
	m_singername->SetAttribute("width","64");
	m_mvflag->SetAttribute("width","20");
	//m_nop->SetAttribute("width","4");
	m_listno->SetAttribute("align","right");

	m_songname->SetAttribute("endellipsis","true");
	m_singername->SetAttribute("endellipsis","true");

	m_phl->Add(m_listno);
	m_phl->Add(m_like);
	m_phl->Add(m_songname);
	m_phl->Add(m_singername);
	m_phl->Add(m_mvflag);

	m_listno->SetFont(14);
	m_singername->SetFont(14);
	m_songname->SetFont(14);
	m_songname->SetAttribute("textcolor","#7f1f1f1f");
	m_singername->SetAttribute("textcolor","#7f1f1f1f");

	m_defBKColor = m_item->GetBkColor();
	m_PlayBKColor = 0x7f4f4f4f;

	set_like(false);

}
int wxf_listitem::set_no(int no)
{
	m_no = no;

	wxf_str temp;
	temp.format(" %d",no+1);

	m_listno->SetText(temp.data());
	m_item->SetTag(no);
	return wxf_succ;
}
int wxf_listitem::set_file(const char *file_name)
{
	wxf_str oTemp;
	wxf_id3 oItem;

	m_path = file_name;

	if (wxf_get_id3(file_name, oItem))
	{
		m_singername->SetText(oItem.m_strArtist.c_str());
		m_songname->SetText(oItem.m_strTitle.c_str());
	}
	else
	{
		/*wxf_str temp(file_name);

		temp = temp.split_last('\\');
		m_singername->SetText(temp.split_last('-', false).c_str());

		temp = temp.split_last('-');
		m_songname->SetText(temp.split_last('.', false).c_str());*/
		m_singername->SetText("(未知的艺术家)");
		m_songname->SetText("(未知的标题)");
		
	}
	return wxf_succ;
}
int wxf_listitem::add_to(CListUI *plist)
{
	plist->Add(m_item);
	return wxf_succ;
}
int wxf_listitem::remove_form(CListUI *plist)
{
	plist->Remove(m_item);
	return wxf_succ;
}

void wxf_listitem::set_play( bool IsPlay )
{
	if (IsPlay)
	{
		m_listno->SetFont(13);
		m_singername->SetFont(13);
		m_songname->SetFont(13);
		m_item->SetBkColor(m_PlayBKColor);
		
	}
	else
	{
		m_listno->SetFont(14);
		m_singername->SetFont(14);
		m_songname->SetFont(14);
		m_item->SetBkColor(m_defBKColor);
	}
}

void wxf_listitem::set_like(bool Islike)
{
	wxf_str name = "CasualListenWhiteHeart.png";
	wxf_str file = "";
	if (!Islike)
	{
		file = "file='"+name+"' source='0,0,14,13'";
	}
	else
	{
		file = "file='"+name+"' source='15,0,29,13'";
	}
	wxf_str attr = file + " dest='5,5,19,18'";
	m_like->SetBkImage(attr.c_str());

	m_bIsLike = Islike;
}

void wxf_listitem::change_like()
{
	set_like(!m_bIsLike);
}

void wxf_listitem::click_item( TNotifyUI *pmsg )
{
	//if (pmsg->pSender == m_like)

	if (::PtInRect(&m_like->GetPos(), pmsg->ptMouse))
		set_like(!m_bIsLike);
}

//////////////////////////////////////////////////////////////////////////
//
//

void wxf_item_data::set_filepath(const char *strFilePath)
{
	wxf_id3 oItem;

	if (strFilePath)
	{
		m_strFilePath = strFilePath;
	}
	if (wxf_get_id3(strFilePath, oItem))
	{
		m_strArtist = oItem.m_strArtist;
		m_strTitle = oItem.m_strTitle;
	}
}

void wxf_item_data::set_fav( bool bFav )
{
	if (m_bIsFav != bFav)
	{
		m_bIsFav = bFav;
	}
}

void wxf_item_data::set_play( bool bPlay )
{
	if (m_bIsPlay != bPlay)
	{
		m_bIsPlay = bPlay;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
int wxf_item_view::m_siNum = 0;
void wxf_item_view::init()
{
	deinit();

	m_pItem = new CListContainerElementUI;
	m_pLayoutMgr = new CHorizontalLayoutUI;

	m_pListno = new CLabelUI;
	m_pFav = new CLabelUI;
	m_pSongname = new CLabelUI;
	m_pSingername = new CLabelUI;
	m_pMVflag = new CLabelUI;

	m_pItem->Add(m_pLayoutMgr);
	m_pItem->SetAttribute("height","23");

	m_pListno->SetAttribute("width","30");
	m_pFav->SetAttribute("width","24");
	//m_pSongname
	m_pSingername->SetAttribute("width","64");
	m_pMVflag->SetAttribute("width","20");

	//标号右对齐
	m_pListno->SetAttribute("align","right");

	//歌手名和标题 较长时用省略号表示
	m_pSongname->SetAttribute("endellipsis","true");
	m_pSingername->SetAttribute("endellipsis","true");

	m_pLayoutMgr->Add(m_pListno);
	m_pLayoutMgr->Add(m_pFav);
	m_pLayoutMgr->Add(m_pSongname);
	m_pLayoutMgr->Add(m_pSingername);
	m_pLayoutMgr->Add(m_pMVflag);

	m_pListno->SetFont(14);
	m_pSingername->SetFont(14);
	m_pSongname->SetFont(14);
	m_pSongname->SetAttribute("textcolor","#7f1f1f1f");
	m_pSingername->SetAttribute("textcolor","#7f1f1f1f");

	m_dwDefBKColor = m_pItem->GetBkColor();
	m_dwPlayBKColor = 0x7f4f4f4f;

	wxf_str temp;
	temp.format(" %d",m_siNum);
	m_iCurNo = m_siNum;
	m_pListno->SetText(temp.data());
	m_pItem->SetTag((UINT_PTR)this);
	
	m_bIsInit = true;
}

void wxf_item_view::deinit()
{
	if (m_bIsInit)
	{
		delete m_pItem;
	}

	m_pListno = NULL;
	m_pFav = NULL;
	m_pSongname = NULL;
	m_pSingername = NULL;
	m_pMVflag = NULL;

	m_pLayoutMgr = NULL;
	m_pItem = NULL;

	m_dwDefBKColor = 0x0;
	m_dwPlayBKColor = 0x0;

	m_bIsInit = false;
}


void wxf_item_view::click_item( TNotifyUI *psender )
{
	if (m_bIsInit && m_pData)
	{
		if (::PtInRect(&m_pFav->GetPos(), psender->ptMouse))
			m_pData->set_fav(!m_pData->get_isfav());
		update_view(m_pData);
	}	
}

void wxf_item_view::update_view(wxf_item_data *pData)
{
	if (m_bIsInit && pData)
	{
		m_pData = pData;

		m_pSingername->SetText(pData->get_artist().c_str());
		m_pSongname->SetText(pData->get_title().c_str());

		// 设置fav
		wxf_str name = "CasualListenWhiteHeart.png";
		wxf_str file = "";
		if (!pData->get_isfav())
		{
			file = "file='"+name+"' source='0,0,14,13'";
		}
		else
		{
			file = "file='"+name+"' source='15,0,29,13'";
		}
		wxf_str attr = file + " dest='5,5,19,18'";

		m_pFav->SetBkImage(attr.c_str());

		// 设置是否为当前播放歌曲
		if (pData->get_isplay())
		{
			m_pListno->SetFont(13);
			m_pSingername->SetFont(13);
			m_pSongname->SetFont(13);
			m_pItem->SetBkColor(m_dwPlayBKColor);

		}
		else
		{
			m_pListno->SetFont(14);
			m_pSingername->SetFont(14);
			m_pSongname->SetFont(14);
			m_pItem->SetBkColor(m_dwDefBKColor);
		}
	}
}

