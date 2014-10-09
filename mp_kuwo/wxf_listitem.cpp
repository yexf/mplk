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
