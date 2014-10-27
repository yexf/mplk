/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : wxf_playctl.cpp
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/30
**
** Description:²¥·Å¿ØÖÆÆ÷
**************************************************************************************/ 
#include "wxf_app.h"


wxf_playctl::wxf_playctl(CSliderUI *playprog,wxf_log *log)
{
	m_playprog = playprog;
	m_log = log;
	m_player = new wxf_player(log);
	m_bplay = false;
	m_bseek = false;
	m_bdraw = false;
}

wxf_playctl::~wxf_playctl(void)
{
	m_playprog = NULL;
	m_log = NULL;
	delete m_player;
	m_player = NULL;
	m_bplay = false;
	m_bseek = false;
}

int wxf_playctl::set_vol(int val,int max)
{
	double temp;

	temp = val;

	temp = temp / max;

	return m_player->vol_pos(temp);
}

int wxf_playctl::set_play(int val,int max)
{
	double temp;

	temp = val;

	temp = temp / max;
	m_bseek = true;
	int ret = m_player->play_pos(temp);
	m_bseek = false;

	return ret;
}

int wxf_playctl::get_pos(int max)
{
	if (m_bseek) return wxf_err;
	TStreamInfo info;

	info = m_player->get_info();

	uint64 temp = m_player->get_pos();
	temp = temp * max;
	temp = temp / info.Length.samples;

	return (int)temp;
}

int wxf_playctl::play(wxf_playlist *plist)
{
	int ret = wxf_succ;
	
	if (m_bplay)
	{
		return wxf_succ;
	}
	if (plist->get_play() == -1)
	{
		plist->set_play(0);
	}
	ret = play(m_player,plist->get_curfile());
	
	plist->select_item(plist->get_play()-1);
	return ret;
}
int wxf_playctl::stop(wxf_playlist *plist)
{
	int ret = wxf_succ;

	ret = m_player->stop();

	m_bplay = false;
	return ret;
}
int wxf_playctl::over(wxf_playlist *plist)
{
	int ret = wxf_err;
	if (m_player->get_state() != wxf_player::EM_OVER)
	{
		return wxf_err;
	}

	ret = m_player->stop();
	m_bplay = false;

	if (plist->play_next())
	{
		ret = play(m_player,plist->get_curfile());
		plist->select_item(plist->get_play()-1);
		m_bplay = true;
	}

	return ret;
}
int wxf_playctl::pause(wxf_playlist *plist)
{
	int ret = wxf_succ;
	
	ret = m_player->pause();

	m_bplay = false;
	return ret;
}
int wxf_playctl::pre(wxf_playlist *plist)
{
	int ret = wxf_err;

	ret = m_player->stop();
	m_bplay = false;

	if (plist->play_pre())
	{
		ret = play(m_player,plist->get_curfile());
		plist->select_item(plist->get_play()-1);
		m_bplay = true;
	}
	
	return ret;
}
int wxf_playctl::next(wxf_playlist *plist)
{
	int ret = wxf_err;

	ret = m_player->stop();
	m_bplay = false;

	if (plist->play_next())
	{
		ret = play(m_player,plist->get_curfile());
		plist->select_item(plist->get_play()-1);
		m_bplay = true;
	}

	return ret;
}
int wxf_playctl::muted(int val,int max)
{
	int ret = wxf_succ;

	m_player->vol_pos(0.0);
	m_player->set_muted();

	return ret;
}
int wxf_playctl::mute(int val,int max)
{
	int ret = wxf_succ;

	double temp = val;
	temp = temp / max;
	
	m_player->set_mute();
	m_player->vol_pos(temp);

	return ret;
}

int wxf_playctl::len_time(CLabelUI *plabel)
{
	int ret = wxf_succ;
	wxf_str str;
	int m,s;
	TStreamTime time;
	time = m_player->get_info().Length;

	s = time.hms.second;
	m = time.hms.minute;

	str.format("%02d:%02d",m,s);

	plabel->SetText(str.c_str());
	return ret;
}
int wxf_playctl::cur_time(CLabelUI *plabel)
{
	int ret = wxf_succ;
	wxf_str str;
	int m,s;
	TStreamTime time;
	m_player->get_time(time);

	s = time.hms.second;
	m = time.hms.minute;

	str.format("%02d:%02d",m,s);
	plabel->SetText(str.c_str());
	return ret;
}
int wxf_playctl::draw_fft(HWND hWnd,CControlUI *pctl)
{
	int ret = wxf_succ;
	
	if (m_bdraw)
	{
		const RECT &temp = pctl->GetPos();

		int x,y,w,h;

		x = temp.left;
		y = temp.top;
		w = temp.right - x;
		h = temp.bottom - y;

		m_player->draw_fft(hWnd,x,y,w,h);

	}

	return ret;
}

int wxf_playctl::play(wxf_player *player,const char *file_name)
{
	int ret = wxf_succ;

	if (player->get_state() != wxf_player::EM_PAUSE)
		ret = player->ready(file_name);

	ret += player->play();

	if (ret == wxf_succ)
	{
		
		m_bplay = true;
		return ret;
	}
	return ret;
}