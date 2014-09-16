/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : wxf_player.cpp
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/30
**
** Description:播放器 重新封装一些播放功能
**************************************************************************************/ 
#include "pch.h"
wxf_player *wxf_player::g_player = NULL;

int wxf_player::ready(const char *file_name)
{
	int ret = 0;
	if (m_state == EM_INIT)
	{
		ret = m_player->OpenFile(file_name,sfAutodetect);
		if (ret == 0)
		{
			wxf_logout(ret,"ready");
			return wxf_err;
		}
		
		m_player->GetStreamInfo(&m_info);
		wxf_logout(1,"play","从 EM_INIT切换到EM_READY");
		m_state = EM_READY;
	}
	else if (m_state == EM_STOP)
	{
		ret = m_player->OpenFile(file_name,sfAutodetect);
		if (ret == 0)
		{
			wxf_logout(ret,"ready");
			return wxf_err;
		}
		
		m_player->GetStreamInfo(&m_info);
		wxf_logout(1,"play","从 EM_STOP切换到EM_READY");
		m_state = EM_READY;
	}
	else 
	{
		wxf_str temp;
		temp.format("状态错误,错误状态为%d",m_state);
		wxf_logout(ret,"ready",temp.data());
		return wxf_err;
	}
	wxf_str temp;
	temp.format("准备成功,准备播放%s",file_name);
	wxf_logout(ret,"play",temp.data());

	return wxf_succ;	
}
int wxf_player::play()
{
	int ret = 0;
	if (m_state == EM_PAUSE)
	{
		ret = m_player->Resume();
		if (ret == 0)
		{
			wxf_logout(ret,"play");
			return wxf_err;
		}
		m_state = EM_PLAY;
	}
	else if (m_state == EM_READY)
	{
		ret = m_player->Play();
		if (ret == 0)
		{
			wxf_logout(ret,"play");
			return wxf_err;
		}
		m_state = EM_PLAY;
	}
	else
	{
		wxf_str temp;
		temp.format("状态错误,错误状态为%d",m_state);
		wxf_logout(0,"play",temp.data());
		return wxf_err;
	}
	wxf_logout(ret,"play","播放成功");		
	return wxf_succ;	
}
int wxf_player::stop()
{
	int ret = 0;
	if (m_state == EM_READY)
	{
		ret = m_player->Close();
		if (ret == 0)
		{
			wxf_logout(ret,"stop");
			return wxf_err;
		}
		m_state = EM_STOP;
		wxf_logout(ret,"stop","close成功");
		wxf_logout(ret,"stop","从ready状态转到stop状态");
		
	}
	else if (m_state == EM_PAUSE)
	{
		ret = m_player->Close();
		if (ret == 0)
		{
			wxf_logout(ret,"stop");
			return wxf_err;
		}
		wxf_logout(ret,"stop","close成功");

		ret = m_player->Stop();
		if (ret == 0)
		{
			wxf_logout(ret,"stop");
			return wxf_err;
		}
		m_state = EM_STOP;
		wxf_logout(ret,"stop","从pause状态转到stop状态");				
	}
	else if (m_state == EM_PLAY)
	{
		ret = m_player->Close();
		if (ret == 0)
		{
			wxf_logout(ret,"stop");
			return wxf_err;
		}
		wxf_logout(ret,"stop","close成功");

		ret = m_player->Stop();
		if (ret == 0)
		{
			wxf_logout(ret,"stop");
			return wxf_err;
		}
		m_state = EM_STOP;
		wxf_logout(ret,"stop","从play状态转到stop状态");			
	}
	else if (m_state == EM_OVER)
	{
		ret = m_player->Close();
		if (ret == 0)
		{
			wxf_logout(ret,"stop");
			return wxf_err;
		}
		wxf_logout(ret,"stop","close成功");

		ret = m_player->Stop();
		if (ret == 0)
		{
			wxf_logout(ret,"stop");
			return wxf_err;
		}
		m_state = EM_STOP;
		wxf_logout(ret,"stop","从over状态转到stop状态");
	}
	else if(m_state == EM_INIT) 
	{
		wxf_logout(1,"stop","当前状态为 EM_INIT");
		return wxf_succ;
	}
	else if(m_state == EM_STOP)
	{
		wxf_logout(1,"stop","当前状态为 EM_STOP");
		return wxf_succ;
	}
	wxf_logout(ret,"stop","stop 成功");
	return wxf_succ;	
}
int wxf_player::pause()
{
	if (m_state == EM_PLAY)
	{
		m_player->Pause();
		m_state = EM_PAUSE;
	}
	if (m_state == EM_PAUSE)
	{
		wxf_logout(1,"pause","当前状态为 EM_PAUSE");
		return wxf_succ;
	}
	else
	{
		wxf_str temp;
		temp.format("状态错误,错误状态为%d",m_state);
		wxf_logout(0,"pause",temp.data());
		return wxf_err;
	}
	return wxf_succ;	
}

int wxf_player::play_pos(double pos)
{
	wxf_str str;
	int ret,buf,temp;
	TStreamTime time;

	str.format("pos : %f",pos);
	wxf_logout(1,"play_pos",str.data());

	if (m_state == EM_OVER)
	{
		wxf_logout(1,"play_pos","EM_OVER");
	}

	buf = m_info.Length.samples;
	pos = pos * buf;
	temp = (int)pos;

	time.samples = temp;
	
	ret = m_player->Seek(tfSamples,&time,smFromBeginning);

	if (ret == 0)
	{
		wxf_logout(0,"play_pos");
		return wxf_err;
	}
	
	return wxf_succ;
}

