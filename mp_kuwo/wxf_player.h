#pragma once



class wxf_player
{
	
public:
	wxf_player(void);
	~wxf_player(void);

public:
	int play(const char *file_name);
	int play(wxf_plitem *ppli);
	double get_pos(void);
	int get_sample(void);
	void seek(double pos);
	void set_vol(double vol);
	void close();

	int get_state() {return m_state;}
	TID3InfoEx *get_id3(const char *file_name);
	wxf_playlist *get_playlist(){return m_playlist;}
private:
	ZPlay *m_player;
	int m_state;
	wxf_playlist *m_playlist;

	TStreamInfo m_info;
	int stop_ps;
public:
	static int __stdcall stop_hdl(void* instance,
		void *user_data, 
		TCallbackMessage message,
		unsigned int param1,
		unsigned int param2);
};

