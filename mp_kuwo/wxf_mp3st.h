/************************************************************************************** 
** Copyright (C) 2013 XJSH,Inc. All rights reserved
**
** Filename : wxf_mp3st.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/06/05
**
** Description:
**************************************************************************************/ 
#ifndef _wxf_mp3st_h_
#define _wxf_mp3st_h_

#include "wxf_base.h"


typedef struct
{
	wxf_str m_strArtist;
	wxf_str m_strTitle;
}wxf_id3,*wxf_pid3;

bool wxf_get_id3(const char *filename, wxf_id3 &oid3);

#endif /*_wxf_mp3st_h_*/