int wxf_player::vol_pos(double pos)
{
	if (m_bmute)
	{
		return wxf_succ;
	}
	if (pos > 1)
	{
		wxf_str temp;
		temp.format("pos错误,pos为%g",pos);
		wxf_logout(0,"vol_pos",temp.data());
		return wxf_err;
	}
	int temp;
	pos = pos * 100;
	temp = (int) pos;

	m_player->SetPlayerVolume(temp,temp);

	return wxf_succ;
}
int wxf_player::get_pos()
{
	TStreamTime time;

	m_player->GetPosition(&time);

	return time.samples;
}
int wxf_player::get_time(TStreamTime &time)
{
	if (m_state != EM_PLAY)
	{
		wxf_str temp;
		temp.format("状态错误,错误状态为%d",m_state);
		wxf_logout(0,"play_pos",temp.data());
		return wxf_err;
	}

	m_player->GetPosition(&time);

	return wxf_succ;
}

int wxf_player::set_muted()
{
	int ret = wxf_succ;

	m_bmute = true;

	return ret;
}
int wxf_player::set_mute()
{
	int ret = wxf_succ;

	m_bmute = false;

	return ret;
}
int wxf_player::draw_fft(HWND hWnd,int nX,int nY,int nWidth,int nHeight)
{
	int ret = wxf_succ;

	ret = m_player->DrawFFTGraphOnHWND(hWnd,nX,nY,nWidth,nHeight);

	if (ret == 0)
	{
		wxf_logout(ret,"draw_fft");
		return wxf_err;
	}

	if (nWidth != m_nXfftbk || nHeight != m_nYfftbk)
	{
		HBITMAP temp;
		temp = CRenderEngine::StretchImage(m_hfftbk,nWidth,nHeight,m_nXfftbk,m_nYfftbk);
		if (temp != NULL)
		{
			::DeleteObject(m_hfftbk);	
			m_hfftbk = temp;
			m_nXfftbk = nWidth;
			m_nYfftbk = nHeight;
			m_player->SetFFTGraphParam(gpBgBitmapHandle,(int)m_hfftbk);
		}
	}

	return ret;
}

int wxf_player::LoadID3(const char *filename,TID3Info &id3_info)
{
	if (g_player == NULL)
	{
		wxf_memset(&id3_info,0,sizeof(id3_info));
		return wxf_err;
	}
	else
	{
		return g_player->loadid3(filename,id3_info);
	}
}
void wxf_player::init(void)
{
	int ret = 0;

	m_player = CreateZPlay();
	if (m_player == NULL)
	{
		wxf_logout(0,"init");
		m_state = EM_ERR;
	}

	ret = m_player->SetCallbackFunc(libzplay_hdl,MsgStop,this);
	if (ret == 0)
	{
		wxf_logout(ret,"init");
		m_state = EM_ERR;
	}
	m_state = EM_INIT;
	wxf_logout(1,"init","init 成功");

	g_player = this;

	
	// set graph type to AREA, left channel on top
	m_player->SetFFTGraphParam(gpGraphType, gtAreaLeftOnTop);
	// set linear scale
	m_player->SetFFTGraphParam(gpHorizontalScale, gsLinear);

	m_player->SetFFTGraphParam(gpTransparency , 10);
	m_player->SetFFTGraphParam(gpFrequencyScaleVisible , 0);
	m_player->SetFFTGraphParam(gpDecibelGridVisible  , 0);
	m_player->SetFFTGraphParam(gpDecibelScaleVisible  , 0);
	m_player->SetFFTGraphParam(gpFrequencyGridVisible  , 0);
	m_player->SetFFTGraphParam(gpSubgrid , 1);
	m_player->SetFFTGraphParam(gpBgBitmapVisible  , 1);

	m_hfftbk = CRenderEngine::LoadImage("fftbk.jpg",&m_nXfftbk,&m_nYfftbk);
	if (m_hfftbk) m_player->SetFFTGraphParam(gpBgBitmapHandle,(int)m_hfftbk);
}

void wxf_player::deinit(void)
{
	int ret = wxf_succ;
	g_player = NULL;
	if (m_state == EM_INIT)
	{
		m_player->Release();
		m_player = NULL;
	}
	else
	{
		if (m_state != EM_STOP) 
			ret = stop();
		m_player->Release();
		m_player = NULL;
	}
	if (ret == wxf_err)
	{
		wxf_logout(0,"deinit","stop错误");
	}
	else
	{
		wxf_logout(1,"deinit","deinit 成功");
	}
	m_log = NULL;
	m_state = EM_DEINIT;
	::DeleteObject(m_hfftbk);
}
int wxf_player::loadid3(const char *filename,TID3Info &id3_info)
{
	int ret = 0;

	if (wxf_pathfile_exist(filename))
	{
		wxf_str temp;
		temp.format("文件%s 不存在",filename);
		wxf_logout(ret,"loadid3",temp.data());
	}

	ret = m_player->LoadFileID3(filename,sfAutodetect,id3Version1,&id3_info);
	if (ret == 0)
	{
		wxf_logout(ret,"play");
		return wxf_err;
	}

	return wxf_succ;
}


int __stdcall wxf_player::libzplay_hdl(void* instance,
	void *user_data, 
	TCallbackMessage message,
	unsigned int param1,
	unsigned int param2)
{
	wxf_player *player;

	player = static_cast<wxf_player *>(user_data);
	if (player->m_state == EM_PLAY)
	{
		player->m_state = EM_OVER;
	}

	return 0;
}



