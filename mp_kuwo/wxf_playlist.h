#pragma once

class CKuWo;
class wxf_player;

typedef	enum wxf_loop_mode
{
	EM_LISTLOOP,				//列表循环
	EM_LISTPLAY,				//列表播放
	EM_SONGLOOP,				//单曲循环
	EM_RANDOMLOOP				//随机循环
}wxf_lm;
class wxf_playlist
{
public:
	wxf_playlist(void);
	~wxf_playlist(void);
	void close();
	wxf_playlist(CListUI *plist):m_plist(plist) {init();}
	wxf_plitem *get_pcur(){return pcur;}
	int get_size(){return item_num;}
	wxf_player *get_player(){return m_player;}
public:
	wxf_plitem *find(int index);
	void *del_item(int index);


	wxf_plitem *add(const wxf_str &file);
	int select_item(wxf_plitem *pitem);
	double get_pos(void);
	wxf_plitem *next_song(wxf_plitem *pitem);
	void play_next(void);
public:
	void select_hdl(int index);
	void save(wxf_file_t fp);
protected:
private:
	void init();
	void deinit();
	void insert(wxf_plitem *temp);

	wxf_plitem *head;
	wxf_plitem *tail;
	wxf_plitem *pcur;

	wxf_lm loop_mode;
	int item_num;

	CListUI *m_plist;
	wxf_player *m_player;

};

