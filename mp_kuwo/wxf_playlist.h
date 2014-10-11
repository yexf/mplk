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
	void save_list(const char *file_name);
	bool load_list(const char *file_name);

	wxf_item_data *get_next();
	wxf_item_data *get_pre();
	wxf_item_data *get_cur();

public:
	int get_loopmode()
	{
		return m_emLoopmode;
	}
	void set_loopmode(int loopmode)
	{
		m_emLoopmode = loopmode;
	}
protected:
	vector<wxf_item_data> m_vecItem;

	int m_iNextPlayNo;
	int m_iCurPlayNo;
	int m_iPrePlayNo;

	int m_emLoopmode;
};

class wxf_playlist_view
{
public:
	wxf_playlist_view(CListUI *plist)
		:m_pList(plist),m_pDoc(NULL),m_iSelectIndex(-1),m_iPlayIndex(-1)
	{

	}
public:
	int add_item(wxf_item_data *pData,int index);
	void del_item();

	void set_select(int index);
	void set_play(int index);

	int get_order();
	void set_order(int loopmode, CButtonUI *pOrder);

	int update_next(int playindex = -1);	
	int get_mapat(int iNo);

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
	map<int,wxf_item_view> m_mapItem;

	int m_iSelectIndex;		//-1
	int m_iPlayIndex;		//-1

	wxf_playlist_doc *m_pDoc;
};

class wxf_playlist
{
public:
	
public:
	wxf_playlist(CListUI *plist):m_plist(plist),m_bIsFav(false){init();}
	~wxf_playlist(void){deinit();}
	
public:

	void set_select(int no) {m_nselect = no;}
	
	int get_select(void) {return m_nselect;}
	
	int get_play(void) {return m_nplay;}
	void set_play(int no);

	wxf_listitem *get_pcur(void);
	wxf_listitem *get_next(void);

	wxf_listitem *get_pre(wxf_listitem *pitem);
	wxf_listitem *get_next(wxf_listitem *pitem);

	void select_item(wxf_listitem *pitem);
	wxf_listitem *find_item(int no);
	wxf_listitem *add_item(const char *file_name);

	wxf_listitem *find_pre(int no);
	int del_select();

	void save_list(const char *file_name);
	bool load_list(const char *file_name);

	int get_order();
	void set_order(int loopmode, CButtonUI *pOrder);

	void click_item(int no,TNotifyUI *psender);
	bool change_list(bool bIsFav);
protected:
	CListUI *m_plist;
private:
	wxf_listitem *m_head;
	wxf_listitem *m_tail;

	std::vector<wxf_listitem *> m_vecDisplay;
	int m_nplay;
	int m_nselect;

	int m_loopmode;
	int m_itemnum;

	bool m_bIsFav;

	wxf_log	*m_log;
	
private:
	void init();
	void deinit();

	void push_item(wxf_listitem *pitem);


	inline void wxf_logout(int ret,const char *inf,char *msg)
	{
		if (ret == 0)
		{
			GLOG.error().print("wxf_playlist :[%s]=>>%s",inf,msg);
		}
		else
		{
			GLOG.trace().print("wxf_playlist :[%s]=>>%s",inf,msg);
		}
	}
};


