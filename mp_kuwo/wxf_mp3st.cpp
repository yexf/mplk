/************************************************************************************** 
** Copyright (C) 2013 XJSH,Inc. All rights reserved
**
** Filename : wxf_mp3st.c
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/06/05
**
** Description:
**************************************************************************************/ 
#include "wxf_mp3st.h"
#include "id3tag.h"
#ifdef _DEBUG
#pragma comment(lib,"../lib/libid3tag_d.lib")
#pragma comment(lib,"../lib/libz_d.lib")
#else
#pragma comment(lib,"../lib/libid3tag.lib")
#pragma comment(lib,"../lib/libz.lib")
#endif
#pragma warning(disable:4996)

typedef id3_file *	id3_file_t;
//////////////////////////////////////////////////////////////////////////
//
//
bool wxf_id3_isansi(id3_latin1_t *latin1)
{
	int len = strlen((char*)latin1);
	int cnt1 = 0,cnt2 = 0;
	while (*latin1 != '\0')
	{
		if (*latin1 < 0x80)
		{
			cnt1++;
		}
		if (*latin1 == 0xb7)
		{
			cnt2++;
		}
		
		latin1++;
	}
	if (cnt2 == 0 || cnt1 + cnt2 < len)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

//////////////////////////////////////////////////////////////////////////
//
//
bool wxf_get_filedstring(id3_frame *frame, wxf_str &out, int index = 0)
{
	union id3_field const *field;
	id3_field_textencoding textencode;

	unsigned int nstrings, i;
	bool bRst = false;

	field    = id3_frame_field(frame, 1);
	nstrings = id3_field_getnstrings(field);

	for (i = 0; i < nstrings; ++i) 			
	{
		const id3_ucs4_t *ucs4;
		id3_utf8_t *utf8;
		id3_latin1_t *latin1;
		LPSTR ansi = NULL;

		if (i != index)
		{
			continue;
		}

		ucs4 = id3_field_getstrings(field, i);
		assert(ucs4);
		
		textencode = id3_field_gettextencoding(field);

		latin1 = id3_ucs4_latin1duplicate(ucs4);

		if (wxf_id3_isansi(latin1))
		{
			out = (char*)latin1;
		}
		else
		{
			utf8 = id3_ucs4_utf8duplicate(ucs4);
			ansi = StringConvertor::Utf8ToAnsi((LPCSTR)utf8);

			out = ansi;

			StringConvertor::StringFree(ansi);
			free(utf8);			
		}
		free(latin1);	

		out.term();
		bRst = true;
	}
	return bRst;
}

bool wxf_get_filed(id3_tag *tag, const char *id, wxf_str &out, int index = 0)
{
	id3_frame *frame;
	unsigned int i;

	for (i = 0;i < tag->nframes;i++)
	{
		frame = id3_tag_findframe(tag, NULL, i);
		if (frame == NULL)
		{
			continue;
		}
		if (strcmp(id,frame->id) == 0)
		{
			return wxf_get_filedstring(frame,out,index);
		}
	}
	return false;
}

bool wxf_get_id3(const char *filename, wxf_id3 &oid3)
{
	id3_tag *tag;
	id3_file_t pID3File;

	pID3File = id3_file_open(filename, ID3_FILE_MODE_READONLY);
	if (pID3File == NULL)
	{
		return false;
	}

	tag = id3_file_tag(pID3File);
	if (tag != NULL)
	{
		wxf_get_filed(tag,ID3_FRAME_TITLE,oid3.m_strTitle);
		wxf_get_filed(tag,ID3_FRAME_ARTIST,oid3.m_strArtist);
	}

	id3_file_close(pID3File);

	if (oid3.m_strTitle != "" && oid3.m_strArtist != "")
	{
		return true;
	}
	return false;
}
