#include "ybyt.h"
wxf_player::wxf_player(void)
{
	m_player = CreateZPlay();
	m_player->SetCallbackFunc(stop_hdl,MsgStop,this);
	m_state = EM_INIT;
	m_playlist = NULL;
	stop_ps = 0;
}
wxf_player::~wxf_player(void)
{
	//delete m_player;
	m_player = NULL;
	m_state = EM_UNINIT;
}
void wxf_player::close()
{
	if (m_state == EM_RUN)
	{
		m_player->Stop();
	}
	m_player->Close();
	m_player->Release();
}
int wxf_player::get_sample(void)
{
	TStreamTime temp;
	m_player->GetPosition(&temp);
	if (m_info.Length.samples == 0)
	{
		return 0;
	}
	return temp.samples;
}
double wxf_player::get_pos()
{
	if (stop_ps) return -1;

	TStreamTime temp;
	m_player->GetPosition(&temp);
	if (m_info.Length.samples == 0)
	{
		return 0.0;
	}
	double ret = m_info.Length.samples;
	ret = temp.samples / ret;
	return ret;
}
void wxf_player::seek(double pos)
{
	/*TStreamStatus status;
	m_player->GetStatus(&status);*/

	stop_ps = 1;

	int temp;
	double v;
	v = pos * m_info.Length.samples;
	temp = (int)v;
	TStreamTime time;

	time.samples = temp;
	m_player->Pause();
	m_player->Seek(tfSamples,&time,smFromBeginning);
	while (get_sample() != time.samples);
	m_player->Resume();

	stop_ps = 0;
}
void wxf_player::set_vol(double vol)
{
	int temp;
	vol = vol * 100;
	temp = (int) vol;
	m_player->SetPlayerVolume(temp,temp);
}
int wxf_player::play(const char *file_name)
{
	if (m_state == EM_RUN)
	{
		m_player->Stop();
	}

	m_player->OpenFile(file_name,sfAutodetect);

	// get song length
	m_player->GetStreamInfo(&m_info);

	m_player->Play();
	m_state = EM_RUN;
	return m_state;	
}

int wxf_player::play(wxf_plitem *ppli)
{
	wxf_str file;

	file = ppli->get_file();
	play(file.c_str());

	return m_state;
}

TID3InfoEx *wxf_player::get_id3(const char *file_name)
{
	TID3InfoEx *id3;
	id3 = new TID3InfoEx;
	m_player->LoadFileID3Ex(file_name,sfAutodetect,id3,1);
	return id3;
}
int __stdcall wxf_player::stop_hdl(void* instance,
	void *user_data, 
	TCallbackMessage message,
	unsigned int param1,
	unsigned int param2)
{
	wxf_player *player;
	wxf_playlist *playlist;
	player = static_cast<wxf_player *>(user_data);
	playlist = player->m_playlist;

	player->m_state = EM_STOP;

	return 0;
}