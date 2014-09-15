/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : wxf_playctl.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/30
**
** Description:²¥·Å¿ØÖÆÆ÷
**************************************************************************************/ 

#pragma once
class wxf_playctl
{
public:
	wxf_playctl(CSliderUI *playprog,wxf_log	*log);
	~wxf_playctl(void);
	bool get_state() {return m_bplay;}
	void set_draw(bool bdraw) {m_bdraw = bdraw;}
public:
	int set_vol(int val,int max);
	int set_play(int val,int max);
	int get_pos(int max);

	int play(wxf_playlist *plist);
	int stop(wxf_playlist *plist);
	int over(wxf_playlist *plist);

	int pause(wxf_playlist *plist);
	int pre(wxf_playlist *plist);
	int next(wxf_playlist *plist);

	int muted(int val,int max);
	int mute(int val,int max);

	int len_time(CLabelUI *plabel);
	int cur_time(CLabelUI *plabel);

	int draw_fft(HWND hWnd,CControlUI *pctl);
private:
	int play(wxf_player *player,const char *file_name);
private:
	CSliderUI *m_playprog;
	wxf_player *m_player;
	wxf_log *m_log;
	bool m_bseek;
	bool m_bplay;
	bool m_bdraw;

	inline void wxf_logout(int ret,const char *inf,char *msg)
	{
		wxf_log	*log;

		log = m_log;

		if (ret == 0)
		{
			log->FormatAdd(
				wxf_log::error,
				"wxf_playctl :[%s]=>>%s",
				inf,msg);
		}
		else
		{
			log->FormatAdd(
				wxf_log::trace,
				"wxf_playctl :[%s]=>>%s",
				inf,msg);
		}
	}
};

