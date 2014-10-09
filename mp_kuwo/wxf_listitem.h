/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : wxf_listitem.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/30
**
** Description: 播放列表条目 控制每条选项的外观
**************************************************************************************/ 
#pragma once
class wxf_listitem
{
public:
	wxf_listitem(void);
	~wxf_listitem(void);
	wxf_listitem(int no,const char *file_name);
	int get_no(void) {return m_no;}
	int set_no(int no);

	const char *get_file(void) {return m_path.c_str();}
	int set_file(const char *file_name);

	void set_next(wxf_listitem *next) {m_next = next;}
	wxf_listitem *get_next(){return m_next;}

	void set_like(bool Islike);
	bool get_like() {return m_bIsLike;}
	void change_like();

	void set_play(bool IsPlay);
	void click_item(TNotifyUI *psender);
public:
	int add_to(CListUI *plist);
	int remove_form(CListUI *plist);
private:
	void init();
	void deinit();
	
private:
	CLabelUI *m_listno;
	CLabelUI *m_like;
	CLabelUI *m_songname;
	CLabelUI *m_singername;
	CLabelUI *m_mvflag;

	CControlUI *m_nop;

	CHorizontalLayoutUI *m_phl;
	CListContainerElementUI *m_item;

	int m_no;
	wxf_str m_path;
	TID3Info m_id3;	
	DWORD m_defBKColor;
	DWORD m_PlayBKColor;

	bool m_bIsLike;

	wxf_listitem *m_next;
};


