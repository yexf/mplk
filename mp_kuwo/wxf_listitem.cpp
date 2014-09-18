#include "pch.h"


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

	m_nop = NULL;
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
	m_songname = new CLabelUI;
	m_singername = new CLabelUI;
	m_mvflag = new CLabelUI;
	m_nop = new CControlUI;

	m_item->Add(m_phl);
	m_item->SetAttribute("height","23");

	m_listno->SetAttribute("width","20");
	m_singername->SetAttribute("width","64");
	m_mvflag->SetAttribute("width","20");
	m_nop->SetAttribute("width","4");
	m_listno->SetAttribute("align","right");

	m_songname->SetAttribute("endellipsis","true");
	m_singername->SetAttribute("endellipsis","true");

	m_phl->Add(m_listno);
	m_phl->Add(m_nop);
	m_phl->Add(m_songname);
	m_phl->Add(m_singername);
	m_phl->Add(m_mvflag);

	m_listno->SetFont(14);
	m_singername->SetFont(14);
	m_songname->SetFont(14);
	m_songname->SetAttribute("textcolor","#7f1f1f1f");
	m_singername->SetAttribute("textcolor","#7f1f1f1f");

}
int wxf_listitem::set_no(int no)
{
	m_no = no;

	wxf_str temp;
	temp.format("%d",no+1);

	m_listno->SetText(temp.data());
	m_item->SetTag(no);
	return wxf_succ;
}
int wxf_listitem::set_file(const char *file_name)
{
	m_path = file_name;
	wxf_str temp(file_name);

	temp = temp.split_last('\\');
	m_singername->SetText(temp.split_last('-', false).c_str());

	temp = temp.split_last('-');
	m_songname->SetText(temp.split_last('.', false).c_str());
	
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
