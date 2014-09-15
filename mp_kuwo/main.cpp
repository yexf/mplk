#include "pch.h"
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);

#ifdef SKIN_ZIP
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CPaintManagerUI::SetResourceZip("skin.dat");
#else
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+"skin");
#endif	

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	wxf_kuwo* kuwo = new wxf_kuwo();

	kuwo->Create(NULL, "¿áÎÒÒôÀÖºÐ", UI_WNDSTYLE_FRAME, 0L);
	kuwo->CenterWindow();
	kuwo->ShowWindow();
	CPaintManagerUI::MessageLoop();
	delete kuwo;
	::CoUninitialize();
	return 0;
}
