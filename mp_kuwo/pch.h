#pragma once

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <Windowsx.h>

#pragma warning(disable:4018)

#include <duilib.h>
#include <libzplay.h>
using namespace libZPlay;

#include <wxf_base.h>

#include "wxf_kuwo.h"
#include "wxf_filedlg.h"
#include "wxf_player.h"

#include "wxf_listitem.h"
#include "wxf_playlist.h"
#include "wxf_playctl.h"
#include "wxf_setting.h"

#include "wxf_glog.h"


