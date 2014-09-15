#pragma once
typedef struct wxf_playlist_item_t wxf_plit;
struct wxf_playlist_item_t
{
	CLabelUI *listno;
	CLabelUI *songname;
	CLabelUI *singername;
	CLabelUI *mv_flag;
	CControlUI *nop;
	CHorizontalLayoutUI *phl;
	CListContainerElementUI *ele;
	class wxf_plitem *next;
};
class wxf_plitem
{
public:
	wxf_plitem(void);
	~wxf_plitem(void);
	
private:
	void init();
	void deinit();
public:
	void set_index(int idx);
	void set_file(wxf_str file);
	void set_id3(TID3InfoEx *id3);
	
	TID3InfoEx *get_id3() {return id3_info;}
	wxf_str get_file() {return file_path;}
	int get_index() {return index;}
	CListContainerElementUI *get_ele() {return item.ele;}

	wxf_plitem *get_next() {return item.next;}
	void add_next(wxf_plitem *next) {item.next = next;}
private:
	int index;
	wxf_plit item;
	wxf_str file_path;
	TID3InfoEx *id3_info;	
};

