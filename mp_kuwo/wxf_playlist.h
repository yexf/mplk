/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : wxf_playlist.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/30
**
** Description:播放列表 为播放控制器提供播放内容
**************************************************************************************/ 
#pragma once
#include "pch.h"
enum loop_mode
{
	EM_LISTLOOP = 0,			//列表循环
	EM_LISTPLAY,				//列表播放
	EM_SONGLOOP,				//单曲循环
	EM_RANDOMLOOP,				//随机循环
	EM_MODEMAX
};


class wxf_playlist_doc
{
public:
	wxf_playlist_doc() : m_bFav(false)
	{

	}
	~wxf_playlist_doc()
	{
		for (int i=0;i<m_vecItem.size();i++)
		{
			delete m_vecItem[i];
		}
	}
	int get_count()
	{
		return m_vecItem.size();
	}
	wxf_item_data *get_data(int index)
	{
		return m_vecItem[index];
	}
public:
	void save_list(const char *file_name);
	bool load_list(const char *file_name);

	int add_item(const char *file_name,bool bFav);
	void delete_item(int index);

	int get_index(wxf_item_data *pData);
	vector<wxf_item_data*>::iterator get_iter(int index);

	bool get_fav() {return m_bFav;}
	void set_fav(bool bFav) {m_bFav = bFav;}
protected:
	vector<wxf_item_data*> m_vecItem;
	bool m_bFav;
};

class wxf_playlist_view
{
public:
	wxf_playlist_view(CListUI *plist)
		:m_pList(plist),m_pDoc(NULL),m_iPlayIndex(-1)
	{
		
	}
public:
	wxf_item_view *add_item(wxf_item_data *pData,int index);
	int del_item(int index);
	int del_item();

	void del_all();
	void add_doc(wxf_playlist_doc *pDoc,bool bFav);
	void update_view(bool bFav);

	int get_select()
	{
		return m_pList->GetCurSel();
	}
	void set_play(int index);

	int get_play()
	{
		return m_iPlayIndex;
	}

	wxf_item_view *at_item(int no)
	{
		map<int,wxf_item_view*>::iterator pIter = m_mapItem.begin();
		if (no < m_mapItem.size())
		{
			for (int i=0;i<no;i++)
			{
				pIter++;
			}
			return pIter->second;
		}
		return NULL;
	}
	wxf_item_view *get_item(int no)
	{
		if (m_mapItem.find(no) != m_mapItem.end())
		{
			return m_mapItem[no];
		}
		return NULL;
	}
	bool select_item(int no)
	{
		if (m_mapItem.find(no) != m_mapItem.end())
		{
			m_pList->SelectItem(get_item(no)->get_no());
			return true;
		}
		return false;
	}

	int get_order()
	{
		return m_emLoopmode;
	}
	void set_order(int loopmode, CButtonUI *pOrder);

	int play_next();
	int play_pre();

	int get_mapat(int iNo);

	int near_index(int index);

public:
	void set_doc(wxf_playlist_doc *pDoc)
	{
		m_pDoc = pDoc;
	}
	wxf_playlist_doc *get_doc()
	{
		return m_pDoc;
	}

protected:
	CListUI *m_pList;
	map<int,wxf_item_view*> m_mapItem;

	int m_iPlayIndex;		//-1
	int m_emLoopmode;

	wxf_playlist_doc *m_pDoc;
};

class wxf_playlist
{
public:
	
public:
	wxf_playlist(CListUI *plist):
	  m_oPlayListView(plist)
	{

	}
	~wxf_playlist(void)
	{
		m_oPlayListView.del_all();
	}
	
public:
	int get_index(wxf_item_view *pView)
	{
		return m_oPlayListDoc.get_index(pView->get_data());
	}
	bool play_next()
	{
		int index = m_oPlayListView.get_play();
		m_oPlayListDoc.get_data(index)->set_play(false);

		index = m_oPlayListView.play_next();
		if (index == -1)
		{
			index = m_oPlayListView.near_index(get_play());
		}
		if (index == -1)
		{
			return false;
		}
		m_oPlayListView.set_play(index);

		return true;
	}
	bool play_pre()
	{
		int index = m_oPlayListView.get_play();
		m_oPlayListDoc.get_data(index)->set_play(false);

		index = m_oPlayListView.play_pre();
		if (index == -1)
		{
			index = m_oPlayListView.near_index(get_play());
		}
		if (index == -1)
		{
			return false;
		}
		
		m_oPlayListView.set_play(index);
		return true;
	}	
	
	const char *get_curfile()
	{
		int index = m_oPlayListView.get_play();
		wxf_item_view *pView = m_oPlayListView.get_item(index);
		if (pView)
		{
			return pView->get_data()->get_filepath().c_str();
		}
		else
		{
			return NULL;
		}
	}
	
	int get_play(void) 
	{
		return m_oPlayListView.get_play();
	}

	void set_play(int no)
	{
		int index = m_oPlayListView.get_play();
		if (index != -1)
		{
			m_oPlayListDoc.get_data(index)->set_play(false);
		}		

		m_oPlayListView.set_play(no);
	}

	void select_item(int no)
	{
		wxf_str temp;
		temp.format("[wxf_playlist]:设置选择号:%d",no);
		GLOG.trace().add(temp.c_str());
		m_oPlayListView.select_item(no);
	}

	void del_select()
	{
		int index = m_oPlayListView.del_item();
		
		if (get_fav())
		{
			m_oPlayListDoc.get_data(index)->set_fav(false);
		}
		else
		{
			m_oPlayListDoc.delete_item(index);
		}
	}

	wxf_item_view *add_item(const char *file_name)
	{
		int index = m_oPlayListDoc.add_item(file_name,get_fav());
		return m_oPlayListView.add_item(m_oPlayListDoc.get_data(index),index);
	}
	bool load_list(const char *file_name)
	{
		if (!m_oPlayListDoc.load_list(file_name))
		{
			return false;
		}
	
		m_oPlayListView.add_doc(&m_oPlayListDoc,get_fav());
		return true;
	}
	void save_list(const char *file_name)
	{
		m_oPlayListDoc.save_list(file_name);
	}
	int get_order()
	{
		return m_oPlayListView.get_order();
	}
	void set_order(int loopmode, CButtonUI *pOrder)
	{
		m_oPlayListView.set_order(loopmode,pOrder);
	}
	void click_item(int no,TNotifyUI *psender)
	{
		wxf_item_view *pView = m_oPlayListView.get_item(no);
		if (pView)
		{
			bool bRst = pView->click_item(psender);
			if (bRst && get_fav())
			{
				m_oPlayListView.del_item(no);
			}
		}
	}
	bool change_list(bool bIsFav)
	{
		if (bIsFav != get_fav())
		{
			set_fav(bIsFav);
			int index = m_oPlayListView.get_play();

			m_oPlayListView.add_doc(&m_oPlayListDoc,bIsFav);
			//select_item(index);
		}
		return get_fav();
	}
	bool get_fav()
	{
		return m_oPlayListDoc.get_fav();
	}
	void set_fav(bool bFav)
	{
		m_oPlayListDoc.set_fav(bFav);
	}
protected:
	wxf_playlist_view m_oPlayListView;
	wxf_playlist_doc m_oPlayListDoc;

	//bool m_bIsFav;
};


