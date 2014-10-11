#include "pch.h"
#include "wxf_filelist.h"
#pragma warning(disable:4996)

wxf_item_view *wxf_playlist_view::add_item( wxf_item_data *pData,int index)
{
	wxf_item_view *pView = new wxf_item_view;
	pView->update_view(pData);
	m_mapItem[index] = pView;
	m_pList->Add(*m_mapItem[index]);
	return m_mapItem[index];
}


int wxf_playlist_view::del_item(int index)
{
	int iRst = index;
	wxf_item_view *pView = NULL;
	map<int,wxf_item_view*>::iterator pIter = m_mapItem.find(index);

	if (pIter != m_mapItem.end())
	{
		pView = pIter->second;
		m_pList->Remove(*pView);
		delete pView;
		m_mapItem.erase(pIter);
	}

	pIter = m_mapItem.find(index);
	
	while (pIter != m_mapItem.end())
	{
		pView = pIter->second;
		pView->update_view(pView->get_data());
		pIter++;
	}

	return iRst;
}

int wxf_playlist_view::del_item()
{
	int iSelect = m_pList->GetCurSel();
	
	wxf_item_view *pView = get_item(iSelect);
	
	int index = m_pDoc->get_index(pView->get_data());
	map<int,wxf_item_view*>::iterator pIter = m_mapItem.find(index);
	
	if (pIter != m_mapItem.end())
	{
		pView = pIter->second;
		pIter++;

		while (pIter != m_mapItem.end())
		{
			pView->update_view(pIter->second->get_data());
			pView = pIter->second;
			pIter++;
		}

		pIter--;

		m_pList->Remove(*pIter->second);
		delete pIter->second;
		m_mapItem.erase(pIter);

	}
	return index;
}

void wxf_playlist_view::del_all()
{
	map<int,wxf_item_view*>::iterator pIter = m_mapItem.begin();
	while (pIter != m_mapItem.end())
	{
		m_pList->Remove(*pIter->second);
		delete pIter->second;
		pIter++;
	}
	m_mapItem.clear();
}

void wxf_playlist_view::set_play( int index )
{
	map<int,wxf_item_view*>::iterator pOldPlay = m_mapItem.find(m_iPlayIndex);
	map<int,wxf_item_view*>::iterator pNewPlay = m_mapItem.find(index);
	wxf_item_data *pData;

	if (pNewPlay != m_mapItem.end())
	{
		if (pOldPlay != m_mapItem.end())
		{
			pData = pOldPlay->second->get_data();
			pData->set_play(false);
			pOldPlay->second->update_view(pData);
		}
		m_iPlayIndex = index;
		pData = pNewPlay->second->get_data();
		pData->set_play(true);
		pNewPlay->second->update_view(pData);
	}
}

void wxf_playlist_view::set_order( int loopmode, CButtonUI *pOrder )
{
	wxf_str filename,tip;

	int emLoopmode = get_order();
	emLoopmode = loopmode % EM_MODEMAX;

	switch(emLoopmode)
	{
	case EM_LISTLOOP:
		filename = "PlaylistBtnIcoModeCircle.png";
		tip = "循环播放";
		break;
	case EM_LISTPLAY:
		filename = "PlaylistBtnIcoModeOrder.png";
		tip = "顺序播放";
		break;
	case EM_SONGLOOP:
		filename = "PlaylistBtnIcoModeSingle.png";
		tip = "单曲循环";
		break;
	case EM_RANDOMLOOP:
		filename = "PlaylistBtnIcoModeRand.png";
		tip = "随机播放";
		break;
	default:break;

	}
	wxf_str attr = "file='" + filename + "' dest='3,2,44,18'";
	pOrder->SetBkImage(attr.c_str());
	pOrder->SetToolTip(tip.c_str());

	m_emLoopmode = emLoopmode;
}


int wxf_playlist_view::get_mapat( int iNo )
{
	iNo = iNo % m_mapItem.size();
	map<int,wxf_item_view*>::iterator pIter = m_mapItem.begin();

	for (int i = 0; i < iNo;i++)
	{
		pIter++;
	}
	return pIter->first;
}



void wxf_playlist_view::add_doc( wxf_playlist_doc *pDoc,bool bFav )
{
	if (pDoc)
	{
		m_pDoc = pDoc;
		update_view(bFav);
	}
}

