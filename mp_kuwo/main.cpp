#include "ybyt.h"
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T(".\\skin"));
	//CPaintManagerUI::SetResourceZip(_T("skin.zip"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	CKuWo* player = new CKuWo();
	
	player->Create(NULL, _T("¿áÎÒÒôÀÖºÐ"), UI_WNDSTYLE_FRAME, 0L);
	player->CenterWindow();
	player->ShowWindow();
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}