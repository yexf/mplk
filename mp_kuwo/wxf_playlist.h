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
class wxf_playlist
{
public:
	enum loop_mode
	{
		EM_LISTLOOP = 0,			//列表循环
		EM_LISTPLAY,				//列表播放
		EM_SONGLOOP,				//单曲循环
		EM_RANDOMLOOP,				//随机循环
		EM_MODEMAX
	};
public:
	wxf_playlist(CListUI *plist):m_plist(plist),m_log(NULL){init();}
	~wxf_playlist(void){deinit();}
	
public:
	void set_log(wxf_log *log) {m_log = log;}

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
	int add_item(const char *file_name);

	wxf_listitem *find_pre(int no);
	int del_select();

	void save_list(const char *file_name);
	bool load_list(const char *file_name);

	int get_order();
	void set_order(int loopmode, CButtonUI *pOrder);

	void click_item(int no,TNotifyUI *psender);
protected:
	CListUI *m_plist;
private:
	wxf_listitem *m_head;
	wxf_listitem *m_tail;
	int m_nplay;
	int m_nselect;

	int m_loopmode;
	int m_itemnum;

	wxf_log	*m_log;
	
private:
	void init();
	void deinit();

	void push_item(wxf_listitem *pitem);


	inline void wxf_logout(int ret,const char *inf,char *msg)
	{
		wxf_log	*log;

		log = m_log;

		if (ret == 0)
		{
			log->FormatAdd(
				wxf_log::error,
				"wxf_playlist :[%s]=>>%s",
				inf,msg);
		}
		else
		{
			log->FormatAdd(
				wxf_log::trace,
				"wxf_playlist :[%s]=>>%s",
				inf,msg);
		}
	}
};