int wxf_playlist_view::play_pre()
{
	int playindex = m_iPlayIndex;

	map<int,wxf_item_view*>::iterator pIter = m_mapItem.find(playindex);
	if (pIter == m_mapItem.end())
	{
		return -1;
	}

	switch (get_order())
	{
	case EM_LISTLOOP:
		{
			if (pIter == m_mapItem.begin())
			{
				pIter = m_mapItem.end();
			}
			else
			{
				pIter--;
			}
			return pIter->first;
		}
	case EM_LISTPLAY:
		{
			if (pIter == m_mapItem.begin())
			{
				return -1;
			}
			else
			{
				pIter--;
			}
			return pIter->first;
		}
	case EM_RANDOMLOOP:
		{
			return get_mapat(wxf_rand(playindex));
		}
	case EM_SONGLOOP:
		{
			return playindex;
		}
	}
	return -1;
}

int wxf_playlist_view::play_next()
{
	int playindex = m_iPlayIndex;

	map<int,wxf_item_view*>::iterator pIter = m_mapItem.find(playindex);

	if (pIter == m_mapItem.end())
	{
		return -1;
	}

	switch (get_order())
	{
	case EM_LISTLOOP:
		{
			pIter++;
			if (pIter == m_mapItem.end())
			{
				pIter = m_mapItem.begin();
			}
			return pIter->first;
		}
	case EM_LISTPLAY:
		{
			pIter++;
			if (pIter == m_mapItem.end())
			{
				return -1;
			}
			return pIter->first;
		}
	case EM_RANDOMLOOP:
		{
			return get_mapat(wxf_rand(playindex));
		}
	case EM_SONGLOOP:
		{
			return playindex;
		}
	}
	return -1;
}

void wxf_playlist_view::update_view( bool bFav )
{
	if (m_pDoc == NULL) return;

	del_all();

	for ( int i = 0;i < m_pDoc->get_count();i++)
	{
		wxf_item_data *pData = m_pDoc->get_data(i);
		if (bFav)
		{
			if (!pData->get_isfav())
			{
				pData = NULL;
			}
		}
		if (pData)
		{
			add_item(pData,i);
		}
	}
}



void wxf_playlist_doc::save_list( const char *file_name )
{
	wxf_filemap oFileMap;

	wxf_fileitemvec &oFileList = oFileMap.GetFileList();
	oFileList.clear();

	for (int i = 0; i< m_vecItem.size();i++)
	{
		wxf_fileitem fileitem;
		fileitem.m_strFilePath = m_vecItem[i]->get_filepath().c_str();
		fileitem.m_bIsLikeMusic = m_vecItem[i]->get_isfav();
		oFileList.push_back(fileitem);
	}
	oFileMap.SetFav(m_bFav);
	oFileMap.SaveFile(file_name);
}

bool wxf_playlist_doc::load_list( const char *file_name )
{
	bool ret = false;
	wxf_filemap oFileMap;

	if (!oFileMap.LoadFile(file_name))
	{
		return false;
	}
	wxf_fileitemvec &oFileItemVec = oFileMap.GetFileList();
	for (int i = 0; i < oFileItemVec.size();i++)
	{
		wxf_fileitem &oFileItem = oFileItemVec[i];

		if (wxf_pathfile_exist(oFileItem.m_strFilePath.c_str()) == 0)
		{
			ret = true;
			add_item(oFileItem.m_strFilePath.c_str(),oFileItem.m_bIsLikeMusic);
		}
	}
	m_bFav = oFileMap.GetFav();
	return ret;
}

int wxf_playlist_doc::add_item( const char *file_name,bool bFav )
{
	m_vecItem.push_back(new wxf_item_data());
	wxf_item_data &oData = *m_vecItem.back();
	oData.set_filepath(file_name);
	oData.set_fav(bFav);
	return m_vecItem.size()-1;
}

void wxf_playlist_doc::delete_item( int index )
{
	if (index < m_vecItem.size())
	{
		delete m_vecItem[index];
		m_vecItem.erase(get_iter(index));
	}
}

int wxf_playlist_doc::get_index( wxf_item_data *pData )
{
	if (pData == NULL)
	{
		return -1;
	}
	for (int i = 0;i<m_vecItem.size();i++)
	{
		if (pData == m_vecItem[i])
		{
			return i;
		}
	}
	return -1;
}

vector<wxf_item_data*>::iterator wxf_playlist_doc::get_iter( int index )
{
	if (index < m_vecItem.size())
	{
		vector<wxf_item_data*>::iterator pIter = m_vecItem.begin();
		for (int i= 0;i< index;i++) pIter++;
		return pIter;
	}
	return m_vecItem.end();
}
