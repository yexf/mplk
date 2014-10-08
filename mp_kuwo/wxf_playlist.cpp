#include "pch.h"
#pragma warning(disable:4996)

wxf_listitem *wxf_playlist::get_pcur(void)
{
	if (m_nplay == -1)
	{
		m_nplay = 0;
		return m_head;
	}
	return find_item(m_nplay);
}
wxf_listitem *wxf_playlist::get_next(void)
{
	wxf_listitem *pitem;
	if (m_nplay == -1)
	{
		m_nplay = 0;
	}
	
	pitem = find_item(m_nplay);
	pitem = get_next(pitem);
	
	return pitem;
}
wxf_listitem *wxf_playlist::get_pre(wxf_listitem *pitem)
{
	if (pitem == NULL) return NULL;	
	wxf_listitem *pre = NULL;
	
	if (pitem == m_head)
	{
		pre = m_tail;
	}
	else
	{
		int no = pitem->get_no();
		pre = find_pre(no);
	}
		
	return pre;
}
wxf_listitem *wxf_playlist::get_next(wxf_listitem *pitem)
{	
	if (pitem == NULL)return NULL;	
	wxf_listitem *pnext = NULL;
	int rnd = wxf_rand((int)pitem);

	switch (m_loopmode)
	{
	case EM_LISTLOOP:
		{
			pnext = pitem->get_next();
			if (pnext == NULL)
				pnext = m_head;
			break;
		}
	case EM_LISTPLAY:
		{
			pnext = pitem->get_next();
			break;
		}
	case EM_RANDOMLOOP:
		{
			pnext = find_item(rnd%m_itemnum);
			break;
		}
	case EM_SONGLOOP:
		{
			pnext = pitem;
			break;
		}
	}
	return pnext;
}
void wxf_playlist::select_item(wxf_listitem *pitem)
{
	int no;
	if (pitem)
	{
		no = pitem->get_no();
		m_plist->SelectItem(no);
	}
}
wxf_listitem *wxf_playlist::find_item(int no)
{
	wxf_listitem *temp;
	temp = m_head;
	while(temp != NULL)
	{
		if (no == temp->get_no())
		{
			break;
		}
		temp = temp->get_next();
	}
	return temp;
}

int wxf_playlist::add_item(const char *file_name)
{
	int ret = wxf_succ;
	wxf_listitem *pitem;

	pitem = new wxf_listitem(m_itemnum,file_name);
	
	push_item(pitem);
	return ret;
}

wxf_listitem *wxf_playlist::find_pre(int no)
{
	wxf_listitem *temp,*next;
	temp = m_head;
	if (temp == NULL) return NULL;
	next = temp->get_next();
	while(next != NULL)
	{
		if (no == next->get_no())
		{
			break;
		}
		temp = next;
		next = next->get_next();
	}
	return temp;
}

int wxf_playlist::del_select()
{
	wxf_listitem *pitem;
	wxf_listitem *temp;
	int no = m_nselect;
	pitem = find_item(no);
	if (pitem == NULL)
		return wxf_err;
	
	temp = find_pre(no);
	temp->set_next(pitem->get_next());
	temp = pitem->get_next();
	while(temp != NULL)
	{
		temp->set_no(no++);
		temp = temp->get_next();
	}

	m_itemnum--;
	if (m_nselect >= m_itemnum)
	{
		m_nselect--;
	}
	m_plist->SelectItem(m_nselect);
	pitem->remove_form(m_plist);
	
	//delete pitem;
	pitem = NULL;

	return wxf_succ;
}


void wxf_playlist::save_list(const char *file_name)
{
	wxf_file_t fp;
	wxf_listitem *temp = m_head;
	fp = wxf_fopen(file_name,WXF_OM_CREATE);


	while(temp != NULL)
	{
		wxf_fputs(temp->get_file(),fp);
		wxf_fputs("\r\n",fp);
		temp = temp->get_next();
	}
	wxf_fclose(fp);
}
bool wxf_playlist::load_list(const char *file_name)
{
	bool ret = false;
	wxf_file_t fp;
	char buf[256];
	wxf_listitem *temp = m_head;
	fp = wxf_fopen(file_name,WXF_OM_READ);
	if (fp == NULL)
	{
		return false;
	}
	while(wxf_fgets(buf,256,fp) != NULL)
	{
		char *pstr;
		pstr = wxf_strstr(buf,"\r\n");
		*pstr = '\0';
		if (wxf_pathfile_exist(buf) == 0)
		{
			add_item(buf);
			if (ret == false)
			{
				ret = true;
			}
		}
	}

	wxf_fclose(fp);

	return ret;
}

void wxf_playlist::init(void)
{
	m_head = NULL;
	m_tail = NULL;
	m_nplay = -1;
	m_nselect = -1;
	m_loopmode = EM_LISTLOOP;
	m_itemnum = 0;
}

void wxf_playlist::deinit(void)
{
	m_plist = NULL;
	m_log = NULL;
}
void wxf_playlist::push_item(wxf_listitem *pitem)
{
	if (m_head == NULL)
	{
		m_head = pitem;
		m_tail = pitem;
		m_itemnum = 1;
	}
	else
	{
		m_tail->set_next(pitem);
		m_tail = pitem;
		m_itemnum++;
	}

	pitem->add_to(m_plist);
}

void wxf_playlist::set_play(int no)
{
	if (m_nplay == no)
	{
		return ;
	}
	if (m_nplay != -1)
	{
		find_item(m_nplay)->set_play(false);
	}
	
	m_nplay = no;
	find_item(m_nplay)->set_play(true);
}

void wxf_playlist::set_order( int loopmode, CButtonUI *pOrder )
{
	wxf_str filename,tip;
	m_loopmode = loopmode % EM_MODEMAX;
	switch(m_loopmode)
	{
	case EM_LISTLOOP:
		filename = "PlaylistBtnIcoModeCircle.png";
		tip = "循环播放";
		break;
	case EM_LISTPLAY:
		filename = "PlaylistBtnIcoModeOrder.png";
		tip = "顺序播放";
		break;
	case EM_SONGLOOP:
		filename = "PlaylistBtnIcoModeSingle.png";
		tip = "单曲循环";
		break;
	case EM_RANDOMLOOP:
		filename = "PlaylistBtnIcoModeRand.png";
		tip = "随机播放";
		break;
	default:break;

	}
	wxf_str attr = "file='" + filename + "' dest='3,2,44,18'";
	pOrder->SetBkImage(attr.c_str());
	pOrder->SetToolTip(tip.c_str());
}

int wxf_playlist::get_order()
{
	return m_loopmode;
}
