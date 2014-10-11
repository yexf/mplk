#include "pch.h"
#include "wxf_mp3st.h"

//////////////////////////////////////////////////////////////////////////
//
//

void wxf_item_data::set_filepath(const char *strFilePath)
{
	wxf_id3 oItem;

	if (strFilePath)
	{
		m_strFilePath = strFilePath;
	}
	if (wxf_get_id3(strFilePath, oItem))
	{
		m_strArtist = oItem.m_strArtist;
		m_strTitle = oItem.m_strTitle;
	}
}

void wxf_item_data::set_fav( bool bFav )
{
	if (m_bIsFav != bFav)
	{
		m_bIsFav = bFav;
	}
}

void wxf_item_data::set_play( bool bPlay )
{
	if (m_bIsPlay != bPlay)
	{
		m_bIsPlay = bPlay;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
int wxf_item_view::m_siNum = 0;
void wxf_item_view::init()
{
	deinit();

	m_pItem = new CListContainerElementUI;
	m_pLayoutMgr = new CHorizontalLayoutUI;

	m_pListno = new CLabelUI;
	m_pFav = new CLabelUI;
	m_pSongname = new CLabelUI;
	m_pSingername = new CLabelUI;
	m_pMVflag = new CLabelUI;

	m_pItem->Add(m_pLayoutMgr);
	m_pItem->SetAttribute("height","23");

	m_pListno->SetAttribute("width","30");
	m_pFav->SetAttribute("width","24");
	//m_pSongname
	m_pSingername->SetAttribute("width","64");
	m_pMVflag->SetAttribute("width","20");

	//标号右对齐
	m_pListno->SetAttribute("align","right");

	//歌手名和标题 较长时用省略号表示
	m_pSongname->SetAttribute("endellipsis","true");
	m_pSingername->SetAttribute("endellipsis","true");

	m_pLayoutMgr->Add(m_pListno);
	m_pLayoutMgr->Add(m_pFav);
	m_pLayoutMgr->Add(m_pSongname);
	m_pLayoutMgr->Add(m_pSingername);
	m_pLayoutMgr->Add(m_pMVflag);

	m_pListno->SetFont(14);
	m_pSingername->SetFont(14);
	m_pSongname->SetFont(14);
	m_pSongname->SetAttribute("textcolor","#7f1f1f1f");
	m_pSingername->SetAttribute("textcolor","#7f1f1f1f");

	m_dwDefBKColor = m_pItem->GetBkColor();
	m_dwPlayBKColor = 0x7f4f4f4f;

	wxf_str temp;
	temp.format(" %d",m_siNum);
	m_iCurNo = m_siNum;
	m_pListno->SetText(temp.data());
	m_pItem->SetTag((UINT_PTR)this);
	
	m_bIsInit = true;
}

void wxf_item_view::deinit()
{
	m_pListno = NULL;
	m_pFav = NULL;
	m_pSongname = NULL;
	m_pSingername = NULL;
	m_pMVflag = NULL;

	m_pLayoutMgr = NULL;
	m_pItem = NULL;

	m_dwDefBKColor = 0x0;
	m_dwPlayBKColor = 0x0;

	m_bIsInit = false;
}


bool wxf_item_view::click_item( TNotifyUI *psender )
{
	if (m_bIsInit && m_pData)
	{
		if (::PtInRect(&m_pFav->GetPos(), psender->ptMouse))
			m_pData->set_fav(!m_pData->get_isfav());
		update_view(m_pData);
		return !m_pData->get_isfav();
	}	
	return false;
}

void wxf_item_view::update_view(wxf_item_data *pData)
{
	if (m_bIsInit && pData)
	{
		m_pData = pData;

		m_pSingername->SetText(pData->get_artist().c_str());
		m_pSongname->SetText(pData->get_title().c_str());

		// 设置fav
		wxf_str name = "CasualListenWhiteHeart.png";
		wxf_str file = "";
		if (!pData->get_isfav())
		{
			file = "file='"+name+"' source='0,0,14,13'";
		}
		else
		{
			file = "file='"+name+"' source='15,0,29,13'";
		}
		wxf_str attr = file + " dest='5,5,19,18'";

		m_pFav->SetBkImage(attr.c_str());

		// 设置是否为当前播放歌曲
		if (pData->get_isplay())
		{
			m_pListno->SetFont(13);
			m_pSingername->SetFont(13);
			m_pSongname->SetFont(13);
			m_pItem->SetBkColor(m_dwPlayBKColor);

		}
		else
		{
			m_pListno->SetFont(14);
			m_pSingername->SetFont(14);
			m_pSongname->SetFont(14);
			m_pItem->SetBkColor(m_dwDefBKColor);
		}
	}
}

