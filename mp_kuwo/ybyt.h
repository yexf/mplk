#pragma once
#include <duilib.h>
#include "..\wxf_comm\wxf_comm.h"
#include <libzplay.h>
using namespace libZPlay;
class wxf_playlist;


#include <string>
typedef std::string wxf_str;


typedef enum wxf_common_status_em
{
	EM_INIT = 0,
	EM_READY,
	EM_START,
	EM_RUN,
	EM_PAUSE,
	EM_STOP,
	EM_UNINIT,
}wxf_comms_em;

#include "KuWo.h"

#include "wxf_plitem.h"
#include "wxf_playlist.h"
#include "wxf_player.h"


#include "wxf_add.h"


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include <Windowsx.h>

#pragma warning(disable:4018)

