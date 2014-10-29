#include "wxf_app.h"
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);

	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+"skin");


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
