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

class wxf_item_data
{
public:
	wxf_item_data():m_strArtist("(未知的艺术家)"),m_strTitle("(未知的标题)"),
		m_bIsFav(false),m_bIsPlay(false),m_strFilePath(""),m_pUserData(NULL)
	{
		
	}
	
	void set_userdata(void *pVoid)
	{
		m_pUserData = pVoid;
	}

	void *get_userdata() const
	{
		return m_pUserData;
	}

	const wxf_str &get_filepath() const
	{
		return m_strFilePath;
	}

public:

	void set_filepath(const char *strFilePath);

	void set_no(int iNo);

	void set_fav(bool bFav);

	void set_play(bool bPlay);

	const wxf_str &get_artist() const 
	{
		return m_strArtist;
	}
	const wxf_str &get_title() const 
	{
		return m_strTitle;
	}
	bool get_isfav() const 
	{
		return m_bIsFav;
	}
	bool get_isplay() const 
	{
		return m_bIsPlay;
	}
	
protected:
	wxf_str m_strArtist;
	wxf_str m_strTitle;
	bool m_bIsFav;
	bool m_bIsPlay;

private:
	wxf_str m_strFilePath;
	void *m_pUserData;
};

class wxf_item_view
{
public:
	wxf_item_view(void):m_bIsInit(false),m_pData(NULL)
	{
		m_siNum++;
		init();
	}
	~wxf_item_view(void)
	{
		deinit();
		m_siNum--;
	}
	operator CControlUI*()
	{
		return m_pItem;
	}
	wxf_item_data* get_data()
	{
		return m_pData;
	}
	
public:

	void click_item(TNotifyUI *psender);
	void update_view(wxf_item_data *pData);
	
protected:
	static int m_siNum;
private:

	void init();
	void deinit();

private:
	CLabelUI *m_pListno;

	CLabelUI *m_pFav;
	CLabelUI *m_pSongname;
	CLabelUI *m_pSingername;
	CLabelUI *m_pMVflag;

	CHorizontalLayoutUI *m_pLayoutMgr;
	CListContainerElementUI *m_pItem;

	DWORD m_dwDefBKColor;
	DWORD m_dwPlayBKColor;

	bool m_bIsInit;
	int m_iCurNo;
	wxf_item_data *m_pData;
};


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


