/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : duilib.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/08
**
** Description:
**************************************************************************************/ 

#pragma once

#ifndef __cplusplus
#error "DuiLib ��ֻ������C++��������ļ��ĺ�׺����Ϊ'cpp'"
#endif

#include "..\mp_gui\UIlib.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\lib\\mp_gui_d.lib")
#else
#pragma comment(lib,"..\\lib\\mp_gui.lib")
#endif

using namespace DuiLib;
