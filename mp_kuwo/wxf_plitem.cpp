#include "ybyt.h"


wxf_plitem::wxf_plitem(void)
{
	init();
	id3_info = NULL;
}

wxf_plitem::~wxf_plitem(void)
{
	deinit();
	if (id3_info)
	{
		delete id3_info;
	}
	
}

void wxf_plitem::init()
{
	item.ele = new CListContainerElementUI;
	item.phl = new CHorizontalLayoutUI;
	item.ele->Add(item.phl);
	item.ele->SetAttribute("height","23");

	item.listno = new CLabelUI;
	item.songname = new CLabelUI;
	item.singername = new CLabelUI;
	item.mv_flag = new CLabelUI;
	item.nop = new CControlUI;

	item.listno->SetAttribute("width","20");
	item.singername->SetAttribute("width","64");
	item.mv_flag->SetAttribute("width","20");
	item.nop->SetAttribute("width","4");
	item.listno->SetAttribute("align","right");

	item.songname->SetAttribute("endellipsis","true");
	item.singername->SetAttribute("endellipsis","true");

	item.phl->Add(item.listno);
	item.phl->Add(item.nop);
	item.phl->Add(item.songname);
	item.phl->Add(item.singername);
	item.phl->Add(item.mv_flag);

	item.listno->SetFont(14);
	item.singername->SetFont(14);
	item.songname->SetFont(14);
	item.songname->SetAttribute("textcolor","#7f1f1f1f");
	item.singername->SetAttribute("textcolor","#7f1f1f1f");

	item.next = NULL;
}
void wxf_plitem::deinit()
{
	delete item.listno;
	delete item.songname;
	delete item.singername;
	delete item.mv_flag;

	delete item.phl;
	delete item.ele;
}

void wxf_plitem::set_index(int idx)
{
	index = idx;
	char buf[16] = {0};
	sprintf_s(buf,16,"%d",index+1);
	item.listno->SetText(buf);
	item.ele->SetTag(idx);
}
void wxf_plitem::set_file(wxf_str file)
{
	file_path = file;
}


/*
* get the file name in path, "a/b/c/d/e.f" --> "e"
*
* @param
*		fullpath: full path string
*		name: first directory name string
* @return - 0: fullpath is blank or not '/' ended, <>0, the name string length
*/
int wxf_catch_name_in_path(const char *fullpath, char *name)
{
	int i;
	int len,temp;
	len = (int)strlen(fullpath);

	for (i=len; i--!=0;)
	{
		if (fullpath[i] == '.')
		{
			temp = i;
			break;
		}
	}
	for (i=len; i--!=0;)
	{
		if (fullpath[i] == '\\' || fullpath[i] == '/')
		{
			temp = temp - i;
			break;
		}
	}
	strncpy(name, fullpath+i+1, temp-1);

	return i+1;
}
void wxf_plitem::set_id3(TID3InfoEx *id3)
{
	if (id3_info != NULL)
	{
		delete id3_info;
	}
	id3_info = id3;
	if (id3)
	{
		item.songname->SetText(id3->Title);
		item.singername->SetText(id3->Artist);
	}
	else
	{
		char buf[256];
		wxf_catch_name_in_path(file_path.c_str(),buf);
		item.songname->SetText(buf);
		//item.songname->SetText(file_path.c_str());
	}
}