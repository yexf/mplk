#include "ybyt.h"

wxf_playlist::wxf_playlist(void)
{
	init();
}


wxf_playlist::~wxf_playlist(void)
{
	deinit();
	
}

void wxf_playlist::close()
{
	m_player->close();
	delete m_player;
}
void wxf_playlist::init()
{
	m_player = new wxf_player();
	head = NULL;
	tail = NULL;
	pcur = NULL;
	item_num = 0;

	loop_mode = EM_LISTLOOP;
}
void wxf_playlist::deinit()
{
	delete m_player;
}
double wxf_playlist::get_pos(void)
{
	return m_player->get_pos();
}
void wxf_playlist::insert(wxf_plitem *temp)
{
	if (head == NULL)
	{
		head = temp;
		tail = temp;
	}
	else
	{
		tail->add_next(temp);
		tail = temp;
	}
	m_plist->Add(temp->get_ele());
	temp->set_index(item_num++);
}
wxf_plitem *wxf_playlist::add(const wxf_str &file)
{
	wxf_plitem *temp;
	temp = new wxf_plitem;
	insert(temp);
	
	TID3InfoEx *id3;
	id3 = m_player->get_id3(file.c_str());
	temp->set_id3(id3);
	
	temp->set_file(file);

	if (pcur == NULL)
	{
		pcur = temp;
		m_player->play(temp);
		select_item(temp);
	}
	return temp;
}
wxf_plitem *wxf_playlist::find(int index)
{
	wxf_plitem *temp;
	temp = head;

	while(temp != NULL)
	{
		if (index == temp->get_index())
		{
			break;
		}
		temp = temp->get_next();
	}
	return temp;
}
void *wxf_playlist::del_item(int index)
{
	wxf_plitem *temp,*next;
	
	temp = find(index);
	next = temp->get_next();
	m_plist->RemoveAt(index);
	if (temp == head)
	{
		head = next;
	}
	else
	{
		wxf_plitem *pre;
		pre = find(index-1);
		pre->add_next(next);
	}
	//delete temp;
	temp = next;
	while(temp != NULL)
	{
		temp->set_index(index);
		temp = temp->get_next();
		index++;
	}
	item_num--;
	return next;
}
void wxf_playlist::select_hdl(int index)
{
	wxf_plitem *temp;
	
	temp = find(index);
	pcur = temp;
	m_player->play(temp);
	
}
int wxf_playlist::select_item(wxf_plitem *pitem)
{
	int index = 0;
	if (pitem != NULL)
	{
		index = pitem->get_index();
	}

	m_plist->SelectItem(index);
	return index;
}
wxf_plitem *wxf_playlist::next_song(wxf_plitem *pitem)
{
	if (pitem == NULL)return NULL;	
	wxf_plitem *pnext = NULL;
	int rnd = wxf_rand((int)pitem);

	switch (loop_mode)
	{
	case EM_LISTLOOP:
		{
			pnext = pitem->get_next();
			if (pnext == NULL)
				pnext = head;
			break;
		}
	case EM_LISTPLAY:
		{
			pnext = pitem->get_next();
			break;
		}
	case EM_RANDOMLOOP:
		{
			pnext = find(rnd%item_num);
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
void wxf_playlist::play_next(void)
{
	pcur = next_song(pcur);

	m_player->play(pcur);
	select_item(pcur);
}

void wxf_playlist::save(wxf_file_t fp)
{
	wxf_plitem *temp;
	temp = head;

	while(temp != NULL)
	{
		wxf_fputs(temp->get_file().c_str(),fp);
		wxf_fputs("\r\n",fp);
		temp = temp->get_next();
	}
}