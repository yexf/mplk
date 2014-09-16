/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : wxf_player.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/30
**
** Description:播放器 重新封装一些播放功能 为播放控制器提供播放的功能
**************************************************************************************/ 
#pragma once

class wxf_kuwo;
//#define KUWOLOG							(*(wxf_kuwo::g_kuwo->m_log))

class wxf_player
{
public:
	enum wxf_player_em
	{
		EM_ERR,
		EM_UNINIT,
		EM_INIT,
		EM_READY,
		EM_PLAY,
		EM_PAUSE,
		EM_STOP,
		EM_OVER,
		EM_DEINIT
	};
public:
	wxf_player(wxf_log *log):m_log(log),m_bmute(false){init();}
	~wxf_player(void){deinit();}

	wxf_player_em get_state(){return m_state;}
	void set_log(wxf_log *log) {m_log = log;}
	TStreamInfo &get_info() {return m_info;}
public:
	int ready(const char *filename);
	int play();
	int stop();
	int pause();

	int play_pos(double pos);
	int vol_pos(double pos);
	int get_pos();
	int get_time(TStreamTime &Stime);

	int set_muted();
	int set_mute();

	int draw_fft(HWND hWnd,int nX,int nY,int nWidth,int nHeight);

	static int LoadID3(const char *filename,TID3Info &id3_info);
private:
	void init();
	void deinit();

	int loadid3(const char *filename,TID3Info &id3_info);

	static int __stdcall libzplay_hdl(void* instance,
		void *user_data, 
		TCallbackMessage message,
		unsigned int param1,
		unsigned int param2);
	inline void wxf_logout(int ret,const char *inf,char *msg = NULL)
	{
		wxf_log	*log;

		log = m_log;
		if (m_log == NULL) return;
		if (msg == NULL)
		{
			msg = m_player->GetError();
		}
		if (ret == 0)
		{
			log->FormatAdd(
				wxf_log::error,
				"wxf_player:[%s]=>>%s",
				inf,msg);
		}
		else
		{
			log->FormatAdd(
				wxf_log::trace,
				"wxf_player:[%s]=>>%s",
				inf,msg);
		}
	}

private:
	wxf_player_em m_state;
	ZPlay *m_player;
	TStreamInfo m_info;
	
	wxf_log *m_log;
	bool m_bmute;
	
	int m_nXfftbk;
	int m_nYfftbk;
	HBITMAP m_hfftbk;
	static wxf_player *g_player;
};

